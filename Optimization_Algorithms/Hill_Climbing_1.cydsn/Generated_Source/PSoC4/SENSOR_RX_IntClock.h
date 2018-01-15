/*******************************************************************************
* File Name: SENSOR_RX_IntClock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_SENSOR_RX_IntClock_H)
#define CY_CLOCK_SENSOR_RX_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void SENSOR_RX_IntClock_StartEx(uint32 alignClkDiv);
#define SENSOR_RX_IntClock_Start() \
    SENSOR_RX_IntClock_StartEx(SENSOR_RX_IntClock__PA_DIV_ID)

#else

void SENSOR_RX_IntClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void SENSOR_RX_IntClock_Stop(void);

void SENSOR_RX_IntClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 SENSOR_RX_IntClock_GetDividerRegister(void);
uint8  SENSOR_RX_IntClock_GetFractionalDividerRegister(void);

#define SENSOR_RX_IntClock_Enable()                         SENSOR_RX_IntClock_Start()
#define SENSOR_RX_IntClock_Disable()                        SENSOR_RX_IntClock_Stop()
#define SENSOR_RX_IntClock_SetDividerRegister(clkDivider, reset)  \
    SENSOR_RX_IntClock_SetFractionalDividerRegister((clkDivider), 0u)
#define SENSOR_RX_IntClock_SetDivider(clkDivider)           SENSOR_RX_IntClock_SetDividerRegister((clkDivider), 1u)
#define SENSOR_RX_IntClock_SetDividerValue(clkDivider)      SENSOR_RX_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define SENSOR_RX_IntClock_DIV_ID     SENSOR_RX_IntClock__DIV_ID

#define SENSOR_RX_IntClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define SENSOR_RX_IntClock_CTRL_REG   (*(reg32 *)SENSOR_RX_IntClock__CTRL_REGISTER)
#define SENSOR_RX_IntClock_DIV_REG    (*(reg32 *)SENSOR_RX_IntClock__DIV_REGISTER)

#define SENSOR_RX_IntClock_CMD_DIV_SHIFT          (0u)
#define SENSOR_RX_IntClock_CMD_PA_DIV_SHIFT       (8u)
#define SENSOR_RX_IntClock_CMD_DISABLE_SHIFT      (30u)
#define SENSOR_RX_IntClock_CMD_ENABLE_SHIFT       (31u)

#define SENSOR_RX_IntClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << SENSOR_RX_IntClock_CMD_DISABLE_SHIFT))
#define SENSOR_RX_IntClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << SENSOR_RX_IntClock_CMD_ENABLE_SHIFT))

#define SENSOR_RX_IntClock_DIV_FRAC_MASK  (0x000000F8u)
#define SENSOR_RX_IntClock_DIV_FRAC_SHIFT (3u)
#define SENSOR_RX_IntClock_DIV_INT_MASK   (0xFFFFFF00u)
#define SENSOR_RX_IntClock_DIV_INT_SHIFT  (8u)

#else 

#define SENSOR_RX_IntClock_DIV_REG        (*(reg32 *)SENSOR_RX_IntClock__REGISTER)
#define SENSOR_RX_IntClock_ENABLE_REG     SENSOR_RX_IntClock_DIV_REG
#define SENSOR_RX_IntClock_DIV_FRAC_MASK  SENSOR_RX_IntClock__FRAC_MASK
#define SENSOR_RX_IntClock_DIV_FRAC_SHIFT (16u)
#define SENSOR_RX_IntClock_DIV_INT_MASK   SENSOR_RX_IntClock__DIVIDER_MASK
#define SENSOR_RX_IntClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_SENSOR_RX_IntClock_H) */

/* [] END OF FILE */
