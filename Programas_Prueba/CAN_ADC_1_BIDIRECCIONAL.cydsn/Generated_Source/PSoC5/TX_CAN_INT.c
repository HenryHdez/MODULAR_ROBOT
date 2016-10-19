/*******************************************************************************
* File Name: TX_CAN_INT.c
* Version 3.0
*
* Description:
*  This file contains Interrupt Service Routine (ISR) for CAN Component.
*  The Interrupt handlers functions are generated accordingly to the PSoC
*  Creator Customizer inputs.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "TX_CAN.h"
#include "cyapicallbacks.h"

/* `#START CAN_INT_C_CODE_DEFINITION` */

/* `#END` */

#if (TX_CAN_ARB_LOST)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_ArbLostIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Arbitration Lost Interrupt. Clears
    *  Arbitration Lost interrupt flag. Only generated if Arbitration Lost
    *  Interrupt enable in Customizer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_ArbLostIsr(void) 
    {
        /* Clear Arbitration Lost flag */
        TX_CAN_INT_SR_REG.byte[0u] = TX_CAN_ARBITRATION_LOST_MASK;

        /* `#START ARBITRATION_LOST_ISR` */

        /* `#END` */

        #ifdef TX_CAN_ARB_LOST_ISR_CALLBACK
            TX_CAN_ArbLostIsr_Callback();
        #endif /* TX_CAN_ARB_LOST_ISR_CALLBACK */
    }
#endif /* TX_CAN_ARB_LOST */


#if (TX_CAN_OVERLOAD)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_OvrLdErrorIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Overload Error Interrupt. Clears Overload
    *  Error interrupt flag. Only generated if Overload Error Interrupt enable
    *  in Customizer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_OvrLdErrorIsr(void) 
    {
        /* Clear Overload Error flag */
        TX_CAN_INT_SR_REG.byte[0u] = TX_CAN_OVERLOAD_ERROR_MASK;

        /* `#START OVER_LOAD_ERROR_ISR` */

        /* `#END` */

        #ifdef TX_CAN_OVR_LD_ERROR_ISR_CALLBACK
            TX_CAN_OvrLdErrorIsr_Callback();
        #endif /* TX_CAN_OVR_LD_ERROR_ISR_CALLBACK */
    }
#endif /* TX_CAN_OVERLOAD */


#if (TX_CAN_BIT_ERR)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_BitErrorIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Bit Error Interrupt. Clears Bit Error
    *  interrupt flag. Only generated if Bit Error Interrupt enable in Customizer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_BitErrorIsr(void) 
    {
        /* Clear Bit Error flag */
        TX_CAN_INT_SR_REG.byte[0u] = TX_CAN_BIT_ERROR_MASK;

        /* `#START BIT_ERROR_ISR` */

        /* `#END` */

        #ifdef TX_CAN_BIT_ERROR_ISR_CALLBACK
            TX_CAN_BitErrorIsr_Callback();
        #endif /* TX_CAN_BIT_ERROR_ISR_CALLBACK */
    }
#endif /* TX_CAN_BIT_ERR */


#if (TX_CAN_STUFF_ERR)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_BitStuffErrorIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Bit Stuff Error Interrupt. Clears Bit Stuff
    *  Error interrupt flag. Only generated if Bit Stuff Error Interrupt enable
    *  in Customizer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_BitStuffErrorIsr(void) 
    {
        /* Clear Stuff Error flag */
        TX_CAN_INT_SR_REG.byte[0u] = TX_CAN_STUFF_ERROR_MASK;

        /* `#START BIT_STUFF_ERROR_ISR` */

        /* `#END` */

        #ifdef TX_CAN_BIT_STUFF_ERROR_ISR_CALLBACK
            TX_CAN_BitStuffErrorIsr_Callback();
        #endif /* TX_CAN_BIT_STUFF_ERROR_ISR_CALLBACK */
    }
#endif /* TX_CAN_STUFF_ERR */


#if (TX_CAN_ACK_ERR)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_AckErrorIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Acknowledge Error Interrupt. Clears
    *  Acknowledge Error interrupt flag. Only generated if Acknowledge Error
    *  Interrupt enable in Customizer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_AckErrorIsr(void) 
    {
        /* Clear Acknoledge Error flag */
        TX_CAN_INT_SR_REG.byte[0u] = TX_CAN_ACK_ERROR_MASK;

        /* `#START ACKNOWLEDGE_ERROR_ISR` */

        /* `#END` */

        #ifdef TX_CAN_ACK_ERROR_ISR_CALLBACK
            TX_CAN_AckErrorIsr_Callback();
        #endif /* TX_CAN_ACK_ERROR_ISR_CALLBACK */
    }
#endif /* TX_CAN_ACK_ERR */


#if (TX_CAN_FORM_ERR)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_MsgErrorIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Form Error Interrupt. Clears Form Error
    *  interrupt flag. Only generated if Form Error Interrupt enable in Customizer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_MsgErrorIsr(void) 
    {
        /* Clear Form Error flag */
        TX_CAN_INT_SR_REG.byte[0u] = TX_CAN_FORM_ERROR_MASK;

        /* `#START MESSAGE_ERROR_ISR` */

        /* `#END` */

        #ifdef TX_CAN_MSG_ERROR_ISR_CALLBACK
            TX_CAN_MsgErrorIsr_Callback();
        #endif /* TX_CAN_MSG_ERROR_ISR_CALLBACK */
    }
#endif /* TX_CAN_FORM_ERR */


#if (TX_CAN_CRC_ERR)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_CrcErrorIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to CRC Error Interrupt. Clears CRC Error
    *  interrupt flag. Only generated if CRC Error Interrupt enable in Customizer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_CrcErrorIsr(void) 
    {
        /* Clear CRC Error flag */
        TX_CAN_INT_SR_REG.byte[1u] = TX_CAN_CRC_ERROR_MASK;

        /* `#START CRC_ERROR_ISR` */

        /* `#END` */

        #ifdef TX_CAN_CRC_ERROR_ISR_CALLBACK
            TX_CAN_CrcErrorIsr_Callback();
        #endif /* TX_CAN_CRC_ERROR_ISR_CALLBACK */
    }
#endif /* TX_CAN_CRC_ERR */


#if (TX_CAN_BUS_OFF)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_BusOffIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Bus Off Interrupt. Places CAN Component
    *  to Stop mode. Only generated if Bus Off Interrupt enable in Customizer.
    *  Recommended setting to enable this interrupt.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Stops CAN component operation.
    *
    *******************************************************************************/
    void TX_CAN_BusOffIsr(void) 
    {
        /* Clear Bus Off flag */
        TX_CAN_INT_SR_REG.byte[1u] = TX_CAN_BUS_OFF_MASK;
        (void) TX_CAN_GlobalIntDisable();

        /* `#START BUS_OFF_ISR` */

        /* `#END` */

        #ifdef TX_CAN_BUS_OFF_ISR_CALLBACK
            TX_CAN_BusOffIsr_Callback();
        #endif /* TX_CAN_BUS_OFF_ISR_CALLBACK */

        (void) TX_CAN_Stop();
    }
#endif /* TX_CAN_BUS_OFF */


#if (TX_CAN_RX_MSG_LOST)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_MsgLostIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Message Lost Interrupt. Clears Message Lost
    *  interrupt flag. Only generated if Message Lost Interrupt enable in Customizer
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_MsgLostIsr(void) 
    {
        /* Clear Receive Message Lost flag */
        TX_CAN_INT_SR_REG.byte[1u] = TX_CAN_RX_MSG_LOST_MASK;

        /* `#START MESSAGE_LOST_ISR` */

        /* `#END` */

        #ifdef TX_CAN_MSG_LOST_ISR_CALLBACK
            TX_CAN_MsgLostIsr_Callback();
        #endif /* TX_CAN_MSG_LOST_ISR_CALLBACK */
    }
#endif /* TX_CAN_RX_MSG_LOST */


#if (TX_CAN_TX_MESSAGE)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_MsgTXIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Transmit Message Interrupt. Clears Transmit
    *  Message interrupt flag. Only generated if Transmit Message Interrupt enable
    *  in Customizer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  Depends on Customer code.
    *
    *******************************************************************************/
    void TX_CAN_MsgTXIsr(void) 
    {
        /* Clear Transmit Message flag */
        TX_CAN_INT_SR_REG.byte[1u] = TX_CAN_TX_MESSAGE_MASK;

        /* `#START MESSAGE_TRANSMITTED_ISR` */

        /* `#END` */

        #ifdef TX_CAN_MSG_TX_ISR_CALLBACK
            TX_CAN_MsgTXIsr_Callback();
        #endif /* TX_CAN_MSG_TX_ISR_CALLBACK */
    }
#endif /* TX_CAN_TX_MESSAGE */


#if (TX_CAN_RX_MESSAGE)
    /*******************************************************************************
    * FUNCTION NAME:   TX_CAN_MsgRXIsr
    ********************************************************************************
    *
    * Summary:
    *  This function is entry point to Receive Message Interrupt. Clears Receive
    *  Message interrupt flag and call appropriate handlers for Basic and Full
    *  interrupt based mailboxes. Only generated if Receive Message Interrupt
    *  enable in Customizer. Recommended setting to enable this interrupt.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void TX_CAN_MsgRXIsr(void) 
    {
        uint8 mailboxNumber;
        uint16 shift = 0x01u;

        /* Clear Receive Message flag */
        TX_CAN_INT_SR_REG.byte[1u] = TX_CAN_RX_MESSAGE_MASK;

        /* `#START MESSAGE_RECEIVE_ISR` */

        /* `#END` */

        #ifdef TX_CAN_MSG_RX_ISR_CALLBACK
            TX_CAN_MsgRXIsr_Callback();
        #endif /* TX_CAN_MSG_RX_ISR_CALLBACK */

        for (mailboxNumber = 0u; mailboxNumber < TX_CAN_NUMBER_OF_RX_MAILBOXES; mailboxNumber++)
        {
            if ((CY_GET_REG16((reg16 *) &TX_CAN_BUF_SR_REG.byte[0u]) & shift) != 0u)
            {
                if ((TX_CAN_RX[mailboxNumber].rxcmd.byte[0u] & TX_CAN_RX_INT_ENABLE_MASK) != 0u)
                {
                    if ((TX_CAN_RX_MAILBOX_TYPE & shift) != 0u)
                    {
                        /* RX Full mailboxes handler */
                        switch(mailboxNumber)
                        {
                            case 0u : TX_CAN_ReceiveMsg0();
                            break;
                            default:
                            break;
                        }
                    }
                    else
                    {
                        /* RX Basic mailbox handler */
                        TX_CAN_ReceiveMsg(mailboxNumber);
                    }
                }
            }
            shift <<= 1u;
        }
    }
#endif /* TX_CAN_RX_MESSAGE */


/*******************************************************************************
* Function Name: TX_CAN_ISR
********************************************************************************
*
* Summary:
*  This ISR is executed when CAN Core generates and interrupt on one of events:
*  Arb_lost, Overload, Bit_err, Stuff_err, Ack_err, Form_err, Crc_err,
*  Buss_off, Rx_msg_lost, Tx_msg or Rx_msg. The interrupt sources depends
*  on the Customizer inputs.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(TX_CAN_ISR)
{
    #ifdef TX_CAN_ISR_INTERRUPT_CALLBACK
        TX_CAN_ISR_InterruptCallback();
    #endif /* TX_CAN_ISR_INTERRUPT_CALLBACK */
    
    /* Place your Interrupt code here. */
    /* `#START CAN_ISR` */

    /* `#END` */
    
    /* Arbitration */
    #if (TX_CAN_ARB_LOST && (TX_CAN_ARB_LOST_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[0u] & TX_CAN_ARBITRATION_LOST_MASK) != 0u)
        {
            TX_CAN_ArbLostIsr();
        }
    #endif /* TX_CAN_ARB_LOST && TX_CAN_ARB_LOST_USE_HELPER */

    /* Overload Error */
    #if (TX_CAN_OVERLOAD && (TX_CAN_OVERLOAD_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[0u] & TX_CAN_OVERLOAD_ERROR_MASK) != 0u)
        {
            TX_CAN_OvrLdErrorIsr();
        }
    #endif /* TX_CAN_OVERLOAD && TX_CAN_OVERLOAD_USE_HELPER */

    /* Bit Error */
    #if (TX_CAN_BIT_ERR && (TX_CAN_BIT_ERR_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[0u] & TX_CAN_BIT_ERROR_MASK) != 0u)
        {
            TX_CAN_BitErrorIsr();
        }
    #endif /* TX_CAN_BIT_ERR && TX_CAN_BIT_ERR_USE_HELPER */

    /* Bit Staff Error */
    #if (TX_CAN_STUFF_ERR && (TX_CAN_STUFF_ERR_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[0u] & TX_CAN_STUFF_ERROR_MASK) != 0u)
        {
            TX_CAN_BitStuffErrorIsr();
        }
    #endif /* TX_CAN_STUFF_ERR && TX_CAN_STUFF_ERR_USE_HELPER */

    /* ACK Error */
    #if (TX_CAN_ACK_ERR && (TX_CAN_ACK_ERR_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[0u] & TX_CAN_ACK_ERROR_MASK) != 0u)
        {
            TX_CAN_AckErrorIsr();
        }
    #endif /* TX_CAN_ACK_ERR && TX_CAN_ACK_ERR_USE_HELPER */

    /* Form(msg) Error */
    #if (TX_CAN_FORM_ERR && (TX_CAN_FORM_ERR_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[0u] & TX_CAN_FORM_ERROR_MASK) != 0u)
        {
            TX_CAN_MsgErrorIsr();
        }
    #endif /* TX_CAN_FORM_ERR && TX_CAN_FORM_ERR_USE_HELPER */

    /* CRC Error */
    #if (TX_CAN_CRC_ERR && (TX_CAN_CRC_ERR_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[1u] & TX_CAN_CRC_ERROR_MASK) != 0u)
        {
            TX_CAN_CrcErrorIsr();
        }
    #endif /* TX_CAN_CRC_ERR && TX_CAN_CRC_ERR_USE_HELPER */

    /* Bus Off state */
    #if (TX_CAN_BUS_OFF && (TX_CAN_BUS_OFF_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[1u] & TX_CAN_BUS_OFF_MASK) != 0u)
        {
            TX_CAN_BusOffIsr();
        }
    #endif /* TX_CAN_BUS_OFF && TX_CAN_BUS_OFF_USE_HELPER */

    /* Message Lost */
    #if (TX_CAN_RX_MSG_LOST && (TX_CAN_RX_MSG_LOST_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[1u] & TX_CAN_RX_MSG_LOST_MASK) != 0u)
        {
            TX_CAN_MsgLostIsr();
        }
    #endif /* TX_CAN_RX_MSG_LOST && TX_CAN_RX_MSG_LOST_USE_HELPER */

    /* TX Message Send */
    #if (TX_CAN_TX_MESSAGE && (TX_CAN_TX_MESSAGE_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[1u] & TX_CAN_TX_MESSAGE_MASK) != 0u)
        {
            TX_CAN_MsgTXIsr();
        }
    #endif /* TX_CAN_TX_MESSAGE && TX_CAN_TX_MESSAGE_USE_HELPER */

    /* RX Message Available */
    #if (TX_CAN_RX_MESSAGE && (TX_CAN_RX_MESSAGE_USE_HELPER || \
        (!TX_CAN_ADVANCED_INTERRUPT_CFG)))
        if ((TX_CAN_INT_SR_REG.byte[1u] & TX_CAN_RX_MESSAGE_MASK) != 0u)
        {
            TX_CAN_MsgRXIsr();
        }
    #endif /* TX_CAN_RX_MESSAGE && TX_CAN_RX_MESSAGE_USE_HELPER */
}


/* [] END OF FILE */
