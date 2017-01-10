/*******************************************************************************
* File Name: SENSORES_PM.c
* Version 2.40
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SENSORES.h"


/***************************************
* Local data allocation
***************************************/

static SENSORES_BACKUP_STRUCT  SENSORES_backup =
{
    SENSORES_DISABLED
};


/*******************************************************************************
* Function Name: SENSORES_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SENSORES_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: SENSORES_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SENSORES_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: SENSORES_Sleep
********************************************************************************
*
* Summary:
*  Stops the ADC operation and saves the configuration registers and component
*  enable state. Should be called just prior to entering sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SENSORES_backup - modified.
*
*******************************************************************************/
void SENSORES_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    SENSORES_SAR_DFT_CTRL_REG |= SENSORES_ADFT_OVERRIDE;
    if((SENSORES_SAR_CTRL_REG  & SENSORES_ENABLE) != 0u)
    {
        if((SENSORES_SAR_SAMPLE_CTRL_REG & SENSORES_CONTINUOUS_EN) != 0u)
        {
            SENSORES_backup.enableState = SENSORES_ENABLED | SENSORES_STARTED;
        }
        else
        {
            SENSORES_backup.enableState = SENSORES_ENABLED;
        }
        SENSORES_StopConvert();
        SENSORES_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((SENSORES_SAR_CTRL_REG & SENSORES_BOOSTPUMP_EN) != 0u)
        {
            SENSORES_SAR_CTRL_REG &= (uint32)~SENSORES_BOOSTPUMP_EN;
            SENSORES_backup.enableState |= SENSORES_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        SENSORES_backup.enableState = SENSORES_DISABLED;
    }
}


/*******************************************************************************
* Function Name: SENSORES_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component enable state and configuration registers.
*  This should be called just after awaking from sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SENSORES_backup - used.
*
*******************************************************************************/
void SENSORES_Wakeup(void)
{
    SENSORES_SAR_DFT_CTRL_REG &= (uint32)~SENSORES_ADFT_OVERRIDE;
    if(SENSORES_backup.enableState != SENSORES_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((SENSORES_backup.enableState & SENSORES_BOOSTPUMP_ENABLED) != 0u)
        {
            SENSORES_SAR_CTRL_REG |= SENSORES_BOOSTPUMP_EN;
        }
        SENSORES_Enable();
        if((SENSORES_backup.enableState & SENSORES_STARTED) != 0u)
        {
            SENSORES_StartConvert();
        }
    }
}
/* [] END OF FILE */
