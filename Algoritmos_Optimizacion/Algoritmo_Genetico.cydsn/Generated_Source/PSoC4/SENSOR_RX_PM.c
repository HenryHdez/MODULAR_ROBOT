/*******************************************************************************
* File Name: SENSOR_RX_PM.c
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

#include "SENSOR_RX.h"


/***************************************
* Local data allocation
***************************************/

static SENSOR_RX_BACKUP_STRUCT  SENSOR_RX_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: SENSOR_RX_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the SENSOR_RX_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SENSOR_RX_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SENSOR_RX_SaveConfig(void)
{
    #if(SENSOR_RX_CONTROL_REG_REMOVED == 0u)
        SENSOR_RX_backup.cr = SENSOR_RX_CONTROL_REG;
    #endif /* End SENSOR_RX_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: SENSOR_RX_RestoreConfig
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
*  SENSOR_RX_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling SENSOR_RX_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void SENSOR_RX_RestoreConfig(void)
{
    #if(SENSOR_RX_CONTROL_REG_REMOVED == 0u)
        SENSOR_RX_CONTROL_REG = SENSOR_RX_backup.cr;
    #endif /* End SENSOR_RX_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: SENSOR_RX_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The SENSOR_RX_Sleep() API saves the current component state. Then it
*  calls the SENSOR_RX_Stop() function and calls 
*  SENSOR_RX_SaveConfig() to save the hardware configuration.
*  Call the SENSOR_RX_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SENSOR_RX_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SENSOR_RX_Sleep(void)
{
    #if(SENSOR_RX_RX_ENABLED || SENSOR_RX_HD_ENABLED)
        if((SENSOR_RX_RXSTATUS_ACTL_REG  & SENSOR_RX_INT_ENABLE) != 0u)
        {
            SENSOR_RX_backup.enableState = 1u;
        }
        else
        {
            SENSOR_RX_backup.enableState = 0u;
        }
    #else
        if((SENSOR_RX_TXSTATUS_ACTL_REG  & SENSOR_RX_INT_ENABLE) !=0u)
        {
            SENSOR_RX_backup.enableState = 1u;
        }
        else
        {
            SENSOR_RX_backup.enableState = 0u;
        }
    #endif /* End SENSOR_RX_RX_ENABLED || SENSOR_RX_HD_ENABLED*/

    SENSOR_RX_Stop();
    SENSOR_RX_SaveConfig();
}


/*******************************************************************************
* Function Name: SENSOR_RX_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  SENSOR_RX_Sleep() was called. The SENSOR_RX_Wakeup() function
*  calls the SENSOR_RX_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  SENSOR_RX_Sleep() function was called, the SENSOR_RX_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SENSOR_RX_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SENSOR_RX_Wakeup(void)
{
    SENSOR_RX_RestoreConfig();
    #if( (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) )
        SENSOR_RX_ClearRxBuffer();
    #endif /* End (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) */
    #if(SENSOR_RX_TX_ENABLED || SENSOR_RX_HD_ENABLED)
        SENSOR_RX_ClearTxBuffer();
    #endif /* End SENSOR_RX_TX_ENABLED || SENSOR_RX_HD_ENABLED */

    if(SENSOR_RX_backup.enableState != 0u)
    {
        SENSOR_RX_Enable();
    }
}


/* [] END OF FILE */
