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

#if !defined(CY_SCB_PVT_SENSOR_3_H)
#define CY_SCB_PVT_SENSOR_3_H

#include "SENSOR_3.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SENSOR_3_SetI2CExtClkInterruptMode(interruptMask) SENSOR_3_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SENSOR_3_ClearI2CExtClkInterruptSource(interruptMask) SENSOR_3_CLEAR_INTR_I2C_EC(interruptMask)
#define SENSOR_3_GetI2CExtClkInterruptSource()                (SENSOR_3_INTR_I2C_EC_REG)
#define SENSOR_3_GetI2CExtClkInterruptMode()                  (SENSOR_3_INTR_I2C_EC_MASK_REG)
#define SENSOR_3_GetI2CExtClkInterruptSourceMasked()          (SENSOR_3_INTR_I2C_EC_MASKED_REG)

#if (!SENSOR_3_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SENSOR_3_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SENSOR_3_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SENSOR_3_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SENSOR_3_CLEAR_INTR_SPI_EC(interruptMask)
    #define SENSOR_3_GetExtSpiClkInterruptSource()                 (SENSOR_3_INTR_SPI_EC_REG)
    #define SENSOR_3_GetExtSpiClkInterruptMode()                   (SENSOR_3_INTR_SPI_EC_MASK_REG)
    #define SENSOR_3_GetExtSpiClkInterruptSourceMasked()           (SENSOR_3_INTR_SPI_EC_MASKED_REG)
#endif /* (!SENSOR_3_CY_SCBIP_V1) */

#if(SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SENSOR_3_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SENSOR_3_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SENSOR_3_CUSTOM_INTR_HANDLER)
    extern cyisraddress SENSOR_3_customIntrHandler;
#endif /* !defined (CY_REMOVE_SENSOR_3_CUSTOM_INTR_HANDLER) */
#endif /* (SENSOR_3_SCB_IRQ_INTERNAL) */

extern SENSOR_3_BACKUP_STRUCT SENSOR_3_backup;

#if(SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SENSOR_3_scbMode;
    extern uint8 SENSOR_3_scbEnableWake;
    extern uint8 SENSOR_3_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SENSOR_3_mode;
    extern uint8 SENSOR_3_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SENSOR_3_rxBuffer;
    extern uint8   SENSOR_3_rxDataBits;
    extern uint32  SENSOR_3_rxBufferSize;

    extern volatile uint8 * SENSOR_3_txBuffer;
    extern uint8   SENSOR_3_txDataBits;
    extern uint32  SENSOR_3_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SENSOR_3_numberOfAddr;
    extern uint8 SENSOR_3_subAddrSize;
#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SENSOR_3_SCB_MODE_I2C_CONST_CFG || \
        SENSOR_3_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SENSOR_3_IntrTxMask;
#endif /* (! (SENSOR_3_SCB_MODE_I2C_CONST_CFG || \
              SENSOR_3_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SENSOR_3_SCB_MODE_I2C_RUNTM_CFG     (SENSOR_3_SCB_MODE_I2C      == SENSOR_3_scbMode)
    #define SENSOR_3_SCB_MODE_SPI_RUNTM_CFG     (SENSOR_3_SCB_MODE_SPI      == SENSOR_3_scbMode)
    #define SENSOR_3_SCB_MODE_UART_RUNTM_CFG    (SENSOR_3_SCB_MODE_UART     == SENSOR_3_scbMode)
    #define SENSOR_3_SCB_MODE_EZI2C_RUNTM_CFG   (SENSOR_3_SCB_MODE_EZI2C    == SENSOR_3_scbMode)
    #define SENSOR_3_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SENSOR_3_SCB_MODE_UNCONFIG == SENSOR_3_scbMode)

    /* Defines wakeup enable */
    #define SENSOR_3_SCB_WAKE_ENABLE_CHECK       (0u != SENSOR_3_scbEnableWake)
#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SENSOR_3_CY_SCBIP_V1)
    #define SENSOR_3_SCB_PINS_NUMBER    (7u)
#else
    #define SENSOR_3_SCB_PINS_NUMBER    (2u)
#endif /* (!SENSOR_3_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SENSOR_3_H) */


/* [] END OF FILE */
