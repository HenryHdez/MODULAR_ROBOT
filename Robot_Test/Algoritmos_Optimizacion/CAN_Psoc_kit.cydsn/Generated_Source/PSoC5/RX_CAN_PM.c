/*******************************************************************************
* File Name: RX_CAN_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in the low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "RX_CAN.h"

static RX_CAN_BACKUP_STRUCT RX_CAN_backup =
{
    0u,
    #if (CY_PSOC3 || CY_PSOC5)
        0u,
        RX_CAN_INIT_INTERRUPT_MASK,
        RX_CAN_MODE_MASK,
        (((uint32) ((uint32) RX_CAN_SYNC_EDGE    << RX_CAN_EDGE_MODE_SHIFT))     |
        ((uint32) ((uint32) RX_CAN_SAMPLING_MODE << RX_CAN_SAMPLE_MODE_SHIFT))   |
        ((uint32) ((uint32) RX_CAN_CFG_REG_SJW   << RX_CAN_CFG_REG_SJW_SHIFT))   |
        ((uint32) ((uint32) RX_CAN_RESET_TYPE    << RX_CAN_RESET_SHIFT))         |
        ((uint32) ((uint32) RX_CAN_CFG_REG_TSEG2 << RX_CAN_CFG_REG_TSEG2_SHIFT)) |
        ((uint32) ((uint32) RX_CAN_CFG_REG_TSEG1 << RX_CAN_CFG_REG_TSEG1_SHIFT)) |
        ((uint32) ((uint32) RX_CAN_ARBITER       << RX_CAN_ARBITER_SHIFT))       |
        ((uint32) ((uint32) RX_CAN_BITRATE       << RX_CAN_BITRATE_SHIFT)))
    #endif /* CY_PSOC3 || CY_PSOC5 */
};


/*******************************************************************************
* Function Name: RX_CAN_SaveConfig
********************************************************************************
*
* Summary:
*  Save the CAN configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RX_CAN_backup - Modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RX_CAN_SaveConfig(void) 
{
    #if (CY_PSOC3 || CY_PSOC5)
        RX_CAN_backup.intSr = (CY_GET_REG32(RX_CAN_INT_SR_PTR));
        RX_CAN_backup.intEn = (CY_GET_REG32(RX_CAN_INT_EN_PTR));
        RX_CAN_backup.cmd = (CY_GET_REG32(RX_CAN_CMD_PTR));
        RX_CAN_backup.cfg = (CY_GET_REG32(RX_CAN_CFG_PTR));
    #endif /* CY_PSOC3 || CY_PSOC5 */
}


/*******************************************************************************
* Function Name: RX_CAN_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the CAN configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RX_CAN_backup - Used when non-retention registers are restored.
*
* Side Effects:
*  If this API is called without first calling SaveConfig then default values
*  from Customizer will be in the following registers: RX_CAN_INT_SR,
*  RX_CAN_INT_EN, RX_CAN_CMD, RX_CAN_CFG.
*
*******************************************************************************/
void RX_CAN_RestoreConfig(void) 
{
    #if (CY_PSOC3 || CY_PSOC5)
        CY_SET_REG32(RX_CAN_INT_SR_PTR, RX_CAN_backup.intSr);
        CY_SET_REG32(RX_CAN_INT_EN_PTR, RX_CAN_backup.intEn);
        CY_SET_REG32(RX_CAN_CMD_PTR, RX_CAN_backup.cmd);
        CY_SET_REG32(RX_CAN_CFG_PTR, RX_CAN_backup.cfg);
    #endif /* CY_PSOC3 || CY_PSOC5 */
}


/*******************************************************************************
* Function Name: RX_CAN_Sleep
********************************************************************************
*
* Summary:
*  Prepares CAN Component to go to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RX_CAN_backup - Modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RX_CAN_Sleep(void) 
{
    #if (!(CY_PSOC3 || CY_PSOC5))
        uint8 i;
    #endif /* (!(CY_PSOC3 || CY_PSOC5)) */

    if (0u != (CY_GET_REG32(RX_CAN_CMD_PTR) & RX_CAN_MODE_MASK))
    {
        RX_CAN_backup.enableState = 1u;
    }
    else /* CAN block is disabled */
    {
        RX_CAN_backup.enableState = 0u;
    }

    #if (CY_PSOC3 || CY_PSOC5)
        RX_CAN_SaveConfig();
        (void) RX_CAN_Stop();
    #else /* CY_PSOC4 */
        /* Abort respective pending TX message requests */
        for (i = 0u; i < RX_CAN_NUMBER_OF_TX_MAILBOXES; i++)
        {
            RX_CAN_TX_ABORT_MESSAGE(i);
        }

        /* Sets CAN controller to Stop mode */
        (void) RX_CAN_Stop();

        /* Clear Global Interrupt enable Flag */
        (void) RX_CAN_GlobalIntDisable();
    #endif /* CY_PSOC3 || CY_PSOC5 */
}


/*******************************************************************************
* Function Name: RX_CAN_Wakeup
********************************************************************************
*
* Summary:
*  Prepares CAN Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RX_CAN_backup - Used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RX_CAN_Wakeup(void) 
{
    #if (CY_PSOC3 || CY_PSOC5)
        /* Enable CAN block in Active mode */
        RX_CAN_PM_ACT_CFG_REG |= RX_CAN_ACT_PWR_EN;
        /* Enable CAN block in Alternate Active (Standby) mode */
        RX_CAN_PM_STBY_CFG_REG |= RX_CAN_STBY_PWR_EN;

        RX_CAN_RestoreConfig();

        /* Reconfigure Rx and Tx Buffers control registers */
        (void) RX_CAN_RxTxBuffersConfig();

        if (RX_CAN_backup.enableState != 0u)
        {
            /* Enable component's operation */
            (void) RX_CAN_Enable();
        } /* Do nothing if component's block was disabled before */
    #else /* CY_PSOC4 */
        /* Clear all INT_STATUS bits */
        RX_CAN_INT_SR_REG = RX_CAN_INT_STATUS_MASK;

        /* Set Global Interrupt enable Flag */
        (void) RX_CAN_GlobalIntEnable();

        if (RX_CAN_backup.enableState != 0u)
        {
            (void) RX_CAN_Enable();
        } /* Do nothing if component's block was disabled before */
    #endif /* CY_PSOC3 || CY_PSOC5 */
}


/* [] END OF FILE */
