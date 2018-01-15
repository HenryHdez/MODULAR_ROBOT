/*******************************************************************************
* File Name: SENSOR_PM.c
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

#include "SENSOR.h"


/***************************************
* Local data allocation
***************************************/

static SENSOR_BACKUP_STRUCT  SENSOR_backup =
{
    SENSOR_DISABLED
};


/*******************************************************************************
* Function Name: SENSOR_SaveConfig
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
void SENSOR_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: SENSOR_RestoreConfig
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
void SENSOR_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: SENSOR_Sleep
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
*  SENSOR_backup - modified.
*
*******************************************************************************/
void SENSOR_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    SENSOR_SAR_DFT_CTRL_REG |= SENSOR_ADFT_OVERRIDE;
    if((SENSOR_SAR_CTRL_REG  & SENSOR_ENABLE) != 0u)
    {
        if((SENSOR_SAR_SAMPLE_CTRL_REG & SENSOR_CONTINUOUS_EN) != 0u)
        {
            SENSOR_backup.enableState = SENSOR_ENABLED | SENSOR_STARTED;
        }
        else
        {
            SENSOR_backup.enableState = SENSOR_ENABLED;
        }
        SENSOR_StopConvert();
        SENSOR_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((SENSOR_SAR_CTRL_REG & SENSOR_BOOSTPUMP_EN) != 0u)
        {
            SENSOR_SAR_CTRL_REG &= (uint32)~SENSOR_BOOSTPUMP_EN;
            SENSOR_backup.enableState |= SENSOR_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        SENSOR_backup.enableState = SENSOR_DISABLED;
    }
}


/*******************************************************************************
* Function Name: SENSOR_Wakeup
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
*  SENSOR_backup - used.
*
*******************************************************************************/
void SENSOR_Wakeup(void)
{
    SENSOR_SAR_DFT_CTRL_REG &= (uint32)~SENSOR_ADFT_OVERRIDE;
    if(SENSOR_backup.enableState != SENSOR_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((SENSOR_backup.enableState & SENSOR_BOOSTPUMP_ENABLED) != 0u)
        {
            SENSOR_SAR_CTRL_REG |= SENSOR_BOOSTPUMP_EN;
        }
        SENSOR_Enable();
        if((SENSOR_backup.enableState & SENSOR_STARTED) != 0u)
        {
            SENSOR_StartConvert();
        }
    }
}
/* [] END OF FILE */
