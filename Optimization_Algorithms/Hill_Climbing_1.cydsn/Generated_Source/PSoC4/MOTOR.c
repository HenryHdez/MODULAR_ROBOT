/*******************************************************************************
* File Name: MOTOR.c
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

#include "MOTOR.h"
#if (MOTOR_INTERNAL_CLOCK_USED)
    #include "MOTOR_IntClock.h"
#endif /* End MOTOR_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 MOTOR_initVar = 0u;

#if (MOTOR_TX_INTERRUPT_ENABLED && MOTOR_TX_ENABLED)
    volatile uint8 MOTOR_txBuffer[MOTOR_TX_BUFFER_SIZE];
    volatile uint8 MOTOR_txBufferRead = 0u;
    uint8 MOTOR_txBufferWrite = 0u;
#endif /* (MOTOR_TX_INTERRUPT_ENABLED && MOTOR_TX_ENABLED) */

#if (MOTOR_RX_INTERRUPT_ENABLED && (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED))
    uint8 MOTOR_errorStatus = 0u;
    volatile uint8 MOTOR_rxBuffer[MOTOR_RX_BUFFER_SIZE];
    volatile uint8 MOTOR_rxBufferRead  = 0u;
    volatile uint8 MOTOR_rxBufferWrite = 0u;
    volatile uint8 MOTOR_rxBufferLoopDetect = 0u;
    volatile uint8 MOTOR_rxBufferOverflow   = 0u;
    #if (MOTOR_RXHW_ADDRESS_ENABLED)
        volatile uint8 MOTOR_rxAddressMode = MOTOR_RX_ADDRESS_MODE;
        volatile uint8 MOTOR_rxAddressDetected = 0u;
    #endif /* (MOTOR_RXHW_ADDRESS_ENABLED) */
#endif /* (MOTOR_RX_INTERRUPT_ENABLED && (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)) */


/*******************************************************************************
* Function Name: MOTOR_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  MOTOR_Start() sets the initVar variable, calls the
*  MOTOR_Init() function, and then calls the
*  MOTOR_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The MOTOR_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time MOTOR_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the MOTOR_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MOTOR_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(MOTOR_initVar == 0u)
    {
        MOTOR_Init();
        MOTOR_initVar = 1u;
    }

    MOTOR_Enable();
}


/*******************************************************************************
* Function Name: MOTOR_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call MOTOR_Init() because
*  the MOTOR_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void MOTOR_Init(void) 
{
    #if(MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)

        #if (MOTOR_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(MOTOR_RX_VECT_NUM, &MOTOR_RXISR);
            CyIntSetPriority(MOTOR_RX_VECT_NUM, MOTOR_RX_PRIOR_NUM);
            MOTOR_errorStatus = 0u;
        #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */

        #if (MOTOR_RXHW_ADDRESS_ENABLED)
            MOTOR_SetRxAddressMode(MOTOR_RX_ADDRESS_MODE);
            MOTOR_SetRxAddress1(MOTOR_RX_HW_ADDRESS1);
            MOTOR_SetRxAddress2(MOTOR_RX_HW_ADDRESS2);
        #endif /* End MOTOR_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        MOTOR_RXBITCTR_PERIOD_REG = MOTOR_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        MOTOR_RXSTATUS_MASK_REG  = MOTOR_INIT_RX_INTERRUPTS_MASK;
    #endif /* End MOTOR_RX_ENABLED || MOTOR_HD_ENABLED*/

    #if(MOTOR_TX_ENABLED)
        #if (MOTOR_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(MOTOR_TX_VECT_NUM, &MOTOR_TXISR);
            CyIntSetPriority(MOTOR_TX_VECT_NUM, MOTOR_TX_PRIOR_NUM);
        #endif /* (MOTOR_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (MOTOR_TXCLKGEN_DP)
            MOTOR_TXBITCLKGEN_CTR_REG = MOTOR_BIT_CENTER;
            MOTOR_TXBITCLKTX_COMPLETE_REG = ((MOTOR_NUMBER_OF_DATA_BITS +
                        MOTOR_NUMBER_OF_START_BIT) * MOTOR_OVER_SAMPLE_COUNT) - 1u;
        #else
            MOTOR_TXBITCTR_PERIOD_REG = ((MOTOR_NUMBER_OF_DATA_BITS +
                        MOTOR_NUMBER_OF_START_BIT) * MOTOR_OVER_SAMPLE_8) - 1u;
        #endif /* End MOTOR_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (MOTOR_TX_INTERRUPT_ENABLED)
            MOTOR_TXSTATUS_MASK_REG = MOTOR_TX_STS_FIFO_EMPTY;
        #else
            MOTOR_TXSTATUS_MASK_REG = MOTOR_INIT_TX_INTERRUPTS_MASK;
        #endif /*End MOTOR_TX_INTERRUPT_ENABLED*/

    #endif /* End MOTOR_TX_ENABLED */

    #if(MOTOR_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        MOTOR_WriteControlRegister( \
            (MOTOR_ReadControlRegister() & (uint8)~MOTOR_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(MOTOR_PARITY_TYPE << MOTOR_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End MOTOR_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: MOTOR_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call MOTOR_Enable() because the MOTOR_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MOTOR_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void MOTOR_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        MOTOR_RXBITCTR_CONTROL_REG |= MOTOR_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        MOTOR_RXSTATUS_ACTL_REG  |= MOTOR_INT_ENABLE;

        #if (MOTOR_RX_INTERRUPT_ENABLED)
            MOTOR_EnableRxInt();

            #if (MOTOR_RXHW_ADDRESS_ENABLED)
                MOTOR_rxAddressDetected = 0u;
            #endif /* (MOTOR_RXHW_ADDRESS_ENABLED) */
        #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */
    #endif /* (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED) */

    #if(MOTOR_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!MOTOR_TXCLKGEN_DP)
            MOTOR_TXBITCTR_CONTROL_REG |= MOTOR_CNTR_ENABLE;
        #endif /* End MOTOR_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        MOTOR_TXSTATUS_ACTL_REG |= MOTOR_INT_ENABLE;
        #if (MOTOR_TX_INTERRUPT_ENABLED)
            MOTOR_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            MOTOR_EnableTxInt();
        #endif /* (MOTOR_TX_INTERRUPT_ENABLED) */
     #endif /* (MOTOR_TX_INTERRUPT_ENABLED) */

    #if (MOTOR_INTERNAL_CLOCK_USED)
        MOTOR_IntClock_Start();  /* Enable the clock */
    #endif /* (MOTOR_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MOTOR_Stop
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
void MOTOR_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)
        MOTOR_RXBITCTR_CONTROL_REG &= (uint8) ~MOTOR_CNTR_ENABLE;
    #endif /* (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED) */

    #if (MOTOR_TX_ENABLED)
        #if(!MOTOR_TXCLKGEN_DP)
            MOTOR_TXBITCTR_CONTROL_REG &= (uint8) ~MOTOR_CNTR_ENABLE;
        #endif /* (!MOTOR_TXCLKGEN_DP) */
    #endif /* (MOTOR_TX_ENABLED) */

    #if (MOTOR_INTERNAL_CLOCK_USED)
        MOTOR_IntClock_Stop();   /* Disable the clock */
    #endif /* (MOTOR_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)
        MOTOR_RXSTATUS_ACTL_REG  &= (uint8) ~MOTOR_INT_ENABLE;

        #if (MOTOR_RX_INTERRUPT_ENABLED)
            MOTOR_DisableRxInt();
        #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */
    #endif /* (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED) */

    #if (MOTOR_TX_ENABLED)
        MOTOR_TXSTATUS_ACTL_REG &= (uint8) ~MOTOR_INT_ENABLE;

        #if (MOTOR_TX_INTERRUPT_ENABLED)
            MOTOR_DisableTxInt();
        #endif /* (MOTOR_TX_INTERRUPT_ENABLED) */
    #endif /* (MOTOR_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: MOTOR_ReadControlRegister
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
uint8 MOTOR_ReadControlRegister(void) 
{
    #if (MOTOR_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(MOTOR_CONTROL_REG);
    #endif /* (MOTOR_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: MOTOR_WriteControlRegister
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
void  MOTOR_WriteControlRegister(uint8 control) 
{
    #if (MOTOR_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       MOTOR_CONTROL_REG = control;
    #endif /* (MOTOR_CONTROL_REG_REMOVED) */
}


#if(MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)
    /*******************************************************************************
    * Function Name: MOTOR_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      MOTOR_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      MOTOR_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      MOTOR_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      MOTOR_RX_STS_BREAK            Interrupt on break.
    *      MOTOR_RX_STS_OVERRUN          Interrupt on overrun error.
    *      MOTOR_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      MOTOR_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void MOTOR_SetRxInterruptMode(uint8 intSrc) 
    {
        MOTOR_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: MOTOR_ReadRxData
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
    *  MOTOR_rxBuffer - RAM buffer pointer for save received data.
    *  MOTOR_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  MOTOR_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  MOTOR_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 MOTOR_ReadRxData(void) 
    {
        uint8 rxData;

    #if (MOTOR_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        MOTOR_DisableRxInt();

        locRxBufferRead  = MOTOR_rxBufferRead;
        locRxBufferWrite = MOTOR_rxBufferWrite;

        if( (MOTOR_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = MOTOR_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= MOTOR_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            MOTOR_rxBufferRead = locRxBufferRead;

            if(MOTOR_rxBufferLoopDetect != 0u)
            {
                MOTOR_rxBufferLoopDetect = 0u;
                #if ((MOTOR_RX_INTERRUPT_ENABLED) && (MOTOR_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( MOTOR_HD_ENABLED )
                        if((MOTOR_CONTROL_REG & MOTOR_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            MOTOR_RXSTATUS_MASK_REG  |= MOTOR_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        MOTOR_RXSTATUS_MASK_REG  |= MOTOR_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end MOTOR_HD_ENABLED */
                #endif /* ((MOTOR_RX_INTERRUPT_ENABLED) && (MOTOR_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = MOTOR_RXDATA_REG;
        }

        MOTOR_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = MOTOR_RXDATA_REG;

    #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: MOTOR_ReadRxStatus
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
    *  MOTOR_RX_STS_FIFO_NOTEMPTY.
    *  MOTOR_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  MOTOR_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   MOTOR_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   MOTOR_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 MOTOR_ReadRxStatus(void) 
    {
        uint8 status;

        status = MOTOR_RXSTATUS_REG & MOTOR_RX_HW_MASK;

    #if (MOTOR_RX_INTERRUPT_ENABLED)
        if(MOTOR_rxBufferOverflow != 0u)
        {
            status |= MOTOR_RX_STS_SOFT_BUFF_OVER;
            MOTOR_rxBufferOverflow = 0u;
        }
    #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: MOTOR_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. MOTOR_GetChar() is
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
    *  MOTOR_rxBuffer - RAM buffer pointer for save received data.
    *  MOTOR_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  MOTOR_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  MOTOR_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 MOTOR_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (MOTOR_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        MOTOR_DisableRxInt();

        locRxBufferRead  = MOTOR_rxBufferRead;
        locRxBufferWrite = MOTOR_rxBufferWrite;

        if( (MOTOR_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = MOTOR_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= MOTOR_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            MOTOR_rxBufferRead = locRxBufferRead;

            if(MOTOR_rxBufferLoopDetect != 0u)
            {
                MOTOR_rxBufferLoopDetect = 0u;
                #if( (MOTOR_RX_INTERRUPT_ENABLED) && (MOTOR_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( MOTOR_HD_ENABLED )
                        if((MOTOR_CONTROL_REG & MOTOR_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            MOTOR_RXSTATUS_MASK_REG |= MOTOR_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        MOTOR_RXSTATUS_MASK_REG |= MOTOR_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end MOTOR_HD_ENABLED */
                #endif /* MOTOR_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = MOTOR_RXSTATUS_REG;
            if((rxStatus & MOTOR_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = MOTOR_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (MOTOR_RX_STS_BREAK | MOTOR_RX_STS_PAR_ERROR |
                                MOTOR_RX_STS_STOP_ERROR | MOTOR_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        MOTOR_EnableRxInt();

    #else

        rxStatus =MOTOR_RXSTATUS_REG;
        if((rxStatus & MOTOR_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = MOTOR_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (MOTOR_RX_STS_BREAK | MOTOR_RX_STS_PAR_ERROR |
                            MOTOR_RX_STS_STOP_ERROR | MOTOR_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: MOTOR_GetByte
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
    uint16 MOTOR_GetByte(void) 
    {
        
    #if (MOTOR_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        MOTOR_DisableRxInt();
        locErrorStatus = (uint16)MOTOR_errorStatus;
        MOTOR_errorStatus = 0u;
        MOTOR_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | MOTOR_ReadRxData() );
    #else
        return ( ((uint16)MOTOR_ReadRxStatus() << 8u) | MOTOR_ReadRxData() );
    #endif /* MOTOR_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: MOTOR_GetRxBufferSize
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
    *  MOTOR_rxBufferWrite - used to calculate left bytes.
    *  MOTOR_rxBufferRead - used to calculate left bytes.
    *  MOTOR_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 MOTOR_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (MOTOR_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        MOTOR_DisableRxInt();

        if(MOTOR_rxBufferRead == MOTOR_rxBufferWrite)
        {
            if(MOTOR_rxBufferLoopDetect != 0u)
            {
                size = MOTOR_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(MOTOR_rxBufferRead < MOTOR_rxBufferWrite)
        {
            size = (MOTOR_rxBufferWrite - MOTOR_rxBufferRead);
        }
        else
        {
            size = (MOTOR_RX_BUFFER_SIZE - MOTOR_rxBufferRead) + MOTOR_rxBufferWrite;
        }

        MOTOR_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((MOTOR_RXSTATUS_REG & MOTOR_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: MOTOR_ClearRxBuffer
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
    *  MOTOR_rxBufferWrite - cleared to zero.
    *  MOTOR_rxBufferRead - cleared to zero.
    *  MOTOR_rxBufferLoopDetect - cleared to zero.
    *  MOTOR_rxBufferOverflow - cleared to zero.
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
    void MOTOR_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        MOTOR_RXDATA_AUX_CTL_REG |= (uint8)  MOTOR_RX_FIFO_CLR;
        MOTOR_RXDATA_AUX_CTL_REG &= (uint8) ~MOTOR_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (MOTOR_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        MOTOR_DisableRxInt();

        MOTOR_rxBufferRead = 0u;
        MOTOR_rxBufferWrite = 0u;
        MOTOR_rxBufferLoopDetect = 0u;
        MOTOR_rxBufferOverflow = 0u;

        MOTOR_EnableRxInt();

    #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: MOTOR_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  MOTOR__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  MOTOR__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  MOTOR__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  MOTOR__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  MOTOR__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  MOTOR_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  MOTOR_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void MOTOR_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(MOTOR_RXHW_ADDRESS_ENABLED)
            #if(MOTOR_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* MOTOR_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = MOTOR_CONTROL_REG & (uint8)~MOTOR_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << MOTOR_CTRL_RXADDR_MODE0_SHIFT);
                MOTOR_CONTROL_REG = tmpCtrl;

                #if(MOTOR_RX_INTERRUPT_ENABLED && \
                   (MOTOR_RXBUFFERSIZE > MOTOR_FIFO_LENGTH) )
                    MOTOR_rxAddressMode = addressMode;
                    MOTOR_rxAddressDetected = 0u;
                #endif /* End MOTOR_RXBUFFERSIZE > MOTOR_FIFO_LENGTH*/
            #endif /* End MOTOR_CONTROL_REG_REMOVED */
        #else /* MOTOR_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End MOTOR_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: MOTOR_SetRxAddress1
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
    void MOTOR_SetRxAddress1(uint8 address) 
    {
        MOTOR_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: MOTOR_SetRxAddress2
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
    void MOTOR_SetRxAddress2(uint8 address) 
    {
        MOTOR_RXADDRESS2_REG = address;
    }

#endif  /* MOTOR_RX_ENABLED || MOTOR_HD_ENABLED*/


#if( (MOTOR_TX_ENABLED) || (MOTOR_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: MOTOR_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   MOTOR_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   MOTOR_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   MOTOR_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   MOTOR_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void MOTOR_SetTxInterruptMode(uint8 intSrc) 
    {
        MOTOR_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: MOTOR_WriteTxData
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
    *  MOTOR_txBuffer - RAM buffer pointer for save data for transmission
    *  MOTOR_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  MOTOR_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  MOTOR_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void MOTOR_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(MOTOR_initVar != 0u)
        {
        #if (MOTOR_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            MOTOR_DisableTxInt();

            if( (MOTOR_txBufferRead == MOTOR_txBufferWrite) &&
                ((MOTOR_TXSTATUS_REG & MOTOR_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                MOTOR_TXDATA_REG = txDataByte;
            }
            else
            {
                if(MOTOR_txBufferWrite >= MOTOR_TX_BUFFER_SIZE)
                {
                    MOTOR_txBufferWrite = 0u;
                }

                MOTOR_txBuffer[MOTOR_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                MOTOR_txBufferWrite++;
            }

            MOTOR_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            MOTOR_TXDATA_REG = txDataByte;

        #endif /*(MOTOR_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: MOTOR_ReadTxStatus
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
    uint8 MOTOR_ReadTxStatus(void) 
    {
        return(MOTOR_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: MOTOR_PutChar
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
    *  MOTOR_txBuffer - RAM buffer pointer for save data for transmission
    *  MOTOR_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  MOTOR_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  MOTOR_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void MOTOR_PutChar(uint8 txDataByte) 
    {
    #if (MOTOR_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((MOTOR_TX_BUFFER_SIZE > MOTOR_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            MOTOR_DisableTxInt();
        #endif /* (MOTOR_TX_BUFFER_SIZE > MOTOR_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = MOTOR_txBufferWrite;
            locTxBufferRead  = MOTOR_txBufferRead;

        #if ((MOTOR_TX_BUFFER_SIZE > MOTOR_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            MOTOR_EnableTxInt();
        #endif /* (MOTOR_TX_BUFFER_SIZE > MOTOR_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(MOTOR_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((MOTOR_TXSTATUS_REG & MOTOR_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            MOTOR_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= MOTOR_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            MOTOR_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((MOTOR_TX_BUFFER_SIZE > MOTOR_MAX_BYTE_VALUE) && (CY_PSOC3))
            MOTOR_DisableTxInt();
        #endif /* (MOTOR_TX_BUFFER_SIZE > MOTOR_MAX_BYTE_VALUE) && (CY_PSOC3) */

            MOTOR_txBufferWrite = locTxBufferWrite;

        #if ((MOTOR_TX_BUFFER_SIZE > MOTOR_MAX_BYTE_VALUE) && (CY_PSOC3))
            MOTOR_EnableTxInt();
        #endif /* (MOTOR_TX_BUFFER_SIZE > MOTOR_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (MOTOR_TXSTATUS_REG & MOTOR_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                MOTOR_SetPendingTxInt();
            }
        }

    #else

        while((MOTOR_TXSTATUS_REG & MOTOR_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        MOTOR_TXDATA_REG = txDataByte;

    #endif /* MOTOR_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: MOTOR_PutString
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
    *  MOTOR_initVar - checked to identify that the component has been
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
    void MOTOR_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(MOTOR_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                MOTOR_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: MOTOR_PutArray
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
    *  MOTOR_initVar - checked to identify that the component has been
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
    void MOTOR_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(MOTOR_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                MOTOR_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: MOTOR_PutCRLF
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
    *  MOTOR_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void MOTOR_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(MOTOR_initVar != 0u)
        {
            MOTOR_PutChar(txDataByte);
            MOTOR_PutChar(0x0Du);
            MOTOR_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: MOTOR_GetTxBufferSize
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
    *  MOTOR_txBufferWrite - used to calculate left space.
    *  MOTOR_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 MOTOR_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (MOTOR_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        MOTOR_DisableTxInt();

        if(MOTOR_txBufferRead == MOTOR_txBufferWrite)
        {
            size = 0u;
        }
        else if(MOTOR_txBufferRead < MOTOR_txBufferWrite)
        {
            size = (MOTOR_txBufferWrite - MOTOR_txBufferRead);
        }
        else
        {
            size = (MOTOR_TX_BUFFER_SIZE - MOTOR_txBufferRead) +
                    MOTOR_txBufferWrite;
        }

        MOTOR_EnableTxInt();

    #else

        size = MOTOR_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & MOTOR_TX_STS_FIFO_FULL) != 0u)
        {
            size = MOTOR_FIFO_LENGTH;
        }
        else if((size & MOTOR_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (MOTOR_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: MOTOR_ClearTxBuffer
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
    *  MOTOR_txBufferWrite - cleared to zero.
    *  MOTOR_txBufferRead - cleared to zero.
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
    void MOTOR_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        MOTOR_TXDATA_AUX_CTL_REG |= (uint8)  MOTOR_TX_FIFO_CLR;
        MOTOR_TXDATA_AUX_CTL_REG &= (uint8) ~MOTOR_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (MOTOR_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        MOTOR_DisableTxInt();

        MOTOR_txBufferRead = 0u;
        MOTOR_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        MOTOR_EnableTxInt();

    #endif /* (MOTOR_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: MOTOR_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   MOTOR_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   MOTOR_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   MOTOR_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   MOTOR_SEND_WAIT_REINIT - Performs both options: 
    *      MOTOR_SEND_BREAK and MOTOR_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  MOTOR_initVar - checked to identify that the component has been
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
    *     When interrupt appear with MOTOR_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The MOTOR_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void MOTOR_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(MOTOR_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(MOTOR_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == MOTOR_SEND_BREAK) ||
                (retMode == MOTOR_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                MOTOR_WriteControlRegister(MOTOR_ReadControlRegister() |
                                                      MOTOR_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                MOTOR_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = MOTOR_TXSTATUS_REG;
                }
                while((tmpStat & MOTOR_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == MOTOR_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == MOTOR_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = MOTOR_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & MOTOR_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == MOTOR_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == MOTOR_REINIT) ||
                (retMode == MOTOR_SEND_WAIT_REINIT) )
            {
                MOTOR_WriteControlRegister(MOTOR_ReadControlRegister() &
                                              (uint8)~MOTOR_CTRL_HD_SEND_BREAK);
            }

        #else /* MOTOR_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == MOTOR_SEND_BREAK) ||
                (retMode == MOTOR_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (MOTOR_PARITY_TYPE != MOTOR__B_UART__NONE_REVB) || \
                                    (MOTOR_PARITY_TYPE_SW != 0u) )
                    MOTOR_WriteControlRegister(MOTOR_ReadControlRegister() |
                                                          MOTOR_CTRL_HD_SEND_BREAK);
                #endif /* End MOTOR_PARITY_TYPE != MOTOR__B_UART__NONE_REVB  */

                #if(MOTOR_TXCLKGEN_DP)
                    txPeriod = MOTOR_TXBITCLKTX_COMPLETE_REG;
                    MOTOR_TXBITCLKTX_COMPLETE_REG = MOTOR_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = MOTOR_TXBITCTR_PERIOD_REG;
                    MOTOR_TXBITCTR_PERIOD_REG = MOTOR_TXBITCTR_BREAKBITS8X;
                #endif /* End MOTOR_TXCLKGEN_DP */

                /* Send zeros */
                MOTOR_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = MOTOR_TXSTATUS_REG;
                }
                while((tmpStat & MOTOR_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == MOTOR_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == MOTOR_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = MOTOR_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & MOTOR_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == MOTOR_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == MOTOR_REINIT) ||
                (retMode == MOTOR_SEND_WAIT_REINIT) )
            {

            #if(MOTOR_TXCLKGEN_DP)
                MOTOR_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                MOTOR_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End MOTOR_TXCLKGEN_DP */

            #if( (MOTOR_PARITY_TYPE != MOTOR__B_UART__NONE_REVB) || \
                 (MOTOR_PARITY_TYPE_SW != 0u) )
                MOTOR_WriteControlRegister(MOTOR_ReadControlRegister() &
                                                      (uint8) ~MOTOR_CTRL_HD_SEND_BREAK);
            #endif /* End MOTOR_PARITY_TYPE != NONE */
            }
        #endif    /* End MOTOR_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: MOTOR_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       MOTOR_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       MOTOR_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears MOTOR_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void MOTOR_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( MOTOR_CONTROL_REG_REMOVED == 0u )
            MOTOR_WriteControlRegister(MOTOR_ReadControlRegister() |
                                                  MOTOR_CTRL_MARK);
        #endif /* End MOTOR_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( MOTOR_CONTROL_REG_REMOVED == 0u )
            MOTOR_WriteControlRegister(MOTOR_ReadControlRegister() &
                                                  (uint8) ~MOTOR_CTRL_MARK);
        #endif /* End MOTOR_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndMOTOR_TX_ENABLED */

#if(MOTOR_HD_ENABLED)


    /*******************************************************************************
    * Function Name: MOTOR_LoadRxConfig
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
    void MOTOR_LoadRxConfig(void) 
    {
        MOTOR_WriteControlRegister(MOTOR_ReadControlRegister() &
                                                (uint8)~MOTOR_CTRL_HD_SEND);
        MOTOR_RXBITCTR_PERIOD_REG = MOTOR_HD_RXBITCTR_INIT;

    #if (MOTOR_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        MOTOR_SetRxInterruptMode(MOTOR_INIT_RX_INTERRUPTS_MASK);
    #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: MOTOR_LoadTxConfig
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
    void MOTOR_LoadTxConfig(void) 
    {
    #if (MOTOR_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        MOTOR_SetRxInterruptMode(0u);
    #endif /* (MOTOR_RX_INTERRUPT_ENABLED) */

        MOTOR_WriteControlRegister(MOTOR_ReadControlRegister() | MOTOR_CTRL_HD_SEND);
        MOTOR_RXBITCTR_PERIOD_REG = MOTOR_HD_TXBITCTR_INIT;
    }

#endif  /* MOTOR_HD_ENABLED */


/* [] END OF FILE */
