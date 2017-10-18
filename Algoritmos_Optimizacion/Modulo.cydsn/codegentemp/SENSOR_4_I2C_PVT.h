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

#if !defined(CY_SCB_I2C_PVT_SENSOR_4_H)
#define CY_SCB_I2C_PVT_SENSOR_4_H

#include "SENSOR_4_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 SENSOR_4_state; /* Current state of I2C FSM */

#if(SENSOR_4_I2C_SLAVE_CONST)
    extern volatile uint8 SENSOR_4_slStatus;          /* Slave Status */

    /* Receive buffer variables */
    extern volatile uint8 * SENSOR_4_slWrBufPtr;      /* Pointer to Receive buffer  */
    extern volatile uint32  SENSOR_4_slWrBufSize;     /* Slave Receive buffer size  */
    extern volatile uint32  SENSOR_4_slWrBufIndex;    /* Slave Receive buffer Index */

    /* Transmit buffer variables */
    extern volatile uint8 * SENSOR_4_slRdBufPtr;      /* Pointer to Transmit buffer  */
    extern volatile uint32  SENSOR_4_slRdBufSize;     /* Slave Transmit buffer size  */
    extern volatile uint32  SENSOR_4_slRdBufIndex;    /* Slave Transmit buffer Index */
    extern volatile uint32  SENSOR_4_slRdBufIndexTmp; /* Slave Transmit buffer Index Tmp */
    extern volatile uint8   SENSOR_4_slOverFlowCount; /* Slave Transmit Overflow counter */
#endif /* (SENSOR_4_I2C_SLAVE_CONST) */

#if(SENSOR_4_I2C_MASTER_CONST)
    extern volatile uint16 SENSOR_4_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  SENSOR_4_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * SENSOR_4_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  SENSOR_4_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  SENSOR_4_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * SENSOR_4_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  SENSOR_4_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  SENSOR_4_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  SENSOR_4_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (SENSOR_4_I2C_MASTER_CONST) */

#if (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
    extern uint32 (*SENSOR_4_customAddressHandler) (void);
#endif /* (SENSOR_4_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */

/***************************************
*     Private Function Prototypes
***************************************/

#if(SENSOR_4_SCB_MODE_I2C_CONST_CFG)
    void SENSOR_4_I2CInit(void);
#endif /* (SENSOR_4_SCB_MODE_I2C_CONST_CFG) */

void SENSOR_4_I2CStop(void);
void SENSOR_4_I2CSaveConfig(void);
void SENSOR_4_I2CRestoreConfig(void);

#if(SENSOR_4_I2C_MASTER_CONST)
    void SENSOR_4_I2CReStartGeneration(void);
#endif /* (SENSOR_4_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_SENSOR_4_H) */


/* [] END OF FILE */
