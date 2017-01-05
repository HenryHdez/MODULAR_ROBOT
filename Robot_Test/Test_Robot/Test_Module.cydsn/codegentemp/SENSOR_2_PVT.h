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

#if !defined(CY_SCB_PVT_SENSOR_2_H)
#define CY_SCB_PVT_SENSOR_2_H

#include "SENSOR_2.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SENSOR_2_SetI2CExtClkInterruptMode(interruptMask) SENSOR_2_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SENSOR_2_ClearI2CExtClkInterruptSource(interruptMask) SENSOR_2_CLEAR_INTR_I2C_EC(interruptMask)
#define SENSOR_2_GetI2CExtClkInterruptSource()                (SENSOR_2_INTR_I2C_EC_REG)
#define SENSOR_2_GetI2CExtClkInterruptMode()                  (SENSOR_2_INTR_I2C_EC_MASK_REG)
#define SENSOR_2_GetI2CExtClkInterruptSourceMasked()          (SENSOR_2_INTR_I2C_EC_MASKED_REG)

#if (!SENSOR_2_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SENSOR_2_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SENSOR_2_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SENSOR_2_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SENSOR_2_CLEAR_INTR_SPI_EC(interruptMask)
    #define SENSOR_2_GetExtSpiClkInterruptSource()                 (SENSOR_2_INTR_SPI_EC_REG)
    #define SENSOR_2_GetExtSpiClkInterruptMode()                   (SENSOR_2_INTR_SPI_EC_MASK_REG)
    #define SENSOR_2_GetExtSpiClkInterruptSourceMasked()           (SENSOR_2_INTR_SPI_EC_MASKED_REG)
#endif /* (!SENSOR_2_CY_SCBIP_V1) */

#if(SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SENSOR_2_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SENSOR_2_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SENSOR_2_CUSTOM_INTR_HANDLER)
    extern cyisraddress SENSOR_2_customIntrHandler;
#endif /* !defined (CY_REMOVE_SENSOR_2_CUSTOM_INTR_HANDLER) */
#endif /* (SENSOR_2_SCB_IRQ_INTERNAL) */

extern SENSOR_2_BACKUP_STRUCT SENSOR_2_backup;

#if(SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SENSOR_2_scbMode;
    extern uint8 SENSOR_2_scbEnableWake;
    extern uint8 SENSOR_2_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SENSOR_2_mode;
    extern uint8 SENSOR_2_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SENSOR_2_rxBuffer;
    extern uint8   SENSOR_2_rxDataBits;
    extern uint32  SENSOR_2_rxBufferSize;

    extern volatile uint8 * SENSOR_2_txBuffer;
    extern uint8   SENSOR_2_txDataBits;
    extern uint32  SENSOR_2_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SENSOR_2_numberOfAddr;
    extern uint8 SENSOR_2_subAddrSize;
#endif /* (SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SENSOR_2_SCB_MODE_I2C_CONST_CFG || \
        SENSOR_2_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SENSOR_2_IntrTxMask;
#endif /* (! (SENSOR_2_SCB_MODE_I2C_CONST_CFG || \
              SENSOR_2_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SENSOR_2_SCB_MODE_I2C_RUNTM_CFG     (SENSOR_2_SCB_MODE_I2C      == SENSOR_2_scbMode)
    #define SENSOR_2_SCB_MODE_SPI_RUNTM_CFG     (SENSOR_2_SCB_MODE_SPI      == SENSOR_2_scbMode)
    #define SENSOR_2_SCB_MODE_UART_RUNTM_CFG    (SENSOR_2_SCB_MODE_UART     == SENSOR_2_scbMode)
    #define SENSOR_2_SCB_MODE_EZI2C_RUNTM_CFG   (SENSOR_2_SCB_MODE_EZI2C    == SENSOR_2_scbMode)
    #define SENSOR_2_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SENSOR_2_SCB_MODE_UNCONFIG == SENSOR_2_scbMode)

    /* Defines wakeup enable */
    #define SENSOR_2_SCB_WAKE_ENABLE_CHECK       (0u != SENSOR_2_scbEnableWake)
#endif /* (SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SENSOR_2_CY_SCBIP_V1)
    #define SENSOR_2_SCB_PINS_NUMBER    (7u)
#else
    #define SENSOR_2_SCB_PINS_NUMBER    (2u)
#endif /* (!SENSOR_2_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SENSOR_2_H) */


/* [] END OF FILE */
