/*******************************************************************************
* File Name: RECIBE_PM.c
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

#include "RECIBE.h"


/***************************************
* Local data allocation
***************************************/

static RECIBE_BACKUP_STRUCT  RECIBE_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: RECIBE_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the RECIBE_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RECIBE_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RECIBE_SaveConfig(void)
{
    #if(RECIBE_CONTROL_REG_REMOVED == 0u)
        RECIBE_backup.cr = RECIBE_CONTROL_REG;
    #endif /* End RECIBE_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: RECIBE_RestoreConfig
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
*  RECIBE_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling RECIBE_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void RECIBE_RestoreConfig(void)
{
    #if(RECIBE_CONTROL_REG_REMOVED == 0u)
        RECIBE_CONTROL_REG = RECIBE_backup.cr;
    #endif /* End RECIBE_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: RECIBE_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The RECIBE_Sleep() API saves the current component state. Then it
*  calls the RECIBE_Stop() function and calls 
*  RECIBE_SaveConfig() to save the hardware configuration.
*  Call the RECIBE_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RECIBE_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RECIBE_Sleep(void)
{
    #if(RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)
        if((RECIBE_RXSTATUS_ACTL_REG  & RECIBE_INT_ENABLE) != 0u)
        {
            RECIBE_backup.enableState = 1u;
        }
        else
        {
            RECIBE_backup.enableState = 0u;
        }
    #else
        if((RECIBE_TXSTATUS_ACTL_REG  & RECIBE_INT_ENABLE) !=0u)
        {
            RECIBE_backup.enableState = 1u;
        }
        else
        {
            RECIBE_backup.enableState = 0u;
        }
    #endif /* End RECIBE_RX_ENABLED || RECIBE_HD_ENABLED*/

    RECIBE_Stop();
    RECIBE_SaveConfig();
}


/*******************************************************************************
* Function Name: RECIBE_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  RECIBE_Sleep() was called. The RECIBE_Wakeup() function
*  calls the RECIBE_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  RECIBE_Sleep() function was called, the RECIBE_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RECIBE_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RECIBE_Wakeup(void)
{
    RECIBE_RestoreConfig();
    #if( (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) )
        RECIBE_ClearRxBuffer();
    #endif /* End (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) */
    #if(RECIBE_TX_ENABLED || RECIBE_HD_ENABLED)
        RECIBE_ClearTxBuffer();
    #endif /* End RECIBE_TX_ENABLED || RECIBE_HD_ENABLED */

    if(RECIBE_backup.enableState != 0u)
    {
        RECIBE_Enable();
    }
}


/* [] END OF FILE */
