/*******************************************************************************
* File Name: SENSOR_U_PM.c
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

#include "SENSOR_U.h"


/***************************************
* Local data allocation
***************************************/

static SENSOR_U_BACKUP_STRUCT  SENSOR_U_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: SENSOR_U_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the SENSOR_U_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SENSOR_U_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SENSOR_U_SaveConfig(void)
{
    #if(SENSOR_U_CONTROL_REG_REMOVED == 0u)
        SENSOR_U_backup.cr = SENSOR_U_CONTROL_REG;
    #endif /* End SENSOR_U_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: SENSOR_U_RestoreConfig
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
*  SENSOR_U_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling SENSOR_U_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void SENSOR_U_RestoreConfig(void)
{
    #if(SENSOR_U_CONTROL_REG_REMOVED == 0u)
        SENSOR_U_CONTROL_REG = SENSOR_U_backup.cr;
    #endif /* End SENSOR_U_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: SENSOR_U_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The SENSOR_U_Sleep() API saves the current component state. Then it
*  calls the SENSOR_U_Stop() function and calls 
*  SENSOR_U_SaveConfig() to save the hardware configuration.
*  Call the SENSOR_U_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SENSOR_U_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SENSOR_U_Sleep(void)
{
    #if(SENSOR_U_RX_ENABLED || SENSOR_U_HD_ENABLED)
        if((SENSOR_U_RXSTATUS_ACTL_REG  & SENSOR_U_INT_ENABLE) != 0u)
        {
            SENSOR_U_backup.enableState = 1u;
        }
        else
        {
            SENSOR_U_backup.enableState = 0u;
        }
    #else
        if((SENSOR_U_TXSTATUS_ACTL_REG  & SENSOR_U_INT_ENABLE) !=0u)
        {
            SENSOR_U_backup.enableState = 1u;
        }
        else
        {
            SENSOR_U_backup.enableState = 0u;
        }
    #endif /* End SENSOR_U_RX_ENABLED || SENSOR_U_HD_ENABLED*/

    SENSOR_U_Stop();
    SENSOR_U_SaveConfig();
}


/*******************************************************************************
* Function Name: SENSOR_U_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  SENSOR_U_Sleep() was called. The SENSOR_U_Wakeup() function
*  calls the SENSOR_U_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  SENSOR_U_Sleep() function was called, the SENSOR_U_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SENSOR_U_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SENSOR_U_Wakeup(void)
{
    SENSOR_U_RestoreConfig();
    #if( (SENSOR_U_RX_ENABLED) || (SENSOR_U_HD_ENABLED) )
        SENSOR_U_ClearRxBuffer();
    #endif /* End (SENSOR_U_RX_ENABLED) || (SENSOR_U_HD_ENABLED) */
    #if(SENSOR_U_TX_ENABLED || SENSOR_U_HD_ENABLED)
        SENSOR_U_ClearTxBuffer();
    #endif /* End SENSOR_U_TX_ENABLED || SENSOR_U_HD_ENABLED */

    if(SENSOR_U_backup.enableState != 0u)
    {
        SENSOR_U_Enable();
    }
}


/* [] END OF FILE */
