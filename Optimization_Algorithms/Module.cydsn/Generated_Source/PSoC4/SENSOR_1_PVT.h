/***************************************************************************//**
* \file .h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_SENSOR_1_H)
#define CY_SCB_PVT_SENSOR_1_H

#include "SENSOR_1.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SENSOR_1_SetI2CExtClkInterruptMode(interruptMask) SENSOR_1_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SENSOR_1_ClearI2CExtClkInterruptSource(interruptMask) SENSOR_1_CLEAR_INTR_I2C_EC(interruptMask)
#define SENSOR_1_GetI2CExtClkInterruptSource()                (SENSOR_1_INTR_I2C_EC_REG)
#define SENSOR_1_GetI2CExtClkInterruptMode()                  (SENSOR_1_INTR_I2C_EC_MASK_REG)
#define SENSOR_1_GetI2CExtClkInterruptSourceMasked()          (SENSOR_1_INTR_I2C_EC_MASKED_REG)

#if (!SENSOR_1_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SENSOR_1_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SENSOR_1_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SENSOR_1_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SENSOR_1_CLEAR_INTR_SPI_EC(interruptMask)
    #define SENSOR_1_GetExtSpiClkInterruptSource()                 (SENSOR_1_INTR_SPI_EC_REG)
    #define SENSOR_1_GetExtSpiClkInterruptMode()                   (SENSOR_1_INTR_SPI_EC_MASK_REG)
    #define SENSOR_1_GetExtSpiClkInterruptSourceMasked()           (SENSOR_1_INTR_SPI_EC_MASKED_REG)
#endif /* (!SENSOR_1_CY_SCBIP_V1) */

#if(SENSOR_1_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SENSOR_1_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SENSOR_1_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SENSOR_1_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SENSOR_1_CUSTOM_INTR_HANDLER)
    extern cyisraddress SENSOR_1_customIntrHandler;
#endif /* !defined (CY_REMOVE_SENSOR_1_CUSTOM_INTR_HANDLER) */
#endif /* (SENSOR_1_SCB_IRQ_INTERNAL) */

extern SENSOR_1_BACKUP_STRUCT SENSOR_1_backup;

#if(SENSOR_1_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SENSOR_1_scbMode;
    extern uint8 SENSOR_1_scbEnableWake;
    extern uint8 SENSOR_1_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SENSOR_1_mode;
    extern uint8 SENSOR_1_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SENSOR_1_rxBuffer;
    extern uint8   SENSOR_1_rxDataBits;
    extern uint32  SENSOR_1_rxBufferSize;

    extern volatile uint8 * SENSOR_1_txBuffer;
    extern uint8   SENSOR_1_txDataBits;
    extern uint32  SENSOR_1_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SENSOR_1_numberOfAddr;
    extern uint8 SENSOR_1_subAddrSize;
#endif /* (SENSOR_1_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SENSOR_1_SCB_MODE_I2C_CONST_CFG || \
        SENSOR_1_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SENSOR_1_IntrTxMask;
#endif /* (! (SENSOR_1_SCB_MODE_I2C_CONST_CFG || \
              SENSOR_1_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SENSOR_1_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SENSOR_1_SCB_MODE_I2C_RUNTM_CFG     (SENSOR_1_SCB_MODE_I2C      == SENSOR_1_scbMode)
    #define SENSOR_1_SCB_MODE_SPI_RUNTM_CFG     (SENSOR_1_SCB_MODE_SPI      == SENSOR_1_scbMode)
    #define SENSOR_1_SCB_MODE_UART_RUNTM_CFG    (SENSOR_1_SCB_MODE_UART     == SENSOR_1_scbMode)
    #define SENSOR_1_SCB_MODE_EZI2C_RUNTM_CFG   (SENSOR_1_SCB_MODE_EZI2C    == SENSOR_1_scbMode)
    #define SENSOR_1_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SENSOR_1_SCB_MODE_UNCONFIG == SENSOR_1_scbMode)

    /* Defines wakeup enable */
    #define SENSOR_1_SCB_WAKE_ENABLE_CHECK       (0u != SENSOR_1_scbEnableWake)
#endif /* (SENSOR_1_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SENSOR_1_CY_SCBIP_V1)
    #define SENSOR_1_SCB_PINS_NUMBER    (7u)
#else
    #define SENSOR_1_SCB_PINS_NUMBER    (2u)
#endif /* (!SENSOR_1_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SENSOR_1_H) */


/* [] END OF FILE */
