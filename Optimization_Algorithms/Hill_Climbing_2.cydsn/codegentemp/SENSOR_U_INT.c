/*******************************************************************************
* File Name: SENSOR_UINT.c
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

#include "SENSOR_U.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (SENSOR_U_RX_INTERRUPT_ENABLED && (SENSOR_U_RX_ENABLED || SENSOR_U_HD_ENABLED))
    /*******************************************************************************
    * Function Name: SENSOR_U_RXISR
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
    *  SENSOR_U_rxBuffer - RAM buffer pointer for save received data.
    *  SENSOR_U_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  SENSOR_U_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  SENSOR_U_rxBufferOverflow - software overflow flag. Set to one
    *     when SENSOR_U_rxBufferWrite index overtakes
    *     SENSOR_U_rxBufferRead index.
    *  SENSOR_U_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when SENSOR_U_rxBufferWrite is equal to
    *    SENSOR_U_rxBufferRead
    *  SENSOR_U_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  SENSOR_U_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(SENSOR_U_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef SENSOR_U_RXISR_ENTRY_CALLBACK
        SENSOR_U_RXISR_EntryCallback();
    #endif /* SENSOR_U_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START SENSOR_U_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = SENSOR_U_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in SENSOR_U_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (SENSOR_U_RX_STS_BREAK | 
                            SENSOR_U_RX_STS_PAR_ERROR |
                            SENSOR_U_RX_STS_STOP_ERROR | 
                            SENSOR_U_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                SENSOR_U_errorStatus |= readStatus & ( SENSOR_U_RX_STS_BREAK | 
                                                            SENSOR_U_RX_STS_PAR_ERROR | 
                                                            SENSOR_U_RX_STS_STOP_ERROR | 
                                                            SENSOR_U_RX_STS_OVERRUN);
                /* `#START SENSOR_U_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef SENSOR_U_RXISR_ERROR_CALLBACK
                SENSOR_U_RXISR_ERROR_Callback();
            #endif /* SENSOR_U_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & SENSOR_U_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = SENSOR_U_RXDATA_REG;
            #if (SENSOR_U_RXHW_ADDRESS_ENABLED)
                if(SENSOR_U_rxAddressMode == (uint8)SENSOR_U__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & SENSOR_U_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & SENSOR_U_RX_STS_ADDR_MATCH) != 0u)
                        {
                            SENSOR_U_rxAddressDetected = 1u;
                        }
                        else
                        {
                            SENSOR_U_rxAddressDetected = 0u;
                        }
                    }
                    if(SENSOR_U_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        SENSOR_U_rxBuffer[SENSOR_U_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    SENSOR_U_rxBuffer[SENSOR_U_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                SENSOR_U_rxBuffer[SENSOR_U_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (SENSOR_U_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(SENSOR_U_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        SENSOR_U_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    SENSOR_U_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(SENSOR_U_rxBufferWrite >= SENSOR_U_RX_BUFFER_SIZE)
                    {
                        SENSOR_U_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(SENSOR_U_rxBufferWrite == SENSOR_U_rxBufferRead)
                    {
                        SENSOR_U_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (SENSOR_U_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            SENSOR_U_RXSTATUS_MASK_REG  &= (uint8)~SENSOR_U_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(SENSOR_U_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (SENSOR_U_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & SENSOR_U_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START SENSOR_U_RXISR_END` */

        /* `#END` */

    #ifdef SENSOR_U_RXISR_EXIT_CALLBACK
        SENSOR_U_RXISR_ExitCallback();
    #endif /* SENSOR_U_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (SENSOR_U_RX_INTERRUPT_ENABLED && (SENSOR_U_RX_ENABLED || SENSOR_U_HD_ENABLED)) */


#if (SENSOR_U_TX_INTERRUPT_ENABLED && SENSOR_U_TX_ENABLED)
    /*******************************************************************************
    * Function Name: SENSOR_U_TXISR
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
    *  SENSOR_U_txBuffer - RAM buffer pointer for transmit data from.
    *  SENSOR_U_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  SENSOR_U_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(SENSOR_U_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef SENSOR_U_TXISR_ENTRY_CALLBACK
        SENSOR_U_TXISR_EntryCallback();
    #endif /* SENSOR_U_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START SENSOR_U_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((SENSOR_U_txBufferRead != SENSOR_U_txBufferWrite) &&
             ((SENSOR_U_TXSTATUS_REG & SENSOR_U_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(SENSOR_U_txBufferRead >= SENSOR_U_TX_BUFFER_SIZE)
            {
                SENSOR_U_txBufferRead = 0u;
            }

            SENSOR_U_TXDATA_REG = SENSOR_U_txBuffer[SENSOR_U_txBufferRead];

            /* Set next pointer */
            SENSOR_U_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START SENSOR_U_TXISR_END` */

        /* `#END` */

    #ifdef SENSOR_U_TXISR_EXIT_CALLBACK
        SENSOR_U_TXISR_ExitCallback();
    #endif /* SENSOR_U_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (SENSOR_U_TX_INTERRUPT_ENABLED && SENSOR_U_TX_ENABLED) */


/* [] END OF FILE */
