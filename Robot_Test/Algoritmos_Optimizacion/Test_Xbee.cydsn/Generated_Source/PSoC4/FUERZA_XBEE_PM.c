/*******************************************************************************
* File Name: FUERZA_XBEE.c  
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
#include "FUERZA_XBEE.h"

static FUERZA_XBEE_BACKUP_STRUCT  FUERZA_XBEE_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: FUERZA_XBEE_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function must be called for SIO and USBIO
*  pins. It is not essential if using GPIO or GPIO_OVT pins.
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
*  \snippet FUERZA_XBEE_SUT.c usage_FUERZA_XBEE_Sleep_Wakeup
*******************************************************************************/
void FUERZA_XBEE_Sleep(void)
{
    #if defined(FUERZA_XBEE__PC)
        FUERZA_XBEE_backup.pcState = FUERZA_XBEE_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            FUERZA_XBEE_backup.usbState = FUERZA_XBEE_CR1_REG;
            FUERZA_XBEE_USB_POWER_REG |= FUERZA_XBEE_USBIO_ENTER_SLEEP;
            FUERZA_XBEE_CR1_REG &= FUERZA_XBEE_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(FUERZA_XBEE__SIO)
        FUERZA_XBEE_backup.sioState = FUERZA_XBEE_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        FUERZA_XBEE_SIO_REG &= (uint32)(~FUERZA_XBEE_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: FUERZA_XBEE_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep().
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to FUERZA_XBEE_Sleep() for an example usage.
*******************************************************************************/
void FUERZA_XBEE_Wakeup(void)
{
    #if defined(FUERZA_XBEE__PC)
        FUERZA_XBEE_PC = FUERZA_XBEE_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            FUERZA_XBEE_USB_POWER_REG &= FUERZA_XBEE_USBIO_EXIT_SLEEP_PH1;
            FUERZA_XBEE_CR1_REG = FUERZA_XBEE_backup.usbState;
            FUERZA_XBEE_USB_POWER_REG &= FUERZA_XBEE_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(FUERZA_XBEE__SIO)
        FUERZA_XBEE_SIO_REG = FUERZA_XBEE_backup.sioState;
    #endif
}


/* [] END OF FILE */
