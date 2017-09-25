/*******************************************************************************
* File Name: RX.c
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

#include "RX.h"
#if (RX_INTERNAL_CLOCK_USED)
    #include "RX_IntClock.h"
#endif /* End RX_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 RX_initVar = 0u;

#if (RX_TX_INTERRUPT_ENABLED && RX_TX_ENABLED)
    volatile uint8 RX_txBuffer[RX_TX_BUFFER_SIZE];
    volatile uint8 RX_txBufferRead = 0u;
    uint8 RX_txBufferWrite = 0u;
#endif /* (RX_TX_INTERRUPT_ENABLED && RX_TX_ENABLED) */

#if (RX_RX_INTERRUPT_ENABLED && (RX_RX_ENABLED || RX_HD_ENABLED))
    uint8 RX_errorStatus = 0u;
    volatile uint8 RX_rxBuffer[RX_RX_BUFFER_SIZE];
    volatile uint8 RX_rxBufferRead  = 0u;
    volatile uint8 RX_rxBufferWrite = 0u;
    volatile uint8 RX_rxBufferLoopDetect = 0u;
    volatile uint8 RX_rxBufferOverflow   = 0u;
    #if (RX_RXHW_ADDRESS_ENABLED)
        volatile uint8 RX_rxAddressMode = RX_RX_ADDRESS_MODE;
        volatile uint8 RX_rxAddressDetected = 0u;
    #endif /* (RX_RXHW_ADDRESS_ENABLED) */
#endif /* (RX_RX_INTERRUPT_ENABLED && (RX_RX_ENABLED || RX_HD_ENABLED)) */


/*******************************************************************************
* Function Name: RX_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  RX_Start() sets the initVar variable, calls the
*  RX_Init() function, and then calls the
*  RX_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The RX_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time RX_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the RX_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RX_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(RX_initVar == 0u)
    {
        RX_Init();
        RX_initVar = 1u;
    }

    RX_Enable();
}


/*******************************************************************************
* Function Name: RX_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call RX_Init() because
*  the RX_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void RX_Init(void) 
{
    #if(RX_RX_ENABLED || RX_HD_ENABLED)

        #if (RX_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(RX_RX_VECT_NUM, &RX_RXISR);
            CyIntSetPriority(RX_RX_VECT_NUM, RX_RX_PRIOR_NUM);
            RX_errorStatus = 0u;
        #endif /* (RX_RX_INTERRUPT_ENABLED) */

        #if (RX_RXHW_ADDRESS_ENABLED)
            RX_SetRxAddressMode(RX_RX_ADDRESS_MODE);
            RX_SetRxAddress1(RX_RX_HW_ADDRESS1);
            RX_SetRxAddress2(RX_RX_HW_ADDRESS2);
        #endif /* End RX_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        RX_RXBITCTR_PERIOD_REG = RX_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        RX_RXSTATUS_MASK_REG  = RX_INIT_RX_INTERRUPTS_MASK;
    #endif /* End RX_RX_ENABLED || RX_HD_ENABLED*/

    #if(RX_TX_ENABLED)
        #if (RX_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(RX_TX_VECT_NUM, &RX_TXISR);
            CyIntSetPriority(RX_TX_VECT_NUM, RX_TX_PRIOR_NUM);
        #endif /* (RX_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (RX_TXCLKGEN_DP)
            RX_TXBITCLKGEN_CTR_REG = RX_BIT_CENTER;
            RX_TXBITCLKTX_COMPLETE_REG = ((RX_NUMBER_OF_DATA_BITS +
                        RX_NUMBER_OF_START_BIT) * RX_OVER_SAMPLE_COUNT) - 1u;
        #else
            RX_TXBITCTR_PERIOD_REG = ((RX_NUMBER_OF_DATA_BITS +
                        RX_NUMBER_OF_START_BIT) * RX_OVER_SAMPLE_8) - 1u;
        #endif /* End RX_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (RX_TX_INTERRUPT_ENABLED)
            RX_TXSTATUS_MASK_REG = RX_TX_STS_FIFO_EMPTY;
        #else
            RX_TXSTATUS_MASK_REG = RX_INIT_TX_INTERRUPTS_MASK;
        #endif /*End RX_TX_INTERRUPT_ENABLED*/

    #endif /* End RX_TX_ENABLED */

    #if(RX_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        RX_WriteControlRegister( \
            (RX_ReadControlRegister() & (uint8)~RX_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(RX_PARITY_TYPE << RX_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End RX_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: RX_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call RX_Enable() because the RX_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RX_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void RX_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (RX_RX_ENABLED || RX_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        RX_RXBITCTR_CONTROL_REG |= RX_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        RX_RXSTATUS_ACTL_REG  |= RX_INT_ENABLE;

        #if (RX_RX_INTERRUPT_ENABLED)
            RX_EnableRxInt();

            #if (RX_RXHW_ADDRESS_ENABLED)
                RX_rxAddressDetected = 0u;
            #endif /* (RX_RXHW_ADDRESS_ENABLED) */
        #endif /* (RX_RX_INTERRUPT_ENABLED) */
    #endif /* (RX_RX_ENABLED || RX_HD_ENABLED) */

    #if(RX_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!RX_TXCLKGEN_DP)
            RX_TXBITCTR_CONTROL_REG |= RX_CNTR_ENABLE;
        #endif /* End RX_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        RX_TXSTATUS_ACTL_REG |= RX_INT_ENABLE;
        #if (RX_TX_INTERRUPT_ENABLED)
            RX_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            RX_EnableTxInt();
        #endif /* (RX_TX_INTERRUPT_ENABLED) */
     #endif /* (RX_TX_INTERRUPT_ENABLED) */

    #if (RX_INTERNAL_CLOCK_USED)
        RX_IntClock_Start();  /* Enable the clock */
    #endif /* (RX_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RX_Stop
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
void RX_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (RX_RX_ENABLED || RX_HD_ENABLED)
        RX_RXBITCTR_CONTROL_REG &= (uint8) ~RX_CNTR_ENABLE;
    #endif /* (RX_RX_ENABLED || RX_HD_ENABLED) */

    #if (RX_TX_ENABLED)
        #if(!RX_TXCLKGEN_DP)
            RX_TXBITCTR_CONTROL_REG &= (uint8) ~RX_CNTR_ENABLE;
        #endif /* (!RX_TXCLKGEN_DP) */
    #endif /* (RX_TX_ENABLED) */

    #if (RX_INTERNAL_CLOCK_USED)
        RX_IntClock_Stop();   /* Disable the clock */
    #endif /* (RX_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (RX_RX_ENABLED || RX_HD_ENABLED)
        RX_RXSTATUS_ACTL_REG  &= (uint8) ~RX_INT_ENABLE;

        #if (RX_RX_INTERRUPT_ENABLED)
            RX_DisableRxInt();
        #endif /* (RX_RX_INTERRUPT_ENABLED) */
    #endif /* (RX_RX_ENABLED || RX_HD_ENABLED) */

    #if (RX_TX_ENABLED)
        RX_TXSTATUS_ACTL_REG &= (uint8) ~RX_INT_ENABLE;

        #if (RX_TX_INTERRUPT_ENABLED)
            RX_DisableTxInt();
        #endif /* (RX_TX_INTERRUPT_ENABLED) */
    #endif /* (RX_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: RX_ReadControlRegister
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
uint8 RX_ReadControlRegister(void) 
{
    #if (RX_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(RX_CONTROL_REG);
    #endif /* (RX_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: RX_WriteControlRegister
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
void  RX_WriteControlRegister(uint8 control) 
{
    #if (RX_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       RX_CONTROL_REG = control;
    #endif /* (RX_CONTROL_REG_REMOVED) */
}


#if(RX_RX_ENABLED || RX_HD_ENABLED)
    /*******************************************************************************
    * Function Name: RX_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      RX_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      RX_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      RX_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      RX_RX_STS_BREAK            Interrupt on break.
    *      RX_RX_STS_OVERRUN          Interrupt on overrun error.
    *      RX_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      RX_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void RX_SetRxInterruptMode(uint8 intSrc) 
    {
        RX_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: RX_ReadRxData
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
    *  RX_rxBuffer - RAM buffer pointer for save received data.
    *  RX_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  RX_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  RX_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 RX_ReadRxData(void) 
    {
        uint8 rxData;

    #if (RX_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        RX_DisableRxInt();

        locRxBufferRead  = RX_rxBufferRead;
        locRxBufferWrite = RX_rxBufferWrite;

        if( (RX_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = RX_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= RX_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            RX_rxBufferRead = locRxBufferRead;

            if(RX_rxBufferLoopDetect != 0u)
            {
                RX_rxBufferLoopDetect = 0u;
                #if ((RX_RX_INTERRUPT_ENABLED) && (RX_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( RX_HD_ENABLED )
                        if((RX_CONTROL_REG & RX_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            RX_RXSTATUS_MASK_REG  |= RX_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        RX_RXSTATUS_MASK_REG  |= RX_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end RX_HD_ENABLED */
                #endif /* ((RX_RX_INTERRUPT_ENABLED) && (RX_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = RX_RXDATA_REG;
        }

        RX_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = RX_RXDATA_REG;

    #endif /* (RX_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: RX_ReadRxStatus
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
    *  RX_RX_STS_FIFO_NOTEMPTY.
    *  RX_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  RX_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   RX_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   RX_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 RX_ReadRxStatus(void) 
    {
        uint8 status;

        status = RX_RXSTATUS_REG & RX_RX_HW_MASK;

    #if (RX_RX_INTERRUPT_ENABLED)
        if(RX_rxBufferOverflow != 0u)
        {
            status |= RX_RX_STS_SOFT_BUFF_OVER;
            RX_rxBufferOverflow = 0u;
        }
    #endif /* (RX_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: RX_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. RX_GetChar() is
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
    *  RX_rxBuffer - RAM buffer pointer for save received data.
    *  RX_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  RX_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  RX_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 RX_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (RX_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        RX_DisableRxInt();

        locRxBufferRead  = RX_rxBufferRead;
        locRxBufferWrite = RX_rxBufferWrite;

        if( (RX_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = RX_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= RX_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            RX_rxBufferRead = locRxBufferRead;

            if(RX_rxBufferLoopDetect != 0u)
            {
                RX_rxBufferLoopDetect = 0u;
                #if( (RX_RX_INTERRUPT_ENABLED) && (RX_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( RX_HD_ENABLED )
                        if((RX_CONTROL_REG & RX_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            RX_RXSTATUS_MASK_REG |= RX_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        RX_RXSTATUS_MASK_REG |= RX_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end RX_HD_ENABLED */
                #endif /* RX_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = RX_RXSTATUS_REG;
            if((rxStatus & RX_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = RX_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (RX_RX_STS_BREAK | RX_RX_STS_PAR_ERROR |
                                RX_RX_STS_STOP_ERROR | RX_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        RX_EnableRxInt();

    #else

        rxStatus =RX_RXSTATUS_REG;
        if((rxStatus & RX_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = RX_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (RX_RX_STS_BREAK | RX_RX_STS_PAR_ERROR |
                            RX_RX_STS_STOP_ERROR | RX_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (RX_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: RX_GetByte
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
    uint16 RX_GetByte(void) 
    {
        
    #if (RX_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        RX_DisableRxInt();
        locErrorStatus = (uint16)RX_errorStatus;
        RX_errorStatus = 0u;
        RX_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | RX_ReadRxData() );
    #else
        return ( ((uint16)RX_ReadRxStatus() << 8u) | RX_ReadRxData() );
    #endif /* RX_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: RX_GetRxBufferSize
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
    *  RX_rxBufferWrite - used to calculate left bytes.
    *  RX_rxBufferRead - used to calculate left bytes.
    *  RX_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 RX_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (RX_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        RX_DisableRxInt();

        if(RX_rxBufferRead == RX_rxBufferWrite)
        {
            if(RX_rxBufferLoopDetect != 0u)
            {
                size = RX_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(RX_rxBufferRead < RX_rxBufferWrite)
        {
            size = (RX_rxBufferWrite - RX_rxBufferRead);
        }
        else
        {
            size = (RX_RX_BUFFER_SIZE - RX_rxBufferRead) + RX_rxBufferWrite;
        }

        RX_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((RX_RXSTATUS_REG & RX_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (RX_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: RX_ClearRxBuffer
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
    *  RX_rxBufferWrite - cleared to zero.
    *  RX_rxBufferRead - cleared to zero.
    *  RX_rxBufferLoopDetect - cleared to zero.
    *  RX_rxBufferOverflow - cleared to zero.
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
    void RX_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        RX_RXDATA_AUX_CTL_REG |= (uint8)  RX_RX_FIFO_CLR;
        RX_RXDATA_AUX_CTL_REG &= (uint8) ~RX_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (RX_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        RX_DisableRxInt();

        RX_rxBufferRead = 0u;
        RX_rxBufferWrite = 0u;
        RX_rxBufferLoopDetect = 0u;
        RX_rxBufferOverflow = 0u;

        RX_EnableRxInt();

    #endif /* (RX_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: RX_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  RX__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  RX__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  RX__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  RX__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  RX__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  RX_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  RX_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void RX_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(RX_RXHW_ADDRESS_ENABLED)
            #if(RX_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* RX_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = RX_CONTROL_REG & (uint8)~RX_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << RX_CTRL_RXADDR_MODE0_SHIFT);
                RX_CONTROL_REG = tmpCtrl;

                #if(RX_RX_INTERRUPT_ENABLED && \
                   (RX_RXBUFFERSIZE > RX_FIFO_LENGTH) )
                    RX_rxAddressMode = addressMode;
                    RX_rxAddressDetected = 0u;
                #endif /* End RX_RXBUFFERSIZE > RX_FIFO_LENGTH*/
            #endif /* End RX_CONTROL_REG_REMOVED */
        #else /* RX_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End RX_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: RX_SetRxAddress1
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
    void RX_SetRxAddress1(uint8 address) 
    {
        RX_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: RX_SetRxAddress2
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
    void RX_SetRxAddress2(uint8 address) 
    {
        RX_RXADDRESS2_REG = address;
    }

#endif  /* RX_RX_ENABLED || RX_HD_ENABLED*/


#if( (RX_TX_ENABLED) || (RX_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: RX_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   RX_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   RX_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   RX_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   RX_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void RX_SetTxInterruptMode(uint8 intSrc) 
    {
        RX_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: RX_WriteTxData
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
    *  RX_txBuffer - RAM buffer pointer for save data for transmission
    *  RX_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  RX_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  RX_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RX_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(RX_initVar != 0u)
        {
        #if (RX_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            RX_DisableTxInt();

            if( (RX_txBufferRead == RX_txBufferWrite) &&
                ((RX_TXSTATUS_REG & RX_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                RX_TXDATA_REG = txDataByte;
            }
            else
            {
                if(RX_txBufferWrite >= RX_TX_BUFFER_SIZE)
                {
                    RX_txBufferWrite = 0u;
                }

                RX_txBuffer[RX_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                RX_txBufferWrite++;
            }

            RX_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            RX_TXDATA_REG = txDataByte;

        #endif /*(RX_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: RX_ReadTxStatus
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
    uint8 RX_ReadTxStatus(void) 
    {
        return(RX_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: RX_PutChar
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
    *  RX_txBuffer - RAM buffer pointer for save data for transmission
    *  RX_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  RX_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  RX_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void RX_PutChar(uint8 txDataByte) 
    {
    #if (RX_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((RX_TX_BUFFER_SIZE > RX_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            RX_DisableTxInt();
        #endif /* (RX_TX_BUFFER_SIZE > RX_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = RX_txBufferWrite;
            locTxBufferRead  = RX_txBufferRead;

        #if ((RX_TX_BUFFER_SIZE > RX_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            RX_EnableTxInt();
        #endif /* (RX_TX_BUFFER_SIZE > RX_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(RX_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((RX_TXSTATUS_REG & RX_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            RX_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= RX_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            RX_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((RX_TX_BUFFER_SIZE > RX_MAX_BYTE_VALUE) && (CY_PSOC3))
            RX_DisableTxInt();
        #endif /* (RX_TX_BUFFER_SIZE > RX_MAX_BYTE_VALUE) && (CY_PSOC3) */

            RX_txBufferWrite = locTxBufferWrite;

        #if ((RX_TX_BUFFER_SIZE > RX_MAX_BYTE_VALUE) && (CY_PSOC3))
            RX_EnableTxInt();
        #endif /* (RX_TX_BUFFER_SIZE > RX_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (RX_TXSTATUS_REG & RX_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                RX_SetPendingTxInt();
            }
        }

    #else

        while((RX_TXSTATUS_REG & RX_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        RX_TXDATA_REG = txDataByte;

    #endif /* RX_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: RX_PutString
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
    *  RX_initVar - checked to identify that the component has been
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
    void RX_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(RX_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                RX_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: RX_PutArray
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
    *  RX_initVar - checked to identify that the component has been
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
    void RX_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(RX_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                RX_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: RX_PutCRLF
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
    *  RX_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void RX_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(RX_initVar != 0u)
        {
            RX_PutChar(txDataByte);
            RX_PutChar(0x0Du);
            RX_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: RX_GetTxBufferSize
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
    *  RX_txBufferWrite - used to calculate left space.
    *  RX_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 RX_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (RX_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        RX_DisableTxInt();

        if(RX_txBufferRead == RX_txBufferWrite)
        {
            size = 0u;
        }
        else if(RX_txBufferRead < RX_txBufferWrite)
        {
            size = (RX_txBufferWrite - RX_txBufferRead);
        }
        else
        {
            size = (RX_TX_BUFFER_SIZE - RX_txBufferRead) +
                    RX_txBufferWrite;
        }

        RX_EnableTxInt();

    #else

        size = RX_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & RX_TX_STS_FIFO_FULL) != 0u)
        {
            size = RX_FIFO_LENGTH;
        }
        else if((size & RX_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (RX_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: RX_ClearTxBuffer
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
    *  RX_txBufferWrite - cleared to zero.
    *  RX_txBufferRead - cleared to zero.
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
    void RX_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        RX_TXDATA_AUX_CTL_REG |= (uint8)  RX_TX_FIFO_CLR;
        RX_TXDATA_AUX_CTL_REG &= (uint8) ~RX_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (RX_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        RX_DisableTxInt();

        RX_txBufferRead = 0u;
        RX_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        RX_EnableTxInt();

    #endif /* (RX_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: RX_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   RX_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   RX_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   RX_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   RX_SEND_WAIT_REINIT - Performs both options: 
    *      RX_SEND_BREAK and RX_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  RX_initVar - checked to identify that the component has been
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
    *     When interrupt appear with RX_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The RX_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void RX_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(RX_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(RX_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == RX_SEND_BREAK) ||
                (retMode == RX_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                RX_WriteControlRegister(RX_ReadControlRegister() |
                                                      RX_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                RX_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = RX_TXSTATUS_REG;
                }
                while((tmpStat & RX_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == RX_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == RX_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = RX_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & RX_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == RX_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == RX_REINIT) ||
                (retMode == RX_SEND_WAIT_REINIT) )
            {
                RX_WriteControlRegister(RX_ReadControlRegister() &
                                              (uint8)~RX_CTRL_HD_SEND_BREAK);
            }

        #else /* RX_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == RX_SEND_BREAK) ||
                (retMode == RX_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (RX_PARITY_TYPE != RX__B_UART__NONE_REVB) || \
                                    (RX_PARITY_TYPE_SW != 0u) )
                    RX_WriteControlRegister(RX_ReadControlRegister() |
                                                          RX_CTRL_HD_SEND_BREAK);
                #endif /* End RX_PARITY_TYPE != RX__B_UART__NONE_REVB  */

                #if(RX_TXCLKGEN_DP)
                    txPeriod = RX_TXBITCLKTX_COMPLETE_REG;
                    RX_TXBITCLKTX_COMPLETE_REG = RX_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = RX_TXBITCTR_PERIOD_REG;
                    RX_TXBITCTR_PERIOD_REG = RX_TXBITCTR_BREAKBITS8X;
                #endif /* End RX_TXCLKGEN_DP */

                /* Send zeros */
                RX_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = RX_TXSTATUS_REG;
                }
                while((tmpStat & RX_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == RX_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == RX_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = RX_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & RX_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == RX_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == RX_REINIT) ||
                (retMode == RX_SEND_WAIT_REINIT) )
            {

            #if(RX_TXCLKGEN_DP)
                RX_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                RX_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End RX_TXCLKGEN_DP */

            #if( (RX_PARITY_TYPE != RX__B_UART__NONE_REVB) || \
                 (RX_PARITY_TYPE_SW != 0u) )
                RX_WriteControlRegister(RX_ReadControlRegister() &
                                                      (uint8) ~RX_CTRL_HD_SEND_BREAK);
            #endif /* End RX_PARITY_TYPE != NONE */
            }
        #endif    /* End RX_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: RX_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       RX_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       RX_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears RX_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void RX_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( RX_CONTROL_REG_REMOVED == 0u )
            RX_WriteControlRegister(RX_ReadControlRegister() |
                                                  RX_CTRL_MARK);
        #endif /* End RX_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( RX_CONTROL_REG_REMOVED == 0u )
            RX_WriteControlRegister(RX_ReadControlRegister() &
                                                  (uint8) ~RX_CTRL_MARK);
        #endif /* End RX_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndRX_TX_ENABLED */

#if(RX_HD_ENABLED)


    /*******************************************************************************
    * Function Name: RX_LoadRxConfig
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
    void RX_LoadRxConfig(void) 
    {
        RX_WriteControlRegister(RX_ReadControlRegister() &
                                                (uint8)~RX_CTRL_HD_SEND);
        RX_RXBITCTR_PERIOD_REG = RX_HD_RXBITCTR_INIT;

    #if (RX_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        RX_SetRxInterruptMode(RX_INIT_RX_INTERRUPTS_MASK);
    #endif /* (RX_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: RX_LoadTxConfig
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
    void RX_LoadTxConfig(void) 
    {
    #if (RX_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        RX_SetRxInterruptMode(0u);
    #endif /* (RX_RX_INTERRUPT_ENABLED) */

        RX_WriteControlRegister(RX_ReadControlRegister() | RX_CTRL_HD_SEND);
        RX_RXBITCTR_PERIOD_REG = RX_HD_TXBITCTR_INIT;
    }

#endif  /* RX_HD_ENABLED */


/* [] END OF FILE */
