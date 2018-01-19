/*******************************************************************************
* File Name: SLEEP_CTRL.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "SLEEP_CTRL.h"

static SLEEP_CTRL_BACKUP_STRUCT  SLEEP_CTRL_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: SLEEP_CTRL_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet SLEEP_CTRL_SUT.c usage_SLEEP_CTRL_Sleep_Wakeup
*******************************************************************************/
void SLEEP_CTRL_Sleep(void)
{
    #if defined(SLEEP_CTRL__PC)
        SLEEP_CTRL_backup.pcState = SLEEP_CTRL_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            SLEEP_CTRL_backup.usbState = SLEEP_CTRL_CR1_REG;
            SLEEP_CTRL_USB_POWER_REG |= SLEEP_CTRL_USBIO_ENTER_SLEEP;
            SLEEP_CTRL_CR1_REG &= SLEEP_CTRL_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SLEEP_CTRL__SIO)
        SLEEP_CTRL_backup.sioState = SLEEP_CTRL_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        SLEEP_CTRL_SIO_REG &= (uint32)(~SLEEP_CTRL_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: SLEEP_CTRL_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to SLEEP_CTRL_Sleep() for an example usage.
*******************************************************************************/
void SLEEP_CTRL_Wakeup(void)
{
    #if defined(SLEEP_CTRL__PC)
        SLEEP_CTRL_PC = SLEEP_CTRL_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            SLEEP_CTRL_USB_POWER_REG &= SLEEP_CTRL_USBIO_EXIT_SLEEP_PH1;
            SLEEP_CTRL_CR1_REG = SLEEP_CTRL_backup.usbState;
            SLEEP_CTRL_USB_POWER_REG &= SLEEP_CTRL_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SLEEP_CTRL__SIO)
        SLEEP_CTRL_SIO_REG = SLEEP_CTRL_backup.sioState;
    #endif
}


/* [] END OF FILE */
