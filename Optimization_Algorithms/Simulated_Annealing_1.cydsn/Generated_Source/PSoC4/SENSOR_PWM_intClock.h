/*******************************************************************************
* File Name: SENSOR_PWM_intClock.h
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

#if !defined(CY_CLOCK_SENSOR_PWM_intClock_H)
#define CY_CLOCK_SENSOR_PWM_intClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void SENSOR_PWM_intClock_StartEx(uint32 alignClkDiv);
#define SENSOR_PWM_intClock_Start() \
    SENSOR_PWM_intClock_StartEx(SENSOR_PWM_intClock__PA_DIV_ID)

#else

void SENSOR_PWM_intClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void SENSOR_PWM_intClock_Stop(void);

void SENSOR_PWM_intClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 SENSOR_PWM_intClock_GetDividerRegister(void);
uint8  SENSOR_PWM_intClock_GetFractionalDividerRegister(void);

#define SENSOR_PWM_intClock_Enable()                         SENSOR_PWM_intClock_Start()
#define SENSOR_PWM_intClock_Disable()                        SENSOR_PWM_intClock_Stop()
#define SENSOR_PWM_intClock_SetDividerRegister(clkDivider, reset)  \
    SENSOR_PWM_intClock_SetFractionalDividerRegister((clkDivider), 0u)
#define SENSOR_PWM_intClock_SetDivider(clkDivider)           SENSOR_PWM_intClock_SetDividerRegister((clkDivider), 1u)
#define SENSOR_PWM_intClock_SetDividerValue(clkDivider)      SENSOR_PWM_intClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define SENSOR_PWM_intClock_DIV_ID     SENSOR_PWM_intClock__DIV_ID

#define SENSOR_PWM_intClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define SENSOR_PWM_intClock_CTRL_REG   (*(reg32 *)SENSOR_PWM_intClock__CTRL_REGISTER)
#define SENSOR_PWM_intClock_DIV_REG    (*(reg32 *)SENSOR_PWM_intClock__DIV_REGISTER)

#define SENSOR_PWM_intClock_CMD_DIV_SHIFT          (0u)
#define SENSOR_PWM_intClock_CMD_PA_DIV_SHIFT       (8u)
#define SENSOR_PWM_intClock_CMD_DISABLE_SHIFT      (30u)
#define SENSOR_PWM_intClock_CMD_ENABLE_SHIFT       (31u)

#define SENSOR_PWM_intClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << SENSOR_PWM_intClock_CMD_DISABLE_SHIFT))
#define SENSOR_PWM_intClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << SENSOR_PWM_intClock_CMD_ENABLE_SHIFT))

#define SENSOR_PWM_intClock_DIV_FRAC_MASK  (0x000000F8u)
#define SENSOR_PWM_intClock_DIV_FRAC_SHIFT (3u)
#define SENSOR_PWM_intClock_DIV_INT_MASK   (0xFFFFFF00u)
#define SENSOR_PWM_intClock_DIV_INT_SHIFT  (8u)

#else 

#define SENSOR_PWM_intClock_DIV_REG        (*(reg32 *)SENSOR_PWM_intClock__REGISTER)
#define SENSOR_PWM_intClock_ENABLE_REG     SENSOR_PWM_intClock_DIV_REG
#define SENSOR_PWM_intClock_DIV_FRAC_MASK  SENSOR_PWM_intClock__FRAC_MASK
#define SENSOR_PWM_intClock_DIV_FRAC_SHIFT (16u)
#define SENSOR_PWM_intClock_DIV_INT_MASK   SENSOR_PWM_intClock__DIVIDER_MASK
#define SENSOR_PWM_intClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_SENSOR_PWM_intClock_H) */

/* [] END OF FILE */
