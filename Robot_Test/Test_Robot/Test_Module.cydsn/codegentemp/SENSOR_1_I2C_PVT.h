/***************************************************************************//**
* \file .h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
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

#if !defined(CY_SCB_I2C_PVT_SENSOR_1_H)
#define CY_SCB_I2C_PVT_SENSOR_1_H

#include "SENSOR_1_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 SENSOR_1_state; /* Current state of I2C FSM */

#if(SENSOR_1_I2C_SLAVE_CONST)
    extern volatile uint8 SENSOR_1_slStatus;          /* Slave Status */

    /* Receive buffer variables */
    extern volatile uint8 * SENSOR_1_slWrBufPtr;      /* Pointer to Receive buffer  */
    extern volatile uint32  SENSOR_1_slWrBufSize;     /* Slave Receive buffer size  */
    extern volatile uint32  SENSOR_1_slWrBufIndex;    /* Slave Receive buffer Index */

    /* Transmit buffer variables */
    extern volatile uint8 * SENSOR_1_slRdBufPtr;      /* Pointer to Transmit buffer  */
    extern volatile uint32  SENSOR_1_slRdBufSize;     /* Slave Transmit buffer size  */
    extern volatile uint32  SENSOR_1_slRdBufIndex;    /* Slave Transmit buffer Index */
    extern volatile uint32  SENSOR_1_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
    extern volatile uint8   SENSOR_1_slOverFlowCount; /* Slave Transmit Overflow counter */
#endif /* (SENSOR_1_I2C_SLAVE_CONST) */

#if(SENSOR_1_I2C_MASTER_CONST)
    extern volatile uint16 SENSOR_1_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  SENSOR_1_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * SENSOR_1_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  SENSOR_1_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  SENSOR_1_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * SENSOR_1_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  SENSOR_1_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  SENSOR_1_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  SENSOR_1_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (SENSOR_1_I2C_MASTER_CONST) */

#if (SENSOR_1_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
    extern uint32 (*SENSOR_1_customAddressHandler) (void);
#endif /* (SENSOR_1_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */

/***************************************
*     Private Function Prototypes
***************************************/

#if(SENSOR_1_SCB_MODE_I2C_CONST_CFG)
    void SENSOR_1_I2CInit(void);
#endif /* (SENSOR_1_SCB_MODE_I2C_CONST_CFG) */

void SENSOR_1_I2CStop(void);
void SENSOR_1_I2CSaveConfig(void);
void SENSOR_1_I2CRestoreConfig(void);

#if(SENSOR_1_I2C_MASTER_CONST)
    void SENSOR_1_I2CReStartGeneration(void);
#endif /* (SENSOR_1_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_SENSOR_1_H) */


/* [] END OF FILE */
