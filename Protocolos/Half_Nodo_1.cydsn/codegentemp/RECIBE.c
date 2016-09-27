/*******************************************************************************
* File Name: RECIBE.c
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

#include "RECIBE.h"
#if (RECIBE_INTERNAL_CLOCK_USED)
    #include "RECIBE_IntClock.h"
#endif /* End RECIBE_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 RECIBE_initVar = 0u;

#if (RECIBE_TX_INTERRUPT_ENABLED && RECIBE_TX_ENABLED)
    volatile uint8 RECIBE_txBuffer[RECIBE_TX_BUFFER_SIZE];
    volatile uint8 RECIBE_txBufferRead = 0u;
    uint8 RECIBE_txBufferWrite = 0u;
#endif /* (RECIBE_TX_INTERRUPT_ENABLED && RECIBE_TX_ENABLED) */

#if (RECIBE_RX_INTERRUPT_ENABLED && (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED))
    uint8 RECIBE_errorStatus = 0u;
    volatile uint8 RECIBE_rxBuffer[RECIBE_RX_BUFFER_SIZE];
    volatile uint8 RECIBE_rxBufferRead  = 0u;
    volatile uint8 RECIBE_rxBufferWrite = 0u;
    volatile uint8 RECIBE_rxBufferLoopDetect = 0u;
    volatile uint8 RECIBE_rxBufferOverflow   = 0u;
    #if (RECIBE_RXHW_ADDRESS_ENABLED)
        volatile uint8 RECIBE_rxAddressMode = RECIBE_RX_ADDRESS_MODE;
        volatile uint8 RECIBE_rxAddressDetected = 0u;
    #endif /* (RECIBE_RXHW_ADDRESS_ENABLED) */
#endif /* (RECIBE_RX_INTERRUPT_ENABLED && (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)) */


/*******************************************************************************
* Function Name: RECIBE_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  RECIBE_Start() sets the initVar variable, calls the
*  RECIBE_Init() function, and then calls the
*  RECIBE_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The RECIBE_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time RECIBE_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the RECIBE_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RECIBE_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(RECIBE_initVar == 0u)
    {
        RECIBE_Init();
        RECIBE_initVar = 1u;
    }

    RECIBE_Enable();
}


/*******************************************************************************
* Function Name: RECIBE_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call RECIBE_Init() because
*  the RECIBE_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void RECIBE_Init(void) 
{
    #if(RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)

        #if (RECIBE_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(RECIBE_RX_VECT_NUM, &RECIBE_RXISR);
            CyIntSetPriority(RECIBE_RX_VECT_NUM, RECIBE_RX_PRIOR_NUM);
            RECIBE_errorStatus = 0u;
        #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */

        #if (RECIBE_RXHW_ADDRESS_ENABLED)
            RECIBE_SetRxAddressMode(RECIBE_RX_ADDRESS_MODE);
            RECIBE_SetRxAddress1(RECIBE_RX_HW_ADDRESS1);
            RECIBE_SetRxAddress2(RECIBE_RX_HW_ADDRESS2);
        #endif /* End RECIBE_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        RECIBE_RXBITCTR_PERIOD_REG = RECIBE_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        RECIBE_RXSTATUS_MASK_REG  = RECIBE_INIT_RX_INTERRUPTS_MASK;
    #endif /* End RECIBE_RX_ENABLED || RECIBE_HD_ENABLED*/

    #if(RECIBE_TX_ENABLED)
        #if (RECIBE_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(RECIBE_TX_VECT_NUM, &RECIBE_TXISR);
            CyIntSetPriority(RECIBE_TX_VECT_NUM, RECIBE_TX_PRIOR_NUM);
        #endif /* (RECIBE_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (RECIBE_TXCLKGEN_DP)
            RECIBE_TXBITCLKGEN_CTR_REG = RECIBE_BIT_CENTER;
            RECIBE_TXBITCLKTX_COMPLETE_REG = ((RECIBE_NUMBER_OF_DATA_BITS +
                        RECIBE_NUMBER_OF_START_BIT) * RECIBE_OVER_SAMPLE_COUNT) - 1u;
        #else
            RECIBE_TXBITCTR_PERIOD_REG = ((RECIBE_NUMBER_OF_DATA_BITS +
                        RECIBE_NUMBER_OF_START_BIT) * RECIBE_OVER_SAMPLE_8) - 1u;
        #endif /* End RECIBE_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (RECIBE_TX_INTERRUPT_ENABLED)
            RECIBE_TXSTATUS_MASK_REG = RECIBE_TX_STS_FIFO_EMPTY;
        #else
            RECIBE_TXSTATUS_MASK_REG = RECIBE_INIT_TX_INTERRUPTS_MASK;
        #endif /*End RECIBE_TX_INTERRUPT_ENABLED*/

    #endif /* End RECIBE_TX_ENABLED */

    #if(RECIBE_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        RECIBE_WriteControlRegister( \
            (RECIBE_ReadControlRegister() & (uint8)~RECIBE_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(RECIBE_PARITY_TYPE << RECIBE_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End RECIBE_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: RECIBE_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call RECIBE_Enable() because the RECIBE_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RECIBE_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void RECIBE_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        RECIBE_RXBITCTR_CONTROL_REG |= RECIBE_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        RECIBE_RXSTATUS_ACTL_REG  |= RECIBE_INT_ENABLE;

        #if (RECIBE_RX_INTERRUPT_ENABLED)
            RECIBE_EnableRxInt();

            #if (RECIBE_RXHW_ADDRESS_ENABLED)
                RECIBE_rxAddressDetected = 0u;
            #endif /* (RECIBE_RXHW_ADDRESS_ENABLED) */
        #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */
    #endif /* (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED) */

    #if(RECIBE_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!RECIBE_TXCLKGEN_DP)
            RECIBE_TXBITCTR_CONTROL_REG |= RECIBE_CNTR_ENABLE;
        #endif /* End RECIBE_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        RECIBE_TXSTATUS_ACTL_REG |= RECIBE_INT_ENABLE;
        #if (RECIBE_TX_INTERRUPT_ENABLED)
            RECIBE_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            RECIBE_EnableTxInt();
        #endif /* (RECIBE_TX_INTERRUPT_ENABLED) */
     #endif /* (RECIBE_TX_INTERRUPT_ENABLED) */

    #if (RECIBE_INTERNAL_CLOCK_USED)
        RECIBE_IntClock_Start();  /* Enable the clock */
    #endif /* (RECIBE_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RECIBE_Stop
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
void RECIBE_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)
        RECIBE_RXBITCTR_CONTROL_REG &= (uint8) ~RECIBE_CNTR_ENABLE;
    #endif /* (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED) */

    #if (RECIBE_TX_ENABLED)
        #if(!RECIBE_TXCLKGEN_DP)
            RECIBE_TXBITCTR_CONTROL_REG &= (uint8) ~RECIBE_CNTR_ENABLE;
        #endif /* (!RECIBE_TXCLKGEN_DP) */
    #endif /* (RECIBE_TX_ENABLED) */

    #if (RECIBE_INTERNAL_CLOCK_USED)
        RECIBE_IntClock_Stop();   /* Disable the clock */
    #endif /* (RECIBE_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)
        RECIBE_RXSTATUS_ACTL_REG  &= (uint8) ~RECIBE_INT_ENABLE;

        #if (RECIBE_RX_INTERRUPT_ENABLED)
            RECIBE_DisableRxInt();
        #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */
    #endif /* (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED) */

    #if (RECIBE_TX_ENABLED)
        RECIBE_TXSTATUS_ACTL_REG &= (uint8) ~RECIBE_INT_ENABLE;

        #if (RECIBE_TX_INTERRUPT_ENABLED)
            RECIBE_DisableTxInt();
        #endif /* (RECIBE_TX_INTERRUPT_ENABLED) */
    #endif /* (RECIBE_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RECIBE_ReadControlRegister
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
uint8 RECIBE_ReadControlRegister(void) 
{
    #if (RECIBE_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(RECIBE_CONTROL_REG);
    #endif /* (RECIBE_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: RECIBE_WriteControlRegister
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
void  RECIBE_WriteControlRegister(uint8 control) 
{
    #if (RECIBE_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       RECIBE_CONTROL_REG = control;
    #endif /* (RECIBE_CONTROL_REG_REMOVED) */
}


#if(RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)
    /*******************************************************************************
    * Function Name: RECIBE_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      RECIBE_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      RECIBE_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      RECIBE_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      RECIBE_RX_STS_BREAK            Interrupt on break.
    *      RECIBE_RX_STS_OVERRUN          Interrupt on overrun error.
    *      RECIBE_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      RECIBE_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void RECIBE_SetRxInterruptMode(uint8 intSrc) 
    {
        RECIBE_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: RECIBE_ReadRxData
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
    *  RECIBE_rxBuffer - RAM buffer pointer for save received data.
    *  RECIBE_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  RECIBE_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  RECIBE_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 RECIBE_ReadRxData(void) 
    {
        uint8 rxData;

    #if (RECIBE_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        RECIBE_DisableRxInt();

        locRxBufferRead  = RECIBE_rxBufferRead;
        locRxBufferWrite = RECIBE_rxBufferWrite;

        if( (RECIBE_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = RECIBE_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= RECIBE_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            RECIBE_rxBufferRead = locRxBufferRead;

            if(RECIBE_rxBufferLoopDetect != 0u)
            {
                RECIBE_rxBufferLoopDetect = 0u;
                #if ((RECIBE_RX_INTERRUPT_ENABLED) && (RECIBE_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( RECIBE_HD_ENABLED )
                        if((RECIBE_CONTROL_REG & RECIBE_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            RECIBE_RXSTATUS_MASK_REG  |= RECIBE_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        RECIBE_RXSTATUS_MASK_REG  |= RECIBE_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end RECIBE_HD_ENABLED */
                #endif /* ((RECIBE_RX_INTERRUPT_ENABLED) && (RECIBE_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = RECIBE_RXDATA_REG;
        }

        RECIBE_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = RECIBE_RXDATA_REG;

    #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: RECIBE_ReadRxStatus
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
    *  RECIBE_RX_STS_FIFO_NOTEMPTY.
    *  RECIBE_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  RECIBE_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   RECIBE_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   RECIBE_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 RECIBE_ReadRxStatus(void) 
    {
        uint8 status;

        status = RECIBE_RXSTATUS_REG & RECIBE_RX_HW_MASK;

    #if (RECIBE_RX_INTERRUPT_ENABLED)
        if(RECIBE_rxBufferOverflow != 0u)
        {
            status |= RECIBE_RX_STS_SOFT_BUFF_OVER;
            RECIBE_rxBufferOverflow = 0u;
        }
    #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: RECIBE_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. RECIBE_GetChar() is
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
    *  RECIBE_rxBuffer - RAM buffer pointer for save received data.
    *  RECIBE_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  RECIBE_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  RECIBE_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 RECIBE_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (RECIBE_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        RECIBE_DisableRxInt();

        locRxBufferRead  = RECIBE_rxBufferRead;
        locRxBufferWrite = RECIBE_rxBufferWrite;

        if( (RECIBE_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = RECIBE_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= RECIBE_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            RECIBE_rxBufferRead = locRxBufferRead;

            if(RECIBE_rxBufferLoopDetect != 0u)
            {
                RECIBE_rxBufferLoopDetect = 0u;
                #if( (RECIBE_RX_INTERRUPT_ENABLED) && (RECIBE_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( RECIBE_HD_ENABLED )
                        if((RECIBE_CONTROL_REG & RECIBE_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            RECIBE_RXSTATUS_MASK_REG |= RECIBE_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        RECIBE_RXSTATUS_MASK_REG |= RECIBE_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end RECIBE_HD_ENABLED */
                #endif /* RECIBE_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = RECIBE_RXSTATUS_REG;
            if((rxStatus & RECIBE_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = RECIBE_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (RECIBE_RX_STS_BREAK | RECIBE_RX_STS_PAR_ERROR |
                                RECIBE_RX_STS_STOP_ERROR | RECIBE_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        RECIBE_EnableRxInt();

    #else

        rxStatus =RECIBE_RXSTATUS_REG;
        if((rxStatus & RECIBE_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = RECIBE_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (RECIBE_RX_STS_BREAK | RECIBE_RX_STS_PAR_ERROR |
                            RECIBE_RX_STS_STOP_ERROR | RECIBE_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: RECIBE_GetByte
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
    uint16 RECIBE_GetByte(void) 
    {
        
    #if (RECIBE_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        RECIBE_DisableRxInt();
        locErrorStatus = (uint16)RECIBE_errorStatus;
        RECIBE_errorStatus = 0u;
        RECIBE_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | RECIBE_ReadRxData() );
    #else
        return ( ((uint16)RECIBE_ReadRxStatus() << 8u) | RECIBE_ReadRxData() );
    #endif /* RECIBE_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: RECIBE_GetRxBufferSize
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
    *  RECIBE_rxBufferWrite - used to calculate left bytes.
    *  RECIBE_rxBufferRead - used to calculate left bytes.
    *  RECIBE_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 RECIBE_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (RECIBE_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        RECIBE_DisableRxInt();

        if(RECIBE_rxBufferRead == RECIBE_rxBufferWrite)
        {
            if(RECIBE_rxBufferLoopDetect != 0u)
            {
                size = RECIBE_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(RECIBE_rxBufferRead < RECIBE_rxBufferWrite)
        {
            size = (RECIBE_rxBufferWrite - RECIBE_rxBufferRead);
        }
        else
        {
            size = (RECIBE_RX_BUFFER_SIZE - RECIBE_rxBufferRead) + RECIBE_rxBufferWrite;
        }

        RECIBE_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((RECIBE_RXSTATUS_REG & RECIBE_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: RECIBE_ClearRxBuffer
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
    *  RECIBE_rxBufferWrite - cleared to zero.
    *  RECIBE_rxBufferRead - cleared to zero.
    *  RECIBE_rxBufferLoopDetect - cleared to zero.
    *  RECIBE_rxBufferOverflow - cleared to zero.
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
    void RECIBE_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        RECIBE_RXDATA_AUX_CTL_REG |= (uint8)  RECIBE_RX_FIFO_CLR;
        RECIBE_RXDATA_AUX_CTL_REG &= (uint8) ~RECIBE_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (RECIBE_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        RECIBE_DisableRxInt();

        RECIBE_rxBufferRead = 0u;
        RECIBE_rxBufferWrite = 0u;
        RECIBE_rxBufferLoopDetect = 0u;
        RECIBE_rxBufferOverflow = 0u;

        RECIBE_EnableRxInt();

    #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: RECIBE_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  RECIBE__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  RECIBE__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  RECIBE__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  RECIBE__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  RECIBE__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  RECIBE_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  RECIBE_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void RECIBE_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(RECIBE_RXHW_ADDRESS_ENABLED)
            #if(RECIBE_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* RECIBE_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = RECIBE_CONTROL_REG & (uint8)~RECIBE_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << RECIBE_CTRL_RXADDR_MODE0_SHIFT);
                RECIBE_CONTROL_REG = tmpCtrl;

                #if(RECIBE_RX_INTERRUPT_ENABLED && \
                   (RECIBE_RXBUFFERSIZE > RECIBE_FIFO_LENGTH) )
                    RECIBE_rxAddressMode = addressMode;
                    RECIBE_rxAddressDetected = 0u;
                #endif /* End RECIBE_RXBUFFERSIZE > RECIBE_FIFO_LENGTH*/
            #endif /* End RECIBE_CONTROL_REG_REMOVED */
        #else /* RECIBE_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End RECIBE_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: RECIBE_SetRxAddress1
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
    void RECIBE_SetRxAddress1(uint8 address) 
    {
        RECIBE_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: RECIBE_SetRxAddress2
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
    void RECIBE_SetRxAddress2(uint8 address) 
    {
        RECIBE_RXADDRESS2_REG = address;
    }

#endif  /* RECIBE_RX_ENABLED || RECIBE_HD_ENABLED*/


#if( (RECIBE_TX_ENABLED) || (RECIBE_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: RECIBE_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   RECIBE_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   RECIBE_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   RECIBE_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   RECIBE_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void RECIBE_SetTxInterruptMode(uint8 intSrc) 
    {
        RECIBE_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: RECIBE_WriteTxData
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
    *  RECIBE_txBuffer - RAM buffer pointer for save data for transmission
    *  RECIBE_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  RECIBE_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  RECIBE_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RECIBE_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(RECIBE_initVar != 0u)
        {
        #if (RECIBE_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            RECIBE_DisableTxInt();

            if( (RECIBE_txBufferRead == RECIBE_txBufferWrite) &&
                ((RECIBE_TXSTATUS_REG & RECIBE_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                RECIBE_TXDATA_REG = txDataByte;
            }
            else
            {
                if(RECIBE_txBufferWrite >= RECIBE_TX_BUFFER_SIZE)
                {
                    RECIBE_txBufferWrite = 0u;
                }

                RECIBE_txBuffer[RECIBE_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                RECIBE_txBufferWrite++;
            }

            RECIBE_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            RECIBE_TXDATA_REG = txDataByte;

        #endif /*(RECIBE_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: RECIBE_ReadTxStatus
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
    uint8 RECIBE_ReadTxStatus(void) 
    {
        return(RECIBE_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: RECIBE_PutChar
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
    *  RECIBE_txBuffer - RAM buffer pointer for save data for transmission
    *  RECIBE_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  RECIBE_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  RECIBE_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void RECIBE_PutChar(uint8 txDataByte) 
    {
    #if (RECIBE_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((RECIBE_TX_BUFFER_SIZE > RECIBE_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            RECIBE_DisableTxInt();
        #endif /* (RECIBE_TX_BUFFER_SIZE > RECIBE_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = RECIBE_txBufferWrite;
            locTxBufferRead  = RECIBE_txBufferRead;

        #if ((RECIBE_TX_BUFFER_SIZE > RECIBE_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            RECIBE_EnableTxInt();
        #endif /* (RECIBE_TX_BUFFER_SIZE > RECIBE_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(RECIBE_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((RECIBE_TXSTATUS_REG & RECIBE_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            RECIBE_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= RECIBE_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            RECIBE_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((RECIBE_TX_BUFFER_SIZE > RECIBE_MAX_BYTE_VALUE) && (CY_PSOC3))
            RECIBE_DisableTxInt();
        #endif /* (RECIBE_TX_BUFFER_SIZE > RECIBE_MAX_BYTE_VALUE) && (CY_PSOC3) */

            RECIBE_txBufferWrite = locTxBufferWrite;

        #if ((RECIBE_TX_BUFFER_SIZE > RECIBE_MAX_BYTE_VALUE) && (CY_PSOC3))
            RECIBE_EnableTxInt();
        #endif /* (RECIBE_TX_BUFFER_SIZE > RECIBE_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (RECIBE_TXSTATUS_REG & RECIBE_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                RECIBE_SetPendingTxInt();
            }
        }

    #else

        while((RECIBE_TXSTATUS_REG & RECIBE_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        RECIBE_TXDATA_REG = txDataByte;

    #endif /* RECIBE_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: RECIBE_PutString
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
    *  RECIBE_initVar - checked to identify that the component has been
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
    void RECIBE_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(RECIBE_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                RECIBE_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: RECIBE_PutArray
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
    *  RECIBE_initVar - checked to identify that the component has been
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
    void RECIBE_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(RECIBE_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                RECIBE_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: RECIBE_PutCRLF
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
    *  RECIBE_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RECIBE_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(RECIBE_initVar != 0u)
        {
            RECIBE_PutChar(txDataByte);
            RECIBE_PutChar(0x0Du);
            RECIBE_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: RECIBE_GetTxBufferSize
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
    *  RECIBE_txBufferWrite - used to calculate left space.
    *  RECIBE_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 RECIBE_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (RECIBE_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        RECIBE_DisableTxInt();

        if(RECIBE_txBufferRead == RECIBE_txBufferWrite)
        {
            size = 0u;
        }
        else if(RECIBE_txBufferRead < RECIBE_txBufferWrite)
        {
            size = (RECIBE_txBufferWrite - RECIBE_txBufferRead);
        }
        else
        {
            size = (RECIBE_TX_BUFFER_SIZE - RECIBE_txBufferRead) +
                    RECIBE_txBufferWrite;
        }

        RECIBE_EnableTxInt();

    #else

        size = RECIBE_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & RECIBE_TX_STS_FIFO_FULL) != 0u)
        {
            size = RECIBE_FIFO_LENGTH;
        }
        else if((size & RECIBE_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (RECIBE_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: RECIBE_ClearTxBuffer
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
    *  RECIBE_txBufferWrite - cleared to zero.
    *  RECIBE_txBufferRead - cleared to zero.
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
    void RECIBE_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        RECIBE_TXDATA_AUX_CTL_REG |= (uint8)  RECIBE_TX_FIFO_CLR;
        RECIBE_TXDATA_AUX_CTL_REG &= (uint8) ~RECIBE_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (RECIBE_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        RECIBE_DisableTxInt();

        RECIBE_txBufferRead = 0u;
        RECIBE_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        RECIBE_EnableTxInt();

    #endif /* (RECIBE_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: RECIBE_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   RECIBE_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   RECIBE_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   RECIBE_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   RECIBE_SEND_WAIT_REINIT - Performs both options: 
    *      RECIBE_SEND_BREAK and RECIBE_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  RECIBE_initVar - checked to identify that the component has been
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
    *     When interrupt appear with RECIBE_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The RECIBE_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void RECIBE_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(RECIBE_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(RECIBE_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == RECIBE_SEND_BREAK) ||
                (retMode == RECIBE_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                RECIBE_WriteControlRegister(RECIBE_ReadControlRegister() |
                                                      RECIBE_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                RECIBE_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = RECIBE_TXSTATUS_REG;
                }
                while((tmpStat & RECIBE_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == RECIBE_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == RECIBE_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = RECIBE_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & RECIBE_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == RECIBE_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == RECIBE_REINIT) ||
                (retMode == RECIBE_SEND_WAIT_REINIT) )
            {
                RECIBE_WriteControlRegister(RECIBE_ReadControlRegister() &
                                              (uint8)~RECIBE_CTRL_HD_SEND_BREAK);
            }

        #else /* RECIBE_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == RECIBE_SEND_BREAK) ||
                (retMode == RECIBE_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (RECIBE_PARITY_TYPE != RECIBE__B_UART__NONE_REVB) || \
                                    (RECIBE_PARITY_TYPE_SW != 0u) )
                    RECIBE_WriteControlRegister(RECIBE_ReadControlRegister() |
                                                          RECIBE_CTRL_HD_SEND_BREAK);
                #endif /* End RECIBE_PARITY_TYPE != RECIBE__B_UART__NONE_REVB  */

                #if(RECIBE_TXCLKGEN_DP)
                    txPeriod = RECIBE_TXBITCLKTX_COMPLETE_REG;
                    RECIBE_TXBITCLKTX_COMPLETE_REG = RECIBE_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = RECIBE_TXBITCTR_PERIOD_REG;
                    RECIBE_TXBITCTR_PERIOD_REG = RECIBE_TXBITCTR_BREAKBITS8X;
                #endif /* End RECIBE_TXCLKGEN_DP */

                /* Send zeros */
                RECIBE_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = RECIBE_TXSTATUS_REG;
                }
                while((tmpStat & RECIBE_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == RECIBE_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == RECIBE_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = RECIBE_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & RECIBE_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == RECIBE_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == RECIBE_REINIT) ||
                (retMode == RECIBE_SEND_WAIT_REINIT) )
            {

            #if(RECIBE_TXCLKGEN_DP)
                RECIBE_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                RECIBE_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End RECIBE_TXCLKGEN_DP */

            #if( (RECIBE_PARITY_TYPE != RECIBE__B_UART__NONE_REVB) || \
                 (RECIBE_PARITY_TYPE_SW != 0u) )
                RECIBE_WriteControlRegister(RECIBE_ReadControlRegister() &
                                                      (uint8) ~RECIBE_CTRL_HD_SEND_BREAK);
            #endif /* End RECIBE_PARITY_TYPE != NONE */
            }
        #endif    /* End RECIBE_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: RECIBE_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       RECIBE_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       RECIBE_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears RECIBE_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void RECIBE_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( RECIBE_CONTROL_REG_REMOVED == 0u )
            RECIBE_WriteControlRegister(RECIBE_ReadControlRegister() |
                                                  RECIBE_CTRL_MARK);
        #endif /* End RECIBE_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( RECIBE_CONTROL_REG_REMOVED == 0u )
            RECIBE_WriteControlRegister(RECIBE_ReadControlRegister() &
                                                  (uint8) ~RECIBE_CTRL_MARK);
        #endif /* End RECIBE_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndRECIBE_TX_ENABLED */

#if(RECIBE_HD_ENABLED)


    /*******************************************************************************
    * Function Name: RECIBE_LoadRxConfig
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
    void RECIBE_LoadRxConfig(void) 
    {
        RECIBE_WriteControlRegister(RECIBE_ReadControlRegister() &
                                                (uint8)~RECIBE_CTRL_HD_SEND);
        RECIBE_RXBITCTR_PERIOD_REG = RECIBE_HD_RXBITCTR_INIT;

    #if (RECIBE_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        RECIBE_SetRxInterruptMode(RECIBE_INIT_RX_INTERRUPTS_MASK);
    #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: RECIBE_LoadTxConfig
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
    void RECIBE_LoadTxConfig(void) 
    {
    #if (RECIBE_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        RECIBE_SetRxInterruptMode(0u);
    #endif /* (RECIBE_RX_INTERRUPT_ENABLED) */

        RECIBE_WriteControlRegister(RECIBE_ReadControlRegister() | RECIBE_CTRL_HD_SEND);
        RECIBE_RXBITCTR_PERIOD_REG = RECIBE_HD_TXBITCTR_INIT;
    }

#endif  /* RECIBE_HD_ENABLED */


/* [] END OF FILE */
