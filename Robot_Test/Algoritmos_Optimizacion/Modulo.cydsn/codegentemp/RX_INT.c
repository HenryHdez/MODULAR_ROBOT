/*******************************************************************************
* File Name: RXINT.c
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

#include "RX.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (RX_RX_INTERRUPT_ENABLED && (RX_RX_ENABLED || RX_HD_ENABLED))
    /*******************************************************************************
    * Function Name: RX_RXISR
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
    *  RX_rxBuffer - RAM buffer pointer for save received data.
    *  RX_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  RX_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  RX_rxBufferOverflow - software overflow flag. Set to one
    *     when RX_rxBufferWrite index overtakes
    *     RX_rxBufferRead index.
    *  RX_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when RX_rxBufferWrite is equal to
    *    RX_rxBufferRead
    *  RX_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  RX_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(RX_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef RX_RXISR_ENTRY_CALLBACK
        RX_RXISR_EntryCallback();
    #endif /* RX_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START RX_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = RX_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in RX_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (RX_RX_STS_BREAK | 
                            RX_RX_STS_PAR_ERROR |
                            RX_RX_STS_STOP_ERROR | 
                            RX_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                RX_errorStatus |= readStatus & ( RX_RX_STS_BREAK | 
                                                            RX_RX_STS_PAR_ERROR | 
                                                            RX_RX_STS_STOP_ERROR | 
                                                            RX_RX_STS_OVERRUN);
                /* `#START RX_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef RX_RXISR_ERROR_CALLBACK
                RX_RXISR_ERROR_Callback();
            #endif /* RX_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & RX_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = RX_RXDATA_REG;
            #if (RX_RXHW_ADDRESS_ENABLED)
                if(RX_rxAddressMode == (uint8)RX__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & RX_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & RX_RX_STS_ADDR_MATCH) != 0u)
                        {
                            RX_rxAddressDetected = 1u;
                        }
                        else
                        {
                            RX_rxAddressDetected = 0u;
                        }
                    }
                    if(RX_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        RX_rxBuffer[RX_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    RX_rxBuffer[RX_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                RX_rxBuffer[RX_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (RX_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(RX_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        RX_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    RX_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(RX_rxBufferWrite >= RX_RX_BUFFER_SIZE)
                    {
                        RX_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(RX_rxBufferWrite == RX_rxBufferRead)
                    {
                        RX_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (RX_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            RX_RXSTATUS_MASK_REG  &= (uint8)~RX_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(RX_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (RX_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & RX_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START RX_RXISR_END` */

        /* `#END` */

    #ifdef RX_RXISR_EXIT_CALLBACK
        RX_RXISR_ExitCallback();
    #endif /* RX_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (RX_RX_INTERRUPT_ENABLED && (RX_RX_ENABLED || RX_HD_ENABLED)) */


#if (RX_TX_INTERRUPT_ENABLED && RX_TX_ENABLED)
    /*******************************************************************************
    * Function Name: RX_TXISR
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
    *  RX_txBuffer - RAM buffer pointer for transmit data from.
    *  RX_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  RX_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(RX_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef RX_TXISR_ENTRY_CALLBACK
        RX_TXISR_EntryCallback();
    #endif /* RX_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START RX_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((RX_txBufferRead != RX_txBufferWrite) &&
             ((RX_TXSTATUS_REG & RX_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(RX_txBufferRead >= RX_TX_BUFFER_SIZE)
            {
                RX_txBufferRead = 0u;
            }

            RX_TXDATA_REG = RX_txBuffer[RX_txBufferRead];

            /* Set next pointer */
            RX_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START RX_TXISR_END` */

        /* `#END` */

    #ifdef RX_TXISR_EXIT_CALLBACK
        RX_TXISR_ExitCallback();
    #endif /* RX_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (RX_TX_INTERRUPT_ENABLED && RX_TX_ENABLED) */


/* [] END OF FILE */
