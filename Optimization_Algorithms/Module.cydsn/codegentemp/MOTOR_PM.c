/*******************************************************************************
* File Name: MOTOR_PM.c
* Version 2.50
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

#include "MOTOR.h"


/***************************************
* Local data allocation
***************************************/

static MOTOR_BACKUP_STRUCT  MOTOR_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: MOTOR_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the MOTOR_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MOTOR_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MOTOR_SaveConfig(void)
{
    #if(MOTOR_CONTROL_REG_REMOVED == 0u)
        MOTOR_backup.cr = MOTOR_CONTROL_REG;
    #endif /* End MOTOR_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: MOTOR_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MOTOR_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling MOTOR_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void MOTOR_RestoreConfig(void)
{
    #if(MOTOR_CONTROL_REG_REMOVED == 0u)
        MOTOR_CONTROL_REG = MOTOR_backup.cr;
    #endif /* End MOTOR_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: MOTOR_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The MOTOR_Sleep() API saves the current component state. Then it
*  calls the MOTOR_Stop() function and calls 
*  MOTOR_SaveConfig() to save the hardware configuration.
*  Call the MOTOR_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MOTOR_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MOTOR_Sleep(void)
{
    #if(MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)
        if((MOTOR_RXSTATUS_ACTL_REG  & MOTOR_INT_ENABLE) != 0u)
        {
            MOTOR_backup.enableState = 1u;
        }
        else
        {
            MOTOR_backup.enableState = 0u;
        }
    #else
        if((MOTOR_TXSTATUS_ACTL_REG  & MOTOR_INT_ENABLE) !=0u)
        {
            MOTOR_backup.enableState = 1u;
        }
        else
        {
            MOTOR_backup.enableState = 0u;
        }
    #endif /* End MOTOR_RX_ENABLED || MOTOR_HD_ENABLED*/

    MOTOR_Stop();
    MOTOR_SaveConfig();
}


/*******************************************************************************
* Function Name: MOTOR_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  MOTOR_Sleep() was called. The MOTOR_Wakeup() function
*  calls the MOTOR_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  MOTOR_Sleep() function was called, the MOTOR_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MOTOR_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MOTOR_Wakeup(void)
{
    MOTOR_RestoreConfig();
    #if( (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) )
        MOTOR_ClearRxBuffer();
    #endif /* End (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) */
    #if(MOTOR_TX_ENABLED || MOTOR_HD_ENABLED)
        MOTOR_ClearTxBuffer();
    #endif /* End MOTOR_TX_ENABLED || MOTOR_HD_ENABLED */

    if(MOTOR_backup.enableState != 0u)
    {
        MOTOR_Enable();
    }
}


/* [] END OF FILE */
