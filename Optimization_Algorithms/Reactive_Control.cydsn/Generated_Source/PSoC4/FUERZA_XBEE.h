/*******************************************************************************
* File Name: FUERZA_XBEE.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_FUERZA_XBEE_H) /* Pins FUERZA_XBEE_H */
#define CY_PINS_FUERZA_XBEE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "FUERZA_XBEE_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} FUERZA_XBEE_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   FUERZA_XBEE_Read(void);
void    FUERZA_XBEE_Write(uint8 value);
uint8   FUERZA_XBEE_ReadDataReg(void);
#if defined(FUERZA_XBEE__PC) || (CY_PSOC4_4200L) 
    void    FUERZA_XBEE_SetDriveMode(uint8 mode);
#endif
void    FUERZA_XBEE_SetInterruptMode(uint16 position, uint16 mode);
uint8   FUERZA_XBEE_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void FUERZA_XBEE_Sleep(void); 
void FUERZA_XBEE_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(FUERZA_XBEE__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define FUERZA_XBEE_DRIVE_MODE_BITS        (3)
    #define FUERZA_XBEE_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - FUERZA_XBEE_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the FUERZA_XBEE_SetDriveMode() function.
         *  @{
         */
        #define FUERZA_XBEE_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define FUERZA_XBEE_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define FUERZA_XBEE_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define FUERZA_XBEE_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define FUERZA_XBEE_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define FUERZA_XBEE_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define FUERZA_XBEE_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define FUERZA_XBEE_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define FUERZA_XBEE_MASK               FUERZA_XBEE__MASK
#define FUERZA_XBEE_SHIFT              FUERZA_XBEE__SHIFT
#define FUERZA_XBEE_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in FUERZA_XBEE_SetInterruptMode() function.
     *  @{
     */
        #define FUERZA_XBEE_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define FUERZA_XBEE_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define FUERZA_XBEE_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define FUERZA_XBEE_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(FUERZA_XBEE__SIO)
    #define FUERZA_XBEE_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(FUERZA_XBEE__PC) && (CY_PSOC4_4200L)
    #define FUERZA_XBEE_USBIO_ENABLE               ((uint32)0x80000000u)
    #define FUERZA_XBEE_USBIO_DISABLE              ((uint32)(~FUERZA_XBEE_USBIO_ENABLE))
    #define FUERZA_XBEE_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define FUERZA_XBEE_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define FUERZA_XBEE_USBIO_ENTER_SLEEP          ((uint32)((1u << FUERZA_XBEE_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << FUERZA_XBEE_USBIO_SUSPEND_DEL_SHIFT)))
    #define FUERZA_XBEE_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << FUERZA_XBEE_USBIO_SUSPEND_SHIFT)))
    #define FUERZA_XBEE_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << FUERZA_XBEE_USBIO_SUSPEND_DEL_SHIFT)))
    #define FUERZA_XBEE_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(FUERZA_XBEE__PC)
    /* Port Configuration */
    #define FUERZA_XBEE_PC                 (* (reg32 *) FUERZA_XBEE__PC)
#endif
/* Pin State */
#define FUERZA_XBEE_PS                     (* (reg32 *) FUERZA_XBEE__PS)
/* Data Register */
#define FUERZA_XBEE_DR                     (* (reg32 *) FUERZA_XBEE__DR)
/* Input Buffer Disable Override */
#define FUERZA_XBEE_INP_DIS                (* (reg32 *) FUERZA_XBEE__PC2)

/* Interrupt configuration Registers */
#define FUERZA_XBEE_INTCFG                 (* (reg32 *) FUERZA_XBEE__INTCFG)
#define FUERZA_XBEE_INTSTAT                (* (reg32 *) FUERZA_XBEE__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define FUERZA_XBEE_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(FUERZA_XBEE__SIO)
    #define FUERZA_XBEE_SIO_REG            (* (reg32 *) FUERZA_XBEE__SIO)
#endif /* (FUERZA_XBEE__SIO_CFG) */

/* USBIO registers */
#if !defined(FUERZA_XBEE__PC) && (CY_PSOC4_4200L)
    #define FUERZA_XBEE_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define FUERZA_XBEE_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define FUERZA_XBEE_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define FUERZA_XBEE_DRIVE_MODE_SHIFT       (0x00u)
#define FUERZA_XBEE_DRIVE_MODE_MASK        (0x07u << FUERZA_XBEE_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins FUERZA_XBEE_H */


/* [] END OF FILE */
