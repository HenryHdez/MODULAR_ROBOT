/*******************************************************************************
* File Name: TXC_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "TXC.h"

/* Check for removal by optimization */
#if !defined(TXC_Sync_ctrl_reg__REMOVED)

static TXC_BACKUP_STRUCT  TXC_backup = {0u};

    
/*******************************************************************************
* Function Name: TXC_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void TXC_SaveConfig(void) 
{
    TXC_backup.controlState = TXC_Control;
}


/*******************************************************************************
* Function Name: TXC_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void TXC_RestoreConfig(void) 
{
     TXC_Control = TXC_backup.controlState;
}


/*******************************************************************************
* Function Name: TXC_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void TXC_Sleep(void) 
{
    TXC_SaveConfig();
}


/*******************************************************************************
* Function Name: TXC_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void TXC_Wakeup(void)  
{
    TXC_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
