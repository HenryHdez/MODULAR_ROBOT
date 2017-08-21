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

#if !defined(CY_SCB_PVT_SENSOR_4_H)
#define CY_SCB_PVT_SENSOR_4_H

#include "SENSOR_4.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SENSOR_4_SetI2CExtClkInterruptMode(interruptMask) SENSOR_4_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SENSOR_4_ClearI2CExtClkInterruptSource(interruptMask) SENSOR_4_CLEAR_INTR_I2C_EC(interruptMask)
#define SENSOR_4_GetI2CExtClkInterruptSource()                (SENSOR_4_INTR_I2C_EC_REG)
#define SENSOR_4_GetI2CExtClkInterruptMode()                  (SENSOR_4_INTR_I2C_EC_MASK_REG)
#define SENSOR_4_GetI2CExtClkInterruptSourceMasked()          (SENSOR_4_INTR_I2C_EC_MASKED_REG)

#if (!SENSOR_4_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SENSOR_4_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SENSOR_4_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SENSOR_4_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SENSOR_4_CLEAR_INTR_SPI_EC(interruptMask)
    #define SENSOR_4_GetExtSpiClkInterruptSource()                 (SENSOR_4_INTR_SPI_EC_REG)
    #define SENSOR_4_GetExtSpiClkInterruptMode()                   (SENSOR_4_INTR_SPI_EC_MASK_REG)
    #define SENSOR_4_GetExtSpiClkInterruptSourceMasked()           (SENSOR_4_INTR_SPI_EC_MASKED_REG)
#endif /* (!SENSOR_4_CY_SCBIP_V1) */

#if(SENSOR_4_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SENSOR_4_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SENSOR_4_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SENSOR_4_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SENSOR_4_CUSTOM_INTR_HANDLER)
    extern cyisraddress SENSOR_4_customIntrHandler;
#endif /* !defined (CY_REMOVE_SENSOR_4_CUSTOM_INTR_HANDLER) */
#endif /* (SENSOR_4_SCB_IRQ_INTERNAL) */

extern SENSOR_4_BACKUP_STRUCT SENSOR_4_backup;

#if(SENSOR_4_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SENSOR_4_scbMode;
    extern uint8 SENSOR_4_scbEnableWake;
    extern uint8 SENSOR_4_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SENSOR_4_mode;
    extern uint8 SENSOR_4_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SENSOR_4_rxBuffer;
    extern uint8   SENSOR_4_rxDataBits;
    extern uint32  SENSOR_4_rxBufferSize;

    extern volatile uint8 * SENSOR_4_txBuffer;
    extern uint8   SENSOR_4_txDataBits;
    extern uint32  SENSOR_4_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SENSOR_4_numberOfAddr;
    extern uint8 SENSOR_4_subAddrSize;
#endif /* (SENSOR_4_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SENSOR_4_SCB_MODE_I2C_CONST_CFG || \
        SENSOR_4_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SENSOR_4_IntrTxMask;
#endif /* (! (SENSOR_4_SCB_MODE_I2C_CONST_CFG || \
              SENSOR_4_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SENSOR_4_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SENSOR_4_SCB_MODE_I2C_RUNTM_CFG     (SENSOR_4_SCB_MODE_I2C      == SENSOR_4_scbMode)
    #define SENSOR_4_SCB_MODE_SPI_RUNTM_CFG     (SENSOR_4_SCB_MODE_SPI      == SENSOR_4_scbMode)
    #define SENSOR_4_SCB_MODE_UART_RUNTM_CFG    (SENSOR_4_SCB_MODE_UART     == SENSOR_4_scbMode)
    #define SENSOR_4_SCB_MODE_EZI2C_RUNTM_CFG   (SENSOR_4_SCB_MODE_EZI2C    == SENSOR_4_scbMode)
    #define SENSOR_4_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SENSOR_4_SCB_MODE_UNCONFIG == SENSOR_4_scbMode)

    /* Defines wakeup enable */
    #define SENSOR_4_SCB_WAKE_ENABLE_CHECK       (0u != SENSOR_4_scbEnableWake)
#endif /* (SENSOR_4_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SENSOR_4_CY_SCBIP_V1)
    #define SENSOR_4_SCB_PINS_NUMBER    (7u)
#else
    #define SENSOR_4_SCB_PINS_NUMBER    (2u)
#endif /* (!SENSOR_4_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SENSOR_4_H) */


/* [] END OF FILE */
