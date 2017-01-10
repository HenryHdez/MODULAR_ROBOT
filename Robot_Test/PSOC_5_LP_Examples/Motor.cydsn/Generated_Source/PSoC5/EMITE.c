/*******************************************************************************
* File Name: EMITE.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EMITE.h"
#if (EMITE_INTERNAL_CLOCK_USED)
    #include "EMITE_IntClock.h"
#endif /* End EMITE_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 EMITE_initVar = 0u;

#if (EMITE_TX_INTERRUPT_ENABLED && EMITE_TX_ENABLED)
    volatile uint8 EMITE_txBuffer[EMITE_TX_BUFFER_SIZE];
    volatile uint8 EMITE_txBufferRead = 0u;
    uint8 EMITE_txBufferWrite = 0u;
#endif /* (EMITE_TX_INTERRUPT_ENABLED && EMITE_TX_ENABLED) */

#if (EMITE_RX_INTERRUPT_ENABLED && (EMITE_RX_ENABLED || EMITE_HD_ENABLED))
    uint8 EMITE_errorStatus = 0u;
    volatile uint8 EMITE_rxBuffer[EMITE_RX_BUFFER_SIZE];
    volatile uint8 EMITE_rxBufferRead  = 0u;
    volatile uint8 EMITE_rxBufferWrite = 0u;
    volatile uint8 EMITE_rxBufferLoopDetect = 0u;
    volatile uint8 EMITE_rxBufferOverflow   = 0u;
    #if (EMITE_RXHW_ADDRESS_ENABLED)
        volatile uint8 EMITE_rxAddressMode = EMITE_RX_ADDRESS_MODE;
        volatile uint8 EMITE_rxAddressDetected = 0u;
    #endif /* (EMITE_RXHW_ADDRESS_ENABLED) */
#endif /* (EMITE_RX_INTERRUPT_ENABLED && (EMITE_RX_ENABLED || EMITE_HD_ENABLED)) */


/*******************************************************************************
* Function Name: EMITE_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  EMITE_Start() sets the initVar variable, calls the
*  EMITE_Init() function, and then calls the
*  EMITE_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The EMITE_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time EMITE_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the EMITE_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EMITE_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(EMITE_initVar == 0u)
    {
        EMITE_Init();
        EMITE_initVar = 1u;
    }

    EMITE_Enable();
}


/*******************************************************************************
* Function Name: EMITE_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call EMITE_Init() because
*  the EMITE_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void EMITE_Init(void) 
{
    #if(EMITE_RX_ENABLED || EMITE_HD_ENABLED)

        #if (EMITE_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(EMITE_RX_VECT_NUM, &EMITE_RXISR);
            CyIntSetPriority(EMITE_RX_VECT_NUM, EMITE_RX_PRIOR_NUM);
            EMITE_errorStatus = 0u;
        #endif /* (EMITE_RX_INTERRUPT_ENABLED) */

        #if (EMITE_RXHW_ADDRESS_ENABLED)
            EMITE_SetRxAddressMode(EMITE_RX_ADDRESS_MODE);
            EMITE_SetRxAddress1(EMITE_RX_HW_ADDRESS1);
            EMITE_SetRxAddress2(EMITE_RX_HW_ADDRESS2);
        #endif /* End EMITE_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        EMITE_RXBITCTR_PERIOD_REG = EMITE_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        EMITE_RXSTATUS_MASK_REG  = EMITE_INIT_RX_INTERRUPTS_MASK;
    #endif /* End EMITE_RX_ENABLED || EMITE_HD_ENABLED*/

    #if(EMITE_TX_ENABLED)
        #if (EMITE_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(EMITE_TX_VECT_NUM, &EMITE_TXISR);
            CyIntSetPriority(EMITE_TX_VECT_NUM, EMITE_TX_PRIOR_NUM);
        #endif /* (EMITE_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (EMITE_TXCLKGEN_DP)
            EMITE_TXBITCLKGEN_CTR_REG = EMITE_BIT_CENTER;
            EMITE_TXBITCLKTX_COMPLETE_REG = ((EMITE_NUMBER_OF_DATA_BITS +
                        EMITE_NUMBER_OF_START_BIT) * EMITE_OVER_SAMPLE_COUNT) - 1u;
        #else
            EMITE_TXBITCTR_PERIOD_REG = ((EMITE_NUMBER_OF_DATA_BITS +
                        EMITE_NUMBER_OF_START_BIT) * EMITE_OVER_SAMPLE_8) - 1u;
        #endif /* End EMITE_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (EMITE_TX_INTERRUPT_ENABLED)
            EMITE_TXSTATUS_MASK_REG = EMITE_TX_STS_FIFO_EMPTY;
        #else
            EMITE_TXSTATUS_MASK_REG = EMITE_INIT_TX_INTERRUPTS_MASK;
        #endif /*End EMITE_TX_INTERRUPT_ENABLED*/

    #endif /* End EMITE_TX_ENABLED */

    #if(EMITE_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        EMITE_WriteControlRegister( \
            (EMITE_ReadControlRegister() & (uint8)~EMITE_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(EMITE_PARITY_TYPE << EMITE_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End EMITE_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: EMITE_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call EMITE_Enable() because the EMITE_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  EMITE_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void EMITE_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (EMITE_RX_ENABLED || EMITE_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        EMITE_RXBITCTR_CONTROL_REG |= EMITE_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        EMITE_RXSTATUS_ACTL_REG  |= EMITE_INT_ENABLE;

        #if (EMITE_RX_INTERRUPT_ENABLED)
            EMITE_EnableRxInt();

            #if (EMITE_RXHW_ADDRESS_ENABLED)
                EMITE_rxAddressDetected = 0u;
            #endif /* (EMITE_RXHW_ADDRESS_ENABLED) */
        #endif /* (EMITE_RX_INTERRUPT_ENABLED) */
    #endif /* (EMITE_RX_ENABLED || EMITE_HD_ENABLED) */

    #if(EMITE_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!EMITE_TXCLKGEN_DP)
            EMITE_TXBITCTR_CONTROL_REG |= EMITE_CNTR_ENABLE;
        #endif /* End EMITE_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        EMITE_TXSTATUS_ACTL_REG |= EMITE_INT_ENABLE;
        #if (EMITE_TX_INTERRUPT_ENABLED)
            EMITE_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            EMITE_EnableTxInt();
        #endif /* (EMITE_TX_INTERRUPT_ENABLED) */
     #endif /* (EMITE_TX_INTERRUPT_ENABLED) */

    #if (EMITE_INTERNAL_CLOCK_USED)
        EMITE_IntClock_Start();  /* Enable the clock */
    #endif /* (EMITE_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: EMITE_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void EMITE_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (EMITE_RX_ENABLED || EMITE_HD_ENABLED)
        EMITE_RXBITCTR_CONTROL_REG &= (uint8) ~EMITE_CNTR_ENABLE;
    #endif /* (EMITE_RX_ENABLED || EMITE_HD_ENABLED) */

    #if (EMITE_TX_ENABLED)
        #if(!EMITE_TXCLKGEN_DP)
            EMITE_TXBITCTR_CONTROL_REG &= (uint8) ~EMITE_CNTR_ENABLE;
        #endif /* (!EMITE_TXCLKGEN_DP) */
    #endif /* (EMITE_TX_ENABLED) */

    #if (EMITE_INTERNAL_CLOCK_USED)
        EMITE_IntClock_Stop();   /* Disable the clock */
    #endif /* (EMITE_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (EMITE_RX_ENABLED || EMITE_HD_ENABLED)
        EMITE_RXSTATUS_ACTL_REG  &= (uint8) ~EMITE_INT_ENABLE;

        #if (EMITE_RX_INTERRUPT_ENABLED)
            EMITE_DisableRxInt();
        #endif /* (EMITE_RX_INTERRUPT_ENABLED) */
    #endif /* (EMITE_RX_ENABLED || EMITE_HD_ENABLED) */

    #if (EMITE_TX_ENABLED)
        EMITE_TXSTATUS_ACTL_REG &= (uint8) ~EMITE_INT_ENABLE;

        #if (EMITE_TX_INTERRUPT_ENABLED)
            EMITE_DisableTxInt();
        #endif /* (EMITE_TX_INTERRUPT_ENABLED) */
    #endif /* (EMITE_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: EMITE_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 EMITE_ReadControlRegister(void) 
{
    #if (EMITE_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(EMITE_CONTROL_REG);
    #endif /* (EMITE_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: EMITE_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  EMITE_WriteControlRegister(uint8 control) 
{
    #if (EMITE_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       EMITE_CONTROL_REG = control;
    #endif /* (EMITE_CONTROL_REG_REMOVED) */
}


#if(EMITE_RX_ENABLED || EMITE_HD_ENABLED)
    /*******************************************************************************
    * Function Name: EMITE_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      EMITE_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      EMITE_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      EMITE_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      EMITE_RX_STS_BREAK            Interrupt on break.
    *      EMITE_RX_STS_OVERRUN          Interrupt on overrun error.
    *      EMITE_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      EMITE_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void EMITE_SetRxInterruptMode(uint8 intSrc) 
    {
        EMITE_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: EMITE_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  EMITE_rxBuffer - RAM buffer pointer for save received data.
    *  EMITE_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  EMITE_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  EMITE_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 EMITE_ReadRxData(void) 
    {
        uint8 rxData;

    #if (EMITE_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        EMITE_DisableRxInt();

        locRxBufferRead  = EMITE_rxBufferRead;
        locRxBufferWrite = EMITE_rxBufferWrite;

        if( (EMITE_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = EMITE_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= EMITE_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            EMITE_rxBufferRead = locRxBufferRead;

            if(EMITE_rxBufferLoopDetect != 0u)
            {
                EMITE_rxBufferLoopDetect = 0u;
                #if ((EMITE_RX_INTERRUPT_ENABLED) && (EMITE_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( EMITE_HD_ENABLED )
                        if((EMITE_CONTROL_REG & EMITE_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            EMITE_RXSTATUS_MASK_REG  |= EMITE_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        EMITE_RXSTATUS_MASK_REG  |= EMITE_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end EMITE_HD_ENABLED */
                #endif /* ((EMITE_RX_INTERRUPT_ENABLED) && (EMITE_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = EMITE_RXDATA_REG;
        }

        EMITE_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = EMITE_RXDATA_REG;

    #endif /* (EMITE_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: EMITE_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  EMITE_RX_STS_FIFO_NOTEMPTY.
    *  EMITE_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  EMITE_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   EMITE_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   EMITE_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 EMITE_ReadRxStatus(void) 
    {
        uint8 status;

        status = EMITE_RXSTATUS_REG & EMITE_RX_HW_MASK;

    #if (EMITE_RX_INTERRUPT_ENABLED)
        if(EMITE_rxBufferOverflow != 0u)
        {
            status |= EMITE_RX_STS_SOFT_BUFF_OVER;
            EMITE_rxBufferOverflow = 0u;
        }
    #endif /* (EMITE_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: EMITE_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. EMITE_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  EMITE_rxBuffer - RAM buffer pointer for save received data.
    *  EMITE_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  EMITE_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  EMITE_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 EMITE_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (EMITE_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        EMITE_DisableRxInt();

        locRxBufferRead  = EMITE_rxBufferRead;
        locRxBufferWrite = EMITE_rxBufferWrite;

        if( (EMITE_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = EMITE_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= EMITE_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            EMITE_rxBufferRead = locRxBufferRead;

            if(EMITE_rxBufferLoopDetect != 0u)
            {
                EMITE_rxBufferLoopDetect = 0u;
                #if( (EMITE_RX_INTERRUPT_ENABLED) && (EMITE_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( EMITE_HD_ENABLED )
                        if((EMITE_CONTROL_REG & EMITE_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            EMITE_RXSTATUS_MASK_REG |= EMITE_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        EMITE_RXSTATUS_MASK_REG |= EMITE_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end EMITE_HD_ENABLED */
                #endif /* EMITE_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = EMITE_RXSTATUS_REG;
            if((rxStatus & EMITE_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = EMITE_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (EMITE_RX_STS_BREAK | EMITE_RX_STS_PAR_ERROR |
                                EMITE_RX_STS_STOP_ERROR | EMITE_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        EMITE_EnableRxInt();

    #else

        rxStatus =EMITE_RXSTATUS_REG;
        if((rxStatus & EMITE_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = EMITE_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (EMITE_RX_STS_BREAK | EMITE_RX_STS_PAR_ERROR |
                            EMITE_RX_STS_STOP_ERROR | EMITE_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (EMITE_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: EMITE_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 EMITE_GetByte(void) 
    {
        
    #if (EMITE_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        EMITE_DisableRxInt();
        locErrorStatus = (uint16)EMITE_errorStatus;
        EMITE_errorStatus = 0u;
        EMITE_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | EMITE_ReadRxData() );
    #else
        return ( ((uint16)EMITE_ReadRxStatus() << 8u) | EMITE_ReadRxData() );
    #endif /* EMITE_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: EMITE_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  EMITE_rxBufferWrite - used to calculate left bytes.
    *  EMITE_rxBufferRead - used to calculate left bytes.
    *  EMITE_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 EMITE_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (EMITE_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        EMITE_DisableRxInt();

        if(EMITE_rxBufferRead == EMITE_rxBufferWrite)
        {
            if(EMITE_rxBufferLoopDetect != 0u)
            {
                size = EMITE_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(EMITE_rxBufferRead < EMITE_rxBufferWrite)
        {
            size = (EMITE_rxBufferWrite - EMITE_rxBufferRead);
        }
        else
        {
            size = (EMITE_RX_BUFFER_SIZE - EMITE_rxBufferRead) + EMITE_rxBufferWrite;
        }

        EMITE_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((EMITE_RXSTATUS_REG & EMITE_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (EMITE_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: EMITE_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_rxBufferWrite - cleared to zero.
    *  EMITE_rxBufferRead - cleared to zero.
    *  EMITE_rxBufferLoopDetect - cleared to zero.
    *  EMITE_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void EMITE_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        EMITE_RXDATA_AUX_CTL_REG |= (uint8)  EMITE_RX_FIFO_CLR;
        EMITE_RXDATA_AUX_CTL_REG &= (uint8) ~EMITE_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (EMITE_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        EMITE_DisableRxInt();

        EMITE_rxBufferRead = 0u;
        EMITE_rxBufferWrite = 0u;
        EMITE_rxBufferLoopDetect = 0u;
        EMITE_rxBufferOverflow = 0u;

        EMITE_EnableRxInt();

    #endif /* (EMITE_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: EMITE_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  EMITE__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  EMITE__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  EMITE__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  EMITE__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  EMITE__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  EMITE_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void EMITE_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(EMITE_RXHW_ADDRESS_ENABLED)
            #if(EMITE_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* EMITE_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = EMITE_CONTROL_REG & (uint8)~EMITE_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << EMITE_CTRL_RXADDR_MODE0_SHIFT);
                EMITE_CONTROL_REG = tmpCtrl;

                #if(EMITE_RX_INTERRUPT_ENABLED && \
                   (EMITE_RXBUFFERSIZE > EMITE_FIFO_LENGTH) )
                    EMITE_rxAddressMode = addressMode;
                    EMITE_rxAddressDetected = 0u;
                #endif /* End EMITE_RXBUFFERSIZE > EMITE_FIFO_LENGTH*/
            #endif /* End EMITE_CONTROL_REG_REMOVED */
        #else /* EMITE_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End EMITE_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: EMITE_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void EMITE_SetRxAddress1(uint8 address) 
    {
        EMITE_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: EMITE_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void EMITE_SetRxAddress2(uint8 address) 
    {
        EMITE_RXADDRESS2_REG = address;
    }

#endif  /* EMITE_RX_ENABLED || EMITE_HD_ENABLED*/


#if( (EMITE_TX_ENABLED) || (EMITE_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: EMITE_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   EMITE_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   EMITE_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   EMITE_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   EMITE_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void EMITE_SetTxInterruptMode(uint8 intSrc) 
    {
        EMITE_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: EMITE_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  EMITE_txBuffer - RAM buffer pointer for save data for transmission
    *  EMITE_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  EMITE_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  EMITE_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void EMITE_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(EMITE_initVar != 0u)
        {
        #if (EMITE_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            EMITE_DisableTxInt();

            if( (EMITE_txBufferRead == EMITE_txBufferWrite) &&
                ((EMITE_TXSTATUS_REG & EMITE_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                EMITE_TXDATA_REG = txDataByte;
            }
            else
            {
                if(EMITE_txBufferWrite >= EMITE_TX_BUFFER_SIZE)
                {
                    EMITE_txBufferWrite = 0u;
                }

                EMITE_txBuffer[EMITE_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                EMITE_txBufferWrite++;
            }

            EMITE_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            EMITE_TXDATA_REG = txDataByte;

        #endif /*(EMITE_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: EMITE_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 EMITE_ReadTxStatus(void) 
    {
        return(EMITE_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: EMITE_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_txBuffer - RAM buffer pointer for save data for transmission
    *  EMITE_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  EMITE_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  EMITE_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void EMITE_PutChar(uint8 txDataByte) 
    {
    #if (EMITE_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((EMITE_TX_BUFFER_SIZE > EMITE_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            EMITE_DisableTxInt();
        #endif /* (EMITE_TX_BUFFER_SIZE > EMITE_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = EMITE_txBufferWrite;
            locTxBufferRead  = EMITE_txBufferRead;

        #if ((EMITE_TX_BUFFER_SIZE > EMITE_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            EMITE_EnableTxInt();
        #endif /* (EMITE_TX_BUFFER_SIZE > EMITE_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(EMITE_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((EMITE_TXSTATUS_REG & EMITE_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            EMITE_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= EMITE_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            EMITE_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((EMITE_TX_BUFFER_SIZE > EMITE_MAX_BYTE_VALUE) && (CY_PSOC3))
            EMITE_DisableTxInt();
        #endif /* (EMITE_TX_BUFFER_SIZE > EMITE_MAX_BYTE_VALUE) && (CY_PSOC3) */

            EMITE_txBufferWrite = locTxBufferWrite;

        #if ((EMITE_TX_BUFFER_SIZE > EMITE_MAX_BYTE_VALUE) && (CY_PSOC3))
            EMITE_EnableTxInt();
        #endif /* (EMITE_TX_BUFFER_SIZE > EMITE_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (EMITE_TXSTATUS_REG & EMITE_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                EMITE_SetPendingTxInt();
            }
        }

    #else

        while((EMITE_TXSTATUS_REG & EMITE_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        EMITE_TXDATA_REG = txDataByte;

    #endif /* EMITE_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: EMITE_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void EMITE_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(EMITE_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                EMITE_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: EMITE_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void EMITE_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(EMITE_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                EMITE_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: EMITE_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void EMITE_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(EMITE_initVar != 0u)
        {
            EMITE_PutChar(txDataByte);
            EMITE_PutChar(0x0Du);
            EMITE_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: EMITE_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  EMITE_txBufferWrite - used to calculate left space.
    *  EMITE_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 EMITE_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (EMITE_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        EMITE_DisableTxInt();

        if(EMITE_txBufferRead == EMITE_txBufferWrite)
        {
            size = 0u;
        }
        else if(EMITE_txBufferRead < EMITE_txBufferWrite)
        {
            size = (EMITE_txBufferWrite - EMITE_txBufferRead);
        }
        else
        {
            size = (EMITE_TX_BUFFER_SIZE - EMITE_txBufferRead) +
                    EMITE_txBufferWrite;
        }

        EMITE_EnableTxInt();

    #else

        size = EMITE_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & EMITE_TX_STS_FIFO_FULL) != 0u)
        {
            size = EMITE_FIFO_LENGTH;
        }
        else if((size & EMITE_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (EMITE_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: EMITE_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_txBufferWrite - cleared to zero.
    *  EMITE_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void EMITE_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        EMITE_TXDATA_AUX_CTL_REG |= (uint8)  EMITE_TX_FIFO_CLR;
        EMITE_TXDATA_AUX_CTL_REG &= (uint8) ~EMITE_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (EMITE_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        EMITE_DisableTxInt();

        EMITE_txBufferRead = 0u;
        EMITE_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        EMITE_EnableTxInt();

    #endif /* (EMITE_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: EMITE_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   EMITE_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   EMITE_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   EMITE_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   EMITE_SEND_WAIT_REINIT - Performs both options: 
    *      EMITE_SEND_BREAK and EMITE_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  EMITE_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with EMITE_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The EMITE_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void EMITE_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(EMITE_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(EMITE_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == EMITE_SEND_BREAK) ||
                (retMode == EMITE_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                EMITE_WriteControlRegister(EMITE_ReadControlRegister() |
                                                      EMITE_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                EMITE_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = EMITE_TXSTATUS_REG;
                }
                while((tmpStat & EMITE_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == EMITE_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == EMITE_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = EMITE_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & EMITE_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == EMITE_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == EMITE_REINIT) ||
                (retMode == EMITE_SEND_WAIT_REINIT) )
            {
                EMITE_WriteControlRegister(EMITE_ReadControlRegister() &
                                              (uint8)~EMITE_CTRL_HD_SEND_BREAK);
            }

        #else /* EMITE_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == EMITE_SEND_BREAK) ||
                (retMode == EMITE_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (EMITE_PARITY_TYPE != EMITE__B_UART__NONE_REVB) || \
                                    (EMITE_PARITY_TYPE_SW != 0u) )
                    EMITE_WriteControlRegister(EMITE_ReadControlRegister() |
                                                          EMITE_CTRL_HD_SEND_BREAK);
                #endif /* End EMITE_PARITY_TYPE != EMITE__B_UART__NONE_REVB  */

                #if(EMITE_TXCLKGEN_DP)
                    txPeriod = EMITE_TXBITCLKTX_COMPLETE_REG;
                    EMITE_TXBITCLKTX_COMPLETE_REG = EMITE_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = EMITE_TXBITCTR_PERIOD_REG;
                    EMITE_TXBITCTR_PERIOD_REG = EMITE_TXBITCTR_BREAKBITS8X;
                #endif /* End EMITE_TXCLKGEN_DP */

                /* Send zeros */
                EMITE_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = EMITE_TXSTATUS_REG;
                }
                while((tmpStat & EMITE_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == EMITE_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == EMITE_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = EMITE_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & EMITE_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == EMITE_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == EMITE_REINIT) ||
                (retMode == EMITE_SEND_WAIT_REINIT) )
            {

            #if(EMITE_TXCLKGEN_DP)
                EMITE_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                EMITE_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End EMITE_TXCLKGEN_DP */

            #if( (EMITE_PARITY_TYPE != EMITE__B_UART__NONE_REVB) || \
                 (EMITE_PARITY_TYPE_SW != 0u) )
                EMITE_WriteControlRegister(EMITE_ReadControlRegister() &
                                                      (uint8) ~EMITE_CTRL_HD_SEND_BREAK);
            #endif /* End EMITE_PARITY_TYPE != NONE */
            }
        #endif    /* End EMITE_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: EMITE_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       EMITE_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       EMITE_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears EMITE_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void EMITE_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( EMITE_CONTROL_REG_REMOVED == 0u )
            EMITE_WriteControlRegister(EMITE_ReadControlRegister() |
                                                  EMITE_CTRL_MARK);
        #endif /* End EMITE_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( EMITE_CONTROL_REG_REMOVED == 0u )
            EMITE_WriteControlRegister(EMITE_ReadControlRegister() &
                                                  (uint8) ~EMITE_CTRL_MARK);
        #endif /* End EMITE_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndEMITE_TX_ENABLED */

#if(EMITE_HD_ENABLED)


    /*******************************************************************************
    * Function Name: EMITE_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void EMITE_LoadRxConfig(void) 
    {
        EMITE_WriteControlRegister(EMITE_ReadControlRegister() &
                                                (uint8)~EMITE_CTRL_HD_SEND);
        EMITE_RXBITCTR_PERIOD_REG = EMITE_HD_RXBITCTR_INIT;

    #if (EMITE_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        EMITE_SetRxInterruptMode(EMITE_INIT_RX_INTERRUPTS_MASK);
    #endif /* (EMITE_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: EMITE_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void EMITE_LoadTxConfig(void) 
    {
    #if (EMITE_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        EMITE_SetRxInterruptMode(0u);
    #endif /* (EMITE_RX_INTERRUPT_ENABLED) */

        EMITE_WriteControlRegister(EMITE_ReadControlRegister() | EMITE_CTRL_HD_SEND);
        EMITE_RXBITCTR_PERIOD_REG = EMITE_HD_TXBITCTR_INIT;
    }

#endif  /* EMITE_HD_ENABLED */


/* [] END OF FILE */
