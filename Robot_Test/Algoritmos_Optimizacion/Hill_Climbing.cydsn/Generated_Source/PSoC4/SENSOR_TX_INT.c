/*******************************************************************************
* File Name: SENSOR_TXINT.c
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

#include "SENSOR_TX.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (SENSOR_TX_RX_INTERRUPT_ENABLED && (SENSOR_TX_RX_ENABLED || SENSOR_TX_HD_ENABLED))
    /*******************************************************************************
    * Function Name: SENSOR_TX_RXISR
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
    *  SENSOR_TX_rxBuffer - RAM buffer pointer for save received data.
    *  SENSOR_TX_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  SENSOR_TX_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  SENSOR_TX_rxBufferOverflow - software overflow flag. Set to one
    *     when SENSOR_TX_rxBufferWrite index overtakes
    *     SENSOR_TX_rxBufferRead index.
    *  SENSOR_TX_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when SENSOR_TX_rxBufferWrite is equal to
    *    SENSOR_TX_rxBufferRead
    *  SENSOR_TX_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  SENSOR_TX_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(SENSOR_TX_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef SENSOR_TX_RXISR_ENTRY_CALLBACK
        SENSOR_TX_RXISR_EntryCallback();
    #endif /* SENSOR_TX_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START SENSOR_TX_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = SENSOR_TX_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in SENSOR_TX_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (SENSOR_TX_RX_STS_BREAK | 
                            SENSOR_TX_RX_STS_PAR_ERROR |
                            SENSOR_TX_RX_STS_STOP_ERROR | 
                            SENSOR_TX_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                SENSOR_TX_errorStatus |= readStatus & ( SENSOR_TX_RX_STS_BREAK | 
                                                            SENSOR_TX_RX_STS_PAR_ERROR | 
                                                            SENSOR_TX_RX_STS_STOP_ERROR | 
                                                            SENSOR_TX_RX_STS_OVERRUN);
                /* `#START SENSOR_TX_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef SENSOR_TX_RXISR_ERROR_CALLBACK
                SENSOR_TX_RXISR_ERROR_Callback();
            #endif /* SENSOR_TX_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & SENSOR_TX_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = SENSOR_TX_RXDATA_REG;
            #if (SENSOR_TX_RXHW_ADDRESS_ENABLED)
                if(SENSOR_TX_rxAddressMode == (uint8)SENSOR_TX__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & SENSOR_TX_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & SENSOR_TX_RX_STS_ADDR_MATCH) != 0u)
                        {
                            SENSOR_TX_rxAddressDetected = 1u;
                        }
                        else
                        {
                            SENSOR_TX_rxAddressDetected = 0u;
                        }
                    }
                    if(SENSOR_TX_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        SENSOR_TX_rxBuffer[SENSOR_TX_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    SENSOR_TX_rxBuffer[SENSOR_TX_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                SENSOR_TX_rxBuffer[SENSOR_TX_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (SENSOR_TX_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(SENSOR_TX_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        SENSOR_TX_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    SENSOR_TX_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(SENSOR_TX_rxBufferWrite >= SENSOR_TX_RX_BUFFER_SIZE)
                    {
                        SENSOR_TX_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(SENSOR_TX_rxBufferWrite == SENSOR_TX_rxBufferRead)
                    {
                        SENSOR_TX_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (SENSOR_TX_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            SENSOR_TX_RXSTATUS_MASK_REG  &= (uint8)~SENSOR_TX_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(SENSOR_TX_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (SENSOR_TX_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & SENSOR_TX_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START SENSOR_TX_RXISR_END` */

        /* `#END` */

    #ifdef SENSOR_TX_RXISR_EXIT_CALLBACK
        SENSOR_TX_RXISR_ExitCallback();
    #endif /* SENSOR_TX_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (SENSOR_TX_RX_INTERRUPT_ENABLED && (SENSOR_TX_RX_ENABLED || SENSOR_TX_HD_ENABLED)) */


#if (SENSOR_TX_TX_INTERRUPT_ENABLED && SENSOR_TX_TX_ENABLED)
    /*******************************************************************************
    * Function Name: SENSOR_TX_TXISR
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
    *  SENSOR_TX_txBuffer - RAM buffer pointer for transmit data from.
    *  SENSOR_TX_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  SENSOR_TX_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(SENSOR_TX_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef SENSOR_TX_TXISR_ENTRY_CALLBACK
        SENSOR_TX_TXISR_EntryCallback();
    #endif /* SENSOR_TX_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START SENSOR_TX_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((SENSOR_TX_txBufferRead != SENSOR_TX_txBufferWrite) &&
             ((SENSOR_TX_TXSTATUS_REG & SENSOR_TX_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(SENSOR_TX_txBufferRead >= SENSOR_TX_TX_BUFFER_SIZE)
            {
                SENSOR_TX_txBufferRead = 0u;
            }

            SENSOR_TX_TXDATA_REG = SENSOR_TX_txBuffer[SENSOR_TX_txBufferRead];

            /* Set next pointer */
            SENSOR_TX_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START SENSOR_TX_TXISR_END` */

        /* `#END` */

    #ifdef SENSOR_TX_TXISR_EXIT_CALLBACK
        SENSOR_TX_TXISR_ExitCallback();
    #endif /* SENSOR_TX_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (SENSOR_TX_TX_INTERRUPT_ENABLED && SENSOR_TX_TX_ENABLED) */


/* [] END OF FILE */
