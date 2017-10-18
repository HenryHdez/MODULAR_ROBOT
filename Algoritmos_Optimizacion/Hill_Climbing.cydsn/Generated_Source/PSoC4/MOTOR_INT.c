/*******************************************************************************
* File Name: MOTORINT.c
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

#include "MOTOR.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (MOTOR_RX_INTERRUPT_ENABLED && (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED))
    /*******************************************************************************
    * Function Name: MOTOR_RXISR
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
    *  MOTOR_rxBuffer - RAM buffer pointer for save received data.
    *  MOTOR_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  MOTOR_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  MOTOR_rxBufferOverflow - software overflow flag. Set to one
    *     when MOTOR_rxBufferWrite index overtakes
    *     MOTOR_rxBufferRead index.
    *  MOTOR_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when MOTOR_rxBufferWrite is equal to
    *    MOTOR_rxBufferRead
    *  MOTOR_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  MOTOR_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(MOTOR_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef MOTOR_RXISR_ENTRY_CALLBACK
        MOTOR_RXISR_EntryCallback();
    #endif /* MOTOR_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START MOTOR_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = MOTOR_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in MOTOR_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (MOTOR_RX_STS_BREAK | 
                            MOTOR_RX_STS_PAR_ERROR |
                            MOTOR_RX_STS_STOP_ERROR | 
                            MOTOR_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                MOTOR_errorStatus |= readStatus & ( MOTOR_RX_STS_BREAK | 
                                                            MOTOR_RX_STS_PAR_ERROR | 
                                                            MOTOR_RX_STS_STOP_ERROR | 
                                                            MOTOR_RX_STS_OVERRUN);
                /* `#START MOTOR_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef MOTOR_RXISR_ERROR_CALLBACK
                MOTOR_RXISR_ERROR_Callback();
            #endif /* MOTOR_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & MOTOR_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = MOTOR_RXDATA_REG;
            #if (MOTOR_RXHW_ADDRESS_ENABLED)
                if(MOTOR_rxAddressMode == (uint8)MOTOR__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & MOTOR_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & MOTOR_RX_STS_ADDR_MATCH) != 0u)
                        {
                            MOTOR_rxAddressDetected = 1u;
                        }
                        else
                        {
                            MOTOR_rxAddressDetected = 0u;
                        }
                    }
                    if(MOTOR_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        MOTOR_rxBuffer[MOTOR_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    MOTOR_rxBuffer[MOTOR_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                MOTOR_rxBuffer[MOTOR_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (MOTOR_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(MOTOR_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        MOTOR_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    MOTOR_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(MOTOR_rxBufferWrite >= MOTOR_RX_BUFFER_SIZE)
                    {
                        MOTOR_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(MOTOR_rxBufferWrite == MOTOR_rxBufferRead)
                    {
                        MOTOR_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (MOTOR_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            MOTOR_RXSTATUS_MASK_REG  &= (uint8)~MOTOR_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(MOTOR_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (MOTOR_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & MOTOR_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START MOTOR_RXISR_END` */

        /* `#END` */

    #ifdef MOTOR_RXISR_EXIT_CALLBACK
        MOTOR_RXISR_ExitCallback();
    #endif /* MOTOR_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (MOTOR_RX_INTERRUPT_ENABLED && (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)) */


#if (MOTOR_TX_INTERRUPT_ENABLED && MOTOR_TX_ENABLED)
    /*******************************************************************************
    * Function Name: MOTOR_TXISR
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
    *  MOTOR_txBuffer - RAM buffer pointer for transmit data from.
    *  MOTOR_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  MOTOR_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(MOTOR_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef MOTOR_TXISR_ENTRY_CALLBACK
        MOTOR_TXISR_EntryCallback();
    #endif /* MOTOR_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START MOTOR_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((MOTOR_txBufferRead != MOTOR_txBufferWrite) &&
             ((MOTOR_TXSTATUS_REG & MOTOR_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(MOTOR_txBufferRead >= MOTOR_TX_BUFFER_SIZE)
            {
                MOTOR_txBufferRead = 0u;
            }

            MOTOR_TXDATA_REG = MOTOR_txBuffer[MOTOR_txBufferRead];

            /* Set next pointer */
            MOTOR_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START MOTOR_TXISR_END` */

        /* `#END` */

    #ifdef MOTOR_TXISR_EXIT_CALLBACK
        MOTOR_TXISR_ExitCallback();
    #endif /* MOTOR_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (MOTOR_TX_INTERRUPT_ENABLED && MOTOR_TX_ENABLED) */


/* [] END OF FILE */
