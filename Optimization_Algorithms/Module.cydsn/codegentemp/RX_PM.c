/*******************************************************************************
* File Name: RX_PM.c
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

#include "RX.h"


/***************************************
* Local data allocation
***************************************/

static RX_BACKUP_STRUCT  RX_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: RX_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the RX_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RX_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RX_SaveConfig(void)
{
    #if(RX_CONTROL_REG_REMOVED == 0u)
        RX_backup.cr = RX_CONTROL_REG;
    #endif /* End RX_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: RX_RestoreConfig
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
*  RX_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling RX_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void RX_RestoreConfig(void)
{
    #if(RX_CONTROL_REG_REMOVED == 0u)
        RX_CONTROL_REG = RX_backup.cr;
    #endif /* End RX_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: RX_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The RX_Sleep() API saves the current component state. Then it
*  calls the RX_Stop() function and calls 
*  RX_SaveConfig() to save the hardware configuration.
*  Call the RX_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RX_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RX_Sleep(void)
{
    #if(RX_RX_ENABLED || RX_HD_ENABLED)
        if((RX_RXSTATUS_ACTL_REG  & RX_INT_ENABLE) != 0u)
        {
            RX_backup.enableState = 1u;
        }
        else
        {
            RX_backup.enableState = 0u;
        }
    #else
        if((RX_TXSTATUS_ACTL_REG  & RX_INT_ENABLE) !=0u)
        {
            RX_backup.enableState = 1u;
        }
        else
        {
            RX_backup.enableState = 0u;
        }
    #endif /* End RX_RX_ENABLED || RX_HD_ENABLED*/

    RX_Stop();
    RX_SaveConfig();
}


/*******************************************************************************
* Function Name: RX_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  RX_Sleep() was called. The RX_Wakeup() function
*  calls the RX_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  RX_Sleep() function was called, the RX_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  RX_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void RX_Wakeup(void)
{
    RX_RestoreConfig();
    #if( (RX_RX_ENABLED) || (RX_HD_ENABLED) )
        RX_ClearRxBuffer();
    #endif /* End (RX_RX_ENABLED) || (RX_HD_ENABLED) */
    #if(RX_TX_ENABLED || RX_HD_ENABLED)
        RX_ClearTxBuffer();
    #endif /* End RX_TX_ENABLED || RX_HD_ENABLED */

    if(RX_backup.enableState != 0u)
    {
        RX_Enable();
    }
}


/* [] END OF FILE */
