/*******************************************************************************
* File Name: TX_RX.c  
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
#include "TX_RX.h"

static TX_RX_BACKUP_STRUCT  TX_RX_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: TX_RX_Sleep
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
*  \snippet TX_RX_SUT.c usage_TX_RX_Sleep_Wakeup
*******************************************************************************/
void TX_RX_Sleep(void)
{
    #if defined(TX_RX__PC)
        TX_RX_backup.pcState = TX_RX_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            TX_RX_backup.usbState = TX_RX_CR1_REG;
            TX_RX_USB_POWER_REG |= TX_RX_USBIO_ENTER_SLEEP;
            TX_RX_CR1_REG &= TX_RX_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(TX_RX__SIO)
        TX_RX_backup.sioState = TX_RX_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        TX_RX_SIO_REG &= (uint32)(~TX_RX_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: TX_RX_Wakeup
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
*  Refer to TX_RX_Sleep() for an example usage.
*******************************************************************************/
void TX_RX_Wakeup(void)
{
    #if defined(TX_RX__PC)
        TX_RX_PC = TX_RX_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            TX_RX_USB_POWER_REG &= TX_RX_USBIO_EXIT_SLEEP_PH1;
            TX_RX_CR1_REG = TX_RX_backup.usbState;
            TX_RX_USB_POWER_REG &= TX_RX_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(TX_RX__SIO)
        TX_RX_SIO_REG = TX_RX_backup.sioState;
    #endif
}


/* [] END OF FILE */
