/*******************************************************************************
* File Name: SENSOR_PWM_PM.c
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

#include "SENSOR_PWM.h"


/***************************************
* Local data allocation
***************************************/

static SENSOR_PWM_BACKUP_STRUCT  SENSOR_PWM_backup =
{
    SENSOR_PWM_DISABLED
};


/*******************************************************************************
* Function Name: SENSOR_PWM_SaveConfig
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
void SENSOR_PWM_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: SENSOR_PWM_RestoreConfig
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
void SENSOR_PWM_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: SENSOR_PWM_Sleep
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
*  SENSOR_PWM_backup - modified.
*
*******************************************************************************/
void SENSOR_PWM_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    SENSOR_PWM_SAR_DFT_CTRL_REG |= SENSOR_PWM_ADFT_OVERRIDE;
    if((SENSOR_PWM_SAR_CTRL_REG  & SENSOR_PWM_ENABLE) != 0u)
    {
        if((SENSOR_PWM_SAR_SAMPLE_CTRL_REG & SENSOR_PWM_CONTINUOUS_EN) != 0u)
        {
            SENSOR_PWM_backup.enableState = SENSOR_PWM_ENABLED | SENSOR_PWM_STARTED;
        }
        else
        {
            SENSOR_PWM_backup.enableState = SENSOR_PWM_ENABLED;
        }
        SENSOR_PWM_StopConvert();
        SENSOR_PWM_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((SENSOR_PWM_SAR_CTRL_REG & SENSOR_PWM_BOOSTPUMP_EN) != 0u)
        {
            SENSOR_PWM_SAR_CTRL_REG &= (uint32)~SENSOR_PWM_BOOSTPUMP_EN;
            SENSOR_PWM_backup.enableState |= SENSOR_PWM_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        SENSOR_PWM_backup.enableState = SENSOR_PWM_DISABLED;
    }
}


/*******************************************************************************
* Function Name: SENSOR_PWM_Wakeup
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
*  SENSOR_PWM_backup - used.
*
*******************************************************************************/
void SENSOR_PWM_Wakeup(void)
{
    SENSOR_PWM_SAR_DFT_CTRL_REG &= (uint32)~SENSOR_PWM_ADFT_OVERRIDE;
    if(SENSOR_PWM_backup.enableState != SENSOR_PWM_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((SENSOR_PWM_backup.enableState & SENSOR_PWM_BOOSTPUMP_ENABLED) != 0u)
        {
            SENSOR_PWM_SAR_CTRL_REG |= SENSOR_PWM_BOOSTPUMP_EN;
        }
        SENSOR_PWM_Enable();
        if((SENSOR_PWM_backup.enableState & SENSOR_PWM_STARTED) != 0u)
        {
            SENSOR_PWM_StartConvert();
        }
    }
}
/* [] END OF FILE */
