/*******************************************************************************
* File Name: EMITE_PM.c
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

#include "EMITE.h"


/***************************************
* Local data allocation
***************************************/

static EMITE_BACKUP_STRUCT  EMITE_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: EMITE_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the EMITE_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  EMITE_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EMITE_SaveConfig(void)
{
    #if(EMITE_CONTROL_REG_REMOVED == 0u)
        EMITE_backup.cr = EMITE_CONTROL_REG;
    #endif /* End EMITE_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: EMITE_RestoreConfig
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
*  EMITE_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling EMITE_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void EMITE_RestoreConfig(void)
{
    #if(EMITE_CONTROL_REG_REMOVED == 0u)
        EMITE_CONTROL_REG = EMITE_backup.cr;
    #endif /* End EMITE_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: EMITE_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The EMITE_Sleep() API saves the current component state. Then it
*  calls the EMITE_Stop() function and calls 
*  EMITE_SaveConfig() to save the hardware configuration.
*  Call the EMITE_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  EMITE_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EMITE_Sleep(void)
{
    #if(EMITE_RX_ENABLED || EMITE_HD_ENABLED)
        if((EMITE_RXSTATUS_ACTL_REG  & EMITE_INT_ENABLE) != 0u)
        {
            EMITE_backup.enableState = 1u;
        }
        else
        {
            EMITE_backup.enableState = 0u;
        }
    #else
        if((EMITE_TXSTATUS_ACTL_REG  & EMITE_INT_ENABLE) !=0u)
        {
            EMITE_backup.enableState = 1u;
        }
        else
        {
            EMITE_backup.enableState = 0u;
        }
    #endif /* End EMITE_RX_ENABLED || EMITE_HD_ENABLED*/

    EMITE_Stop();
    EMITE_SaveConfig();
}


/*******************************************************************************
* Function Name: EMITE_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  EMITE_Sleep() was called. The EMITE_Wakeup() function
*  calls the EMITE_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  EMITE_Sleep() function was called, the EMITE_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  EMITE_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void EMITE_Wakeup(void)
{
    EMITE_RestoreConfig();
    #if( (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) )
        EMITE_ClearRxBuffer();
    #endif /* End (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) */
    #if(EMITE_TX_ENABLED || EMITE_HD_ENABLED)
        EMITE_ClearTxBuffer();
    #endif /* End EMITE_TX_ENABLED || EMITE_HD_ENABLED */

    if(EMITE_backup.enableState != 0u)
    {
        EMITE_Enable();
    }
}


/* [] END OF FILE */
