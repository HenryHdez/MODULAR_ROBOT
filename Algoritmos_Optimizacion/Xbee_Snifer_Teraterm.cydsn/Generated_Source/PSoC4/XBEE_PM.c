/*******************************************************************************
* File Name: XBEE_PM.c
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

#include "XBEE.h"


/***************************************
* Local data allocation
***************************************/

static XBEE_BACKUP_STRUCT  XBEE_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: XBEE_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the XBEE_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  XBEE_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void XBEE_SaveConfig(void)
{
    #if(XBEE_CONTROL_REG_REMOVED == 0u)
        XBEE_backup.cr = XBEE_CONTROL_REG;
    #endif /* End XBEE_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: XBEE_RestoreConfig
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
*  XBEE_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling XBEE_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void XBEE_RestoreConfig(void)
{
    #if(XBEE_CONTROL_REG_REMOVED == 0u)
        XBEE_CONTROL_REG = XBEE_backup.cr;
    #endif /* End XBEE_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: XBEE_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The XBEE_Sleep() API saves the current component state. Then it
*  calls the XBEE_Stop() function and calls 
*  XBEE_SaveConfig() to save the hardware configuration.
*  Call the XBEE_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  XBEE_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void XBEE_Sleep(void)
{
    #if(XBEE_RX_ENABLED || XBEE_HD_ENABLED)
        if((XBEE_RXSTATUS_ACTL_REG  & XBEE_INT_ENABLE) != 0u)
        {
            XBEE_backup.enableState = 1u;
        }
        else
        {
            XBEE_backup.enableState = 0u;
        }
    #else
        if((XBEE_TXSTATUS_ACTL_REG  & XBEE_INT_ENABLE) !=0u)
        {
            XBEE_backup.enableState = 1u;
        }
        else
        {
            XBEE_backup.enableState = 0u;
        }
    #endif /* End XBEE_RX_ENABLED || XBEE_HD_ENABLED*/

    XBEE_Stop();
    XBEE_SaveConfig();
}


/*******************************************************************************
* Function Name: XBEE_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  XBEE_Sleep() was called. The XBEE_Wakeup() function
*  calls the XBEE_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  XBEE_Sleep() function was called, the XBEE_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  XBEE_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void XBEE_Wakeup(void)
{
    XBEE_RestoreConfig();
    #if( (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) )
        XBEE_ClearRxBuffer();
    #endif /* End (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) */
    #if(XBEE_TX_ENABLED || XBEE_HD_ENABLED)
        XBEE_ClearTxBuffer();
    #endif /* End XBEE_TX_ENABLED || XBEE_HD_ENABLED */

    if(XBEE_backup.enableState != 0u)
    {
        XBEE_Enable();
    }
}


/* [] END OF FILE */
