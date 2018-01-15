/*******************************************************************************
* File Name: SLEEP_CTRL.h  
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

#if !defined(CY_PINS_SLEEP_CTRL_H) /* Pins SLEEP_CTRL_H */
#define CY_PINS_SLEEP_CTRL_H

#include "cytypes.h"
#include "cyfitter.h"
#include "SLEEP_CTRL_aliases.h"


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
} SLEEP_CTRL_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   SLEEP_CTRL_Read(void);
void    SLEEP_CTRL_Write(uint8 value);
uint8   SLEEP_CTRL_ReadDataReg(void);
#if defined(SLEEP_CTRL__PC) || (CY_PSOC4_4200L) 
    void    SLEEP_CTRL_SetDriveMode(uint8 mode);
#endif
void    SLEEP_CTRL_SetInterruptMode(uint16 position, uint16 mode);
uint8   SLEEP_CTRL_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void SLEEP_CTRL_Sleep(void); 
void SLEEP_CTRL_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(SLEEP_CTRL__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define SLEEP_CTRL_DRIVE_MODE_BITS        (3)
    #define SLEEP_CTRL_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - SLEEP_CTRL_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the SLEEP_CTRL_SetDriveMode() function.
         *  @{
         */
        #define SLEEP_CTRL_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define SLEEP_CTRL_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define SLEEP_CTRL_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define SLEEP_CTRL_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define SLEEP_CTRL_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define SLEEP_CTRL_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define SLEEP_CTRL_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define SLEEP_CTRL_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define SLEEP_CTRL_MASK               SLEEP_CTRL__MASK
#define SLEEP_CTRL_SHIFT              SLEEP_CTRL__SHIFT
#define SLEEP_CTRL_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SLEEP_CTRL_SetInterruptMode() function.
     *  @{
     */
        #define SLEEP_CTRL_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define SLEEP_CTRL_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define SLEEP_CTRL_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define SLEEP_CTRL_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(SLEEP_CTRL__SIO)
    #define SLEEP_CTRL_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(SLEEP_CTRL__PC) && (CY_PSOC4_4200L)
    #define SLEEP_CTRL_USBIO_ENABLE               ((uint32)0x80000000u)
    #define SLEEP_CTRL_USBIO_DISABLE              ((uint32)(~SLEEP_CTRL_USBIO_ENABLE))
    #define SLEEP_CTRL_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define SLEEP_CTRL_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define SLEEP_CTRL_USBIO_ENTER_SLEEP          ((uint32)((1u << SLEEP_CTRL_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << SLEEP_CTRL_USBIO_SUSPEND_DEL_SHIFT)))
    #define SLEEP_CTRL_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << SLEEP_CTRL_USBIO_SUSPEND_SHIFT)))
    #define SLEEP_CTRL_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << SLEEP_CTRL_USBIO_SUSPEND_DEL_SHIFT)))
    #define SLEEP_CTRL_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(SLEEP_CTRL__PC)
    /* Port Configuration */
    #define SLEEP_CTRL_PC                 (* (reg32 *) SLEEP_CTRL__PC)
#endif
/* Pin State */
#define SLEEP_CTRL_PS                     (* (reg32 *) SLEEP_CTRL__PS)
/* Data Register */
#define SLEEP_CTRL_DR                     (* (reg32 *) SLEEP_CTRL__DR)
/* Input Buffer Disable Override */
#define SLEEP_CTRL_INP_DIS                (* (reg32 *) SLEEP_CTRL__PC2)

/* Interrupt configuration Registers */
#define SLEEP_CTRL_INTCFG                 (* (reg32 *) SLEEP_CTRL__INTCFG)
#define SLEEP_CTRL_INTSTAT                (* (reg32 *) SLEEP_CTRL__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define SLEEP_CTRL_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(SLEEP_CTRL__SIO)
    #define SLEEP_CTRL_SIO_REG            (* (reg32 *) SLEEP_CTRL__SIO)
#endif /* (SLEEP_CTRL__SIO_CFG) */

/* USBIO registers */
#if !defined(SLEEP_CTRL__PC) && (CY_PSOC4_4200L)
    #define SLEEP_CTRL_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define SLEEP_CTRL_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define SLEEP_CTRL_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define SLEEP_CTRL_DRIVE_MODE_SHIFT       (0x00u)
#define SLEEP_CTRL_DRIVE_MODE_MASK        (0x07u << SLEEP_CTRL_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins SLEEP_CTRL_H */


/* [] END OF FILE */
