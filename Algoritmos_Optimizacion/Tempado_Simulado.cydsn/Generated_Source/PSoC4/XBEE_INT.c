/*******************************************************************************
* File Name: XBEEINT.c
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

#include "XBEE.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (XBEE_RX_INTERRUPT_ENABLED && (XBEE_RX_ENABLED || XBEE_HD_ENABLED))
    /*******************************************************************************
    * Function Name: XBEE_RXISR
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
    *  XBEE_rxBuffer - RAM buffer pointer for save received data.
    *  XBEE_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  XBEE_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  XBEE_rxBufferOverflow - software overflow flag. Set to one
    *     when XBEE_rxBufferWrite index overtakes
    *     XBEE_rxBufferRead index.
    *  XBEE_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when XBEE_rxBufferWrite is equal to
    *    XBEE_rxBufferRead
    *  XBEE_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  XBEE_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(XBEE_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef XBEE_RXISR_ENTRY_CALLBACK
        XBEE_RXISR_EntryCallback();
    #endif /* XBEE_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START XBEE_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = XBEE_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in XBEE_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (XBEE_RX_STS_BREAK | 
                            XBEE_RX_STS_PAR_ERROR |
                            XBEE_RX_STS_STOP_ERROR | 
                            XBEE_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                XBEE_errorStatus |= readStatus & ( XBEE_RX_STS_BREAK | 
                                                            XBEE_RX_STS_PAR_ERROR | 
                                                            XBEE_RX_STS_STOP_ERROR | 
                                                            XBEE_RX_STS_OVERRUN);
                /* `#START XBEE_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef XBEE_RXISR_ERROR_CALLBACK
                XBEE_RXISR_ERROR_Callback();
            #endif /* XBEE_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & XBEE_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = XBEE_RXDATA_REG;
            #if (XBEE_RXHW_ADDRESS_ENABLED)
                if(XBEE_rxAddressMode == (uint8)XBEE__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & XBEE_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & XBEE_RX_STS_ADDR_MATCH) != 0u)
                        {
                            XBEE_rxAddressDetected = 1u;
                        }
                        else
                        {
                            XBEE_rxAddressDetected = 0u;
                        }
                    }
                    if(XBEE_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        XBEE_rxBuffer[XBEE_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    XBEE_rxBuffer[XBEE_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                XBEE_rxBuffer[XBEE_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (XBEE_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(XBEE_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        XBEE_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    XBEE_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(XBEE_rxBufferWrite >= XBEE_RX_BUFFER_SIZE)
                    {
                        XBEE_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(XBEE_rxBufferWrite == XBEE_rxBufferRead)
                    {
                        XBEE_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (XBEE_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            XBEE_RXSTATUS_MASK_REG  &= (uint8)~XBEE_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(XBEE_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (XBEE_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & XBEE_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START XBEE_RXISR_END` */

        /* `#END` */

    #ifdef XBEE_RXISR_EXIT_CALLBACK
        XBEE_RXISR_ExitCallback();
    #endif /* XBEE_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (XBEE_RX_INTERRUPT_ENABLED && (XBEE_RX_ENABLED || XBEE_HD_ENABLED)) */


#if (XBEE_TX_INTERRUPT_ENABLED && XBEE_TX_ENABLED)
    /*******************************************************************************
    * Function Name: XBEE_TXISR
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
    *  XBEE_txBuffer - RAM buffer pointer for transmit data from.
    *  XBEE_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  XBEE_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(XBEE_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef XBEE_TXISR_ENTRY_CALLBACK
        XBEE_TXISR_EntryCallback();
    #endif /* XBEE_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START XBEE_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((XBEE_txBufferRead != XBEE_txBufferWrite) &&
             ((XBEE_TXSTATUS_REG & XBEE_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(XBEE_txBufferRead >= XBEE_TX_BUFFER_SIZE)
            {
                XBEE_txBufferRead = 0u;
            }

            XBEE_TXDATA_REG = XBEE_txBuffer[XBEE_txBufferRead];

            /* Set next pointer */
            XBEE_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START XBEE_TXISR_END` */

        /* `#END` */

    #ifdef XBEE_TXISR_EXIT_CALLBACK
        XBEE_TXISR_ExitCallback();
    #endif /* XBEE_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (XBEE_TX_INTERRUPT_ENABLED && XBEE_TX_ENABLED) */


/* [] END OF FILE */
