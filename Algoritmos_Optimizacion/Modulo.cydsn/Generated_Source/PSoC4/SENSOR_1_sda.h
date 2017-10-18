/*******************************************************************************
* File Name: SENSOR_1_sda.h  
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

#if !defined(CY_PINS_SENSOR_1_sda_H) /* Pins SENSOR_1_sda_H */
#define CY_PINS_SENSOR_1_sda_H

#include "cytypes.h"
#include "cyfitter.h"
#include "SENSOR_1_sda_aliases.h"


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
} SENSOR_1_sda_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   SENSOR_1_sda_Read(void);
void    SENSOR_1_sda_Write(uint8 value);
uint8   SENSOR_1_sda_ReadDataReg(void);
#if defined(SENSOR_1_sda__PC) || (CY_PSOC4_4200L) 
    void    SENSOR_1_sda_SetDriveMode(uint8 mode);
#endif
void    SENSOR_1_sda_SetInterruptMode(uint16 position, uint16 mode);
uint8   SENSOR_1_sda_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void SENSOR_1_sda_Sleep(void); 
void SENSOR_1_sda_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(SENSOR_1_sda__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define SENSOR_1_sda_DRIVE_MODE_BITS        (3)
    #define SENSOR_1_sda_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - SENSOR_1_sda_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the SENSOR_1_sda_SetDriveMode() function.
         *  @{
         */
        #define SENSOR_1_sda_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define SENSOR_1_sda_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define SENSOR_1_sda_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define SENSOR_1_sda_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define SENSOR_1_sda_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define SENSOR_1_sda_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define SENSOR_1_sda_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define SENSOR_1_sda_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define SENSOR_1_sda_MASK               SENSOR_1_sda__MASK
#define SENSOR_1_sda_SHIFT              SENSOR_1_sda__SHIFT
#define SENSOR_1_sda_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SENSOR_1_sda_SetInterruptMode() function.
     *  @{
     */
        #define SENSOR_1_sda_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define SENSOR_1_sda_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define SENSOR_1_sda_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define SENSOR_1_sda_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(SENSOR_1_sda__SIO)
    #define SENSOR_1_sda_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(SENSOR_1_sda__PC) && (CY_PSOC4_4200L)
    #define SENSOR_1_sda_USBIO_ENABLE               ((uint32)0x80000000u)
    #define SENSOR_1_sda_USBIO_DISABLE              ((uint32)(~SENSOR_1_sda_USBIO_ENABLE))
    #define SENSOR_1_sda_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define SENSOR_1_sda_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define SENSOR_1_sda_USBIO_ENTER_SLEEP          ((uint32)((1u << SENSOR_1_sda_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << SENSOR_1_sda_USBIO_SUSPEND_DEL_SHIFT)))
    #define SENSOR_1_sda_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << SENSOR_1_sda_USBIO_SUSPEND_SHIFT)))
    #define SENSOR_1_sda_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << SENSOR_1_sda_USBIO_SUSPEND_DEL_SHIFT)))
    #define SENSOR_1_sda_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(SENSOR_1_sda__PC)
    /* Port Configuration */
    #define SENSOR_1_sda_PC                 (* (reg32 *) SENSOR_1_sda__PC)
#endif
/* Pin State */
#define SENSOR_1_sda_PS                     (* (reg32 *) SENSOR_1_sda__PS)
/* Data Register */
#define SENSOR_1_sda_DR                     (* (reg32 *) SENSOR_1_sda__DR)
/* Input Buffer Disable Override */
#define SENSOR_1_sda_INP_DIS                (* (reg32 *) SENSOR_1_sda__PC2)

/* Interrupt configuration Registers */
#define SENSOR_1_sda_INTCFG                 (* (reg32 *) SENSOR_1_sda__INTCFG)
#define SENSOR_1_sda_INTSTAT                (* (reg32 *) SENSOR_1_sda__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define SENSOR_1_sda_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(SENSOR_1_sda__SIO)
    #define SENSOR_1_sda_SIO_REG            (* (reg32 *) SENSOR_1_sda__SIO)
#endif /* (SENSOR_1_sda__SIO_CFG) */

/* USBIO registers */
#if !defined(SENSOR_1_sda__PC) && (CY_PSOC4_4200L)
    #define SENSOR_1_sda_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define SENSOR_1_sda_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define SENSOR_1_sda_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define SENSOR_1_sda_DRIVE_MODE_SHIFT       (0x00u)
#define SENSOR_1_sda_DRIVE_MODE_MASK        (0x07u << SENSOR_1_sda_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins SENSOR_1_sda_H */


/* [] END OF FILE */
