/*******************************************************************************
* File Name: RECIBEINT.c
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

#include "RECIBE.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (RECIBE_RX_INTERRUPT_ENABLED && (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED))
    /*******************************************************************************
    * Function Name: RECIBE_RXISR
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
    *  RECIBE_rxBuffer - RAM buffer pointer for save received data.
    *  RECIBE_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  RECIBE_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  RECIBE_rxBufferOverflow - software overflow flag. Set to one
    *     when RECIBE_rxBufferWrite index overtakes
    *     RECIBE_rxBufferRead index.
    *  RECIBE_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when RECIBE_rxBufferWrite is equal to
    *    RECIBE_rxBufferRead
    *  RECIBE_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  RECIBE_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(RECIBE_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef RECIBE_RXISR_ENTRY_CALLBACK
        RECIBE_RXISR_EntryCallback();
    #endif /* RECIBE_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START RECIBE_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = RECIBE_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in RECIBE_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (RECIBE_RX_STS_BREAK | 
                            RECIBE_RX_STS_PAR_ERROR |
                            RECIBE_RX_STS_STOP_ERROR | 
                            RECIBE_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                RECIBE_errorStatus |= readStatus & ( RECIBE_RX_STS_BREAK | 
                                                            RECIBE_RX_STS_PAR_ERROR | 
                                                            RECIBE_RX_STS_STOP_ERROR | 
                                                            RECIBE_RX_STS_OVERRUN);
                /* `#START RECIBE_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef RECIBE_RXISR_ERROR_CALLBACK
                RECIBE_RXISR_ERROR_Callback();
            #endif /* RECIBE_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & RECIBE_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = RECIBE_RXDATA_REG;
            #if (RECIBE_RXHW_ADDRESS_ENABLED)
                if(RECIBE_rxAddressMode == (uint8)RECIBE__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & RECIBE_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & RECIBE_RX_STS_ADDR_MATCH) != 0u)
                        {
                            RECIBE_rxAddressDetected = 1u;
                        }
                        else
                        {
                            RECIBE_rxAddressDetected = 0u;
                        }
                    }
                    if(RECIBE_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        RECIBE_rxBuffer[RECIBE_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    RECIBE_rxBuffer[RECIBE_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                RECIBE_rxBuffer[RECIBE_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (RECIBE_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(RECIBE_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        RECIBE_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    RECIBE_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(RECIBE_rxBufferWrite >= RECIBE_RX_BUFFER_SIZE)
                    {
                        RECIBE_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(RECIBE_rxBufferWrite == RECIBE_rxBufferRead)
                    {
                        RECIBE_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (RECIBE_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            RECIBE_RXSTATUS_MASK_REG  &= (uint8)~RECIBE_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(RECIBE_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (RECIBE_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & RECIBE_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START RECIBE_RXISR_END` */

        /* `#END` */

    #ifdef RECIBE_RXISR_EXIT_CALLBACK
        RECIBE_RXISR_ExitCallback();
    #endif /* RECIBE_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (RECIBE_RX_INTERRUPT_ENABLED && (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)) */


#if (RECIBE_TX_INTERRUPT_ENABLED && RECIBE_TX_ENABLED)
    /*******************************************************************************
    * Function Name: RECIBE_TXISR
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
    *  RECIBE_txBuffer - RAM buffer pointer for transmit data from.
    *  RECIBE_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  RECIBE_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(RECIBE_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef RECIBE_TXISR_ENTRY_CALLBACK
        RECIBE_TXISR_EntryCallback();
    #endif /* RECIBE_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START RECIBE_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((RECIBE_txBufferRead != RECIBE_txBufferWrite) &&
             ((RECIBE_TXSTATUS_REG & RECIBE_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(RECIBE_txBufferRead >= RECIBE_TX_BUFFER_SIZE)
            {
                RECIBE_txBufferRead = 0u;
            }

            RECIBE_TXDATA_REG = RECIBE_txBuffer[RECIBE_txBufferRead];

            /* Set next pointer */
            RECIBE_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START RECIBE_TXISR_END` */

        /* `#END` */

    #ifdef RECIBE_TXISR_EXIT_CALLBACK
        RECIBE_TXISR_ExitCallback();
    #endif /* RECIBE_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (RECIBE_TX_INTERRUPT_ENABLED && RECIBE_TX_ENABLED) */


/* [] END OF FILE */
