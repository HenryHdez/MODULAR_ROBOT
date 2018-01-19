/*******************************************************************************
* File Name: Z.c  
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
#include "Z.h"

static Z_BACKUP_STRUCT  Z_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Z_Sleep
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
*  \snippet Z_SUT.c usage_Z_Sleep_Wakeup
*******************************************************************************/
void Z_Sleep(void)
{
    #if defined(Z__PC)
        Z_backup.pcState = Z_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Z_backup.usbState = Z_CR1_REG;
            Z_USB_POWER_REG |= Z_USBIO_ENTER_SLEEP;
            Z_CR1_REG &= Z_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Z__SIO)
        Z_backup.sioState = Z_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Z_SIO_REG &= (uint32)(~Z_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Z_Wakeup
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
*  Refer to Z_Sleep() for an example usage.
*******************************************************************************/
void Z_Wakeup(void)
{
    #if defined(Z__PC)
        Z_PC = Z_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Z_USB_POWER_REG &= Z_USBIO_EXIT_SLEEP_PH1;
            Z_CR1_REG = Z_backup.usbState;
            Z_USB_POWER_REG &= Z_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Z__SIO)
        Z_SIO_REG = Z_backup.sioState;
    #endif
}


/* [] END OF FILE */
