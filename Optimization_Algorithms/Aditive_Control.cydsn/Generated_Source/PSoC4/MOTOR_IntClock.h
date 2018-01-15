/*******************************************************************************
* File Name: MOTOR_IntClock.h
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

#if !defined(CY_CLOCK_MOTOR_IntClock_H)
#define CY_CLOCK_MOTOR_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void MOTOR_IntClock_StartEx(uint32 alignClkDiv);
#define MOTOR_IntClock_Start() \
    MOTOR_IntClock_StartEx(MOTOR_IntClock__PA_DIV_ID)

#else

void MOTOR_IntClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void MOTOR_IntClock_Stop(void);

void MOTOR_IntClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 MOTOR_IntClock_GetDividerRegister(void);
uint8  MOTOR_IntClock_GetFractionalDividerRegister(void);

#define MOTOR_IntClock_Enable()                         MOTOR_IntClock_Start()
#define MOTOR_IntClock_Disable()                        MOTOR_IntClock_Stop()
#define MOTOR_IntClock_SetDividerRegister(clkDivider, reset)  \
    MOTOR_IntClock_SetFractionalDividerRegister((clkDivider), 0u)
#define MOTOR_IntClock_SetDivider(clkDivider)           MOTOR_IntClock_SetDividerRegister((clkDivider), 1u)
#define MOTOR_IntClock_SetDividerValue(clkDivider)      MOTOR_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define MOTOR_IntClock_DIV_ID     MOTOR_IntClock__DIV_ID

#define MOTOR_IntClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define MOTOR_IntClock_CTRL_REG   (*(reg32 *)MOTOR_IntClock__CTRL_REGISTER)
#define MOTOR_IntClock_DIV_REG    (*(reg32 *)MOTOR_IntClock__DIV_REGISTER)

#define MOTOR_IntClock_CMD_DIV_SHIFT          (0u)
#define MOTOR_IntClock_CMD_PA_DIV_SHIFT       (8u)
#define MOTOR_IntClock_CMD_DISABLE_SHIFT      (30u)
#define MOTOR_IntClock_CMD_ENABLE_SHIFT       (31u)

#define MOTOR_IntClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << MOTOR_IntClock_CMD_DISABLE_SHIFT))
#define MOTOR_IntClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << MOTOR_IntClock_CMD_ENABLE_SHIFT))

#define MOTOR_IntClock_DIV_FRAC_MASK  (0x000000F8u)
#define MOTOR_IntClock_DIV_FRAC_SHIFT (3u)
#define MOTOR_IntClock_DIV_INT_MASK   (0xFFFFFF00u)
#define MOTOR_IntClock_DIV_INT_SHIFT  (8u)

#else 

#define MOTOR_IntClock_DIV_REG        (*(reg32 *)MOTOR_IntClock__REGISTER)
#define MOTOR_IntClock_ENABLE_REG     MOTOR_IntClock_DIV_REG
#define MOTOR_IntClock_DIV_FRAC_MASK  MOTOR_IntClock__FRAC_MASK
#define MOTOR_IntClock_DIV_FRAC_SHIFT (16u)
#define MOTOR_IntClock_DIV_INT_MASK   MOTOR_IntClock__DIVIDER_MASK
#define MOTOR_IntClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_MOTOR_IntClock_H) */

/* [] END OF FILE */
