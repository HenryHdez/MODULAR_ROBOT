/*******************************************************************************
* File Name: SLEEP_XBEE.c  
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
#include "SLEEP_XBEE.h"

static SLEEP_XBEE_BACKUP_STRUCT  SLEEP_XBEE_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: SLEEP_XBEE_Sleep
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
*  \snippet SLEEP_XBEE_SUT.c usage_SLEEP_XBEE_Sleep_Wakeup
*******************************************************************************/
void SLEEP_XBEE_Sleep(void)
{
    #if defined(SLEEP_XBEE__PC)
        SLEEP_XBEE_backup.pcState = SLEEP_XBEE_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            SLEEP_XBEE_backup.usbState = SLEEP_XBEE_CR1_REG;
            SLEEP_XBEE_USB_POWER_REG |= SLEEP_XBEE_USBIO_ENTER_SLEEP;
            SLEEP_XBEE_CR1_REG &= SLEEP_XBEE_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SLEEP_XBEE__SIO)
        SLEEP_XBEE_backup.sioState = SLEEP_XBEE_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        SLEEP_XBEE_SIO_REG &= (uint32)(~SLEEP_XBEE_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: SLEEP_XBEE_Wakeup
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
*  Refer to SLEEP_XBEE_Sleep() for an example usage.
*******************************************************************************/
void SLEEP_XBEE_Wakeup(void)
{
    #if defined(SLEEP_XBEE__PC)
        SLEEP_XBEE_PC = SLEEP_XBEE_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            SLEEP_XBEE_USB_POWER_REG &= SLEEP_XBEE_USBIO_EXIT_SLEEP_PH1;
            SLEEP_XBEE_CR1_REG = SLEEP_XBEE_backup.usbState;
            SLEEP_XBEE_USB_POWER_REG &= SLEEP_XBEE_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(SLEEP_XBEE__SIO)
        SLEEP_XBEE_SIO_REG = SLEEP_XBEE_backup.sioState;
    #endif
}


/* [] END OF FILE */
