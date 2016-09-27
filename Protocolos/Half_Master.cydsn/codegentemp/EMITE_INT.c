/*******************************************************************************
* File Name: EMITEINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EMITE.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (EMITE_RX_INTERRUPT_ENABLED && (EMITE_RX_ENABLED || EMITE_HD_ENABLED))
    /*******************************************************************************
    * Function Name: EMITE_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_rxBuffer - RAM buffer pointer for save received data.
    *  EMITE_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  EMITE_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  EMITE_rxBufferOverflow - software overflow flag. Set to one
    *     when EMITE_rxBufferWrite index overtakes
    *     EMITE_rxBufferRead index.
    *  EMITE_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when EMITE_rxBufferWrite is equal to
    *    EMITE_rxBufferRead
    *  EMITE_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  EMITE_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(EMITE_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef EMITE_RXISR_ENTRY_CALLBACK
        EMITE_RXISR_EntryCallback();
    #endif /* EMITE_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START EMITE_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = EMITE_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in EMITE_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (EMITE_RX_STS_BREAK | 
                            EMITE_RX_STS_PAR_ERROR |
                            EMITE_RX_STS_STOP_ERROR | 
                            EMITE_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                EMITE_errorStatus |= readStatus & ( EMITE_RX_STS_BREAK | 
                                                            EMITE_RX_STS_PAR_ERROR | 
                                                            EMITE_RX_STS_STOP_ERROR | 
                                                            EMITE_RX_STS_OVERRUN);
                /* `#START EMITE_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef EMITE_RXISR_ERROR_CALLBACK
                EMITE_RXISR_ERROR_Callback();
            #endif /* EMITE_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & EMITE_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = EMITE_RXDATA_REG;
            #if (EMITE_RXHW_ADDRESS_ENABLED)
                if(EMITE_rxAddressMode == (uint8)EMITE__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & EMITE_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & EMITE_RX_STS_ADDR_MATCH) != 0u)
                        {
                            EMITE_rxAddressDetected = 1u;
                        }
                        else
                        {
                            EMITE_rxAddressDetected = 0u;
                        }
                    }
                    if(EMITE_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        EMITE_rxBuffer[EMITE_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    EMITE_rxBuffer[EMITE_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                EMITE_rxBuffer[EMITE_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (EMITE_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(EMITE_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        EMITE_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    EMITE_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(EMITE_rxBufferWrite >= EMITE_RX_BUFFER_SIZE)
                    {
                        EMITE_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(EMITE_rxBufferWrite == EMITE_rxBufferRead)
                    {
                        EMITE_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (EMITE_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            EMITE_RXSTATUS_MASK_REG  &= (uint8)~EMITE_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(EMITE_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (EMITE_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & EMITE_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START EMITE_RXISR_END` */

        /* `#END` */

    #ifdef EMITE_RXISR_EXIT_CALLBACK
        EMITE_RXISR_ExitCallback();
    #endif /* EMITE_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (EMITE_RX_INTERRUPT_ENABLED && (EMITE_RX_ENABLED || EMITE_HD_ENABLED)) */


#if (EMITE_TX_INTERRUPT_ENABLED && EMITE_TX_ENABLED)
    /*******************************************************************************
    * Function Name: EMITE_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_txBuffer - RAM buffer pointer for transmit data from.
    *  EMITE_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  EMITE_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(EMITE_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef EMITE_TXISR_ENTRY_CALLBACK
        EMITE_TXISR_EntryCallback();
    #endif /* EMITE_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START EMITE_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((EMITE_txBufferRead != EMITE_txBufferWrite) &&
             ((EMITE_TXSTATUS_REG & EMITE_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(EMITE_txBufferRead >= EMITE_TX_BUFFER_SIZE)
            {
                EMITE_txBufferRead = 0u;
            }

            EMITE_TXDATA_REG = EMITE_txBuffer[EMITE_txBufferRead];

            /* Set next pointer */
            EMITE_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START EMITE_TXISR_END` */

        /* `#END` */

    #ifdef EMITE_TXISR_EXIT_CALLBACK
        EMITE_TXISR_ExitCallback();
    #endif /* EMITE_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (EMITE_TX_INTERRUPT_ENABLED && EMITE_TX_ENABLED) */


/* [] END OF FILE */
