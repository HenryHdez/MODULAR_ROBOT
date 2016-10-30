/*******************************************************************************
* File Name: SENSORES_intClock.h
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

#if !defined(CY_CLOCK_SENSORES_intClock_H)
#define CY_CLOCK_SENSORES_intClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void SENSORES_intClock_StartEx(uint32 alignClkDiv);
#define SENSORES_intClock_Start() \
    SENSORES_intClock_StartEx(SENSORES_intClock__PA_DIV_ID)

#else

void SENSORES_intClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void SENSORES_intClock_Stop(void);

void SENSORES_intClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 SENSORES_intClock_GetDividerRegister(void);
uint8  SENSORES_intClock_GetFractionalDividerRegister(void);

#define SENSORES_intClock_Enable()                         SENSORES_intClock_Start()
#define SENSORES_intClock_Disable()                        SENSORES_intClock_Stop()
#define SENSORES_intClock_SetDividerRegister(clkDivider, reset)  \
    SENSORES_intClock_SetFractionalDividerRegister((clkDivider), 0u)
#define SENSORES_intClock_SetDivider(clkDivider)           SENSORES_intClock_SetDividerRegister((clkDivider), 1u)
#define SENSORES_intClock_SetDividerValue(clkDivider)      SENSORES_intClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define SENSORES_intClock_DIV_ID     SENSORES_intClock__DIV_ID

#define SENSORES_intClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define SENSORES_intClock_CTRL_REG   (*(reg32 *)SENSORES_intClock__CTRL_REGISTER)
#define SENSORES_intClock_DIV_REG    (*(reg32 *)SENSORES_intClock__DIV_REGISTER)

#define SENSORES_intClock_CMD_DIV_SHIFT          (0u)
#define SENSORES_intClock_CMD_PA_DIV_SHIFT       (8u)
#define SENSORES_intClock_CMD_DISABLE_SHIFT      (30u)
#define SENSORES_intClock_CMD_ENABLE_SHIFT       (31u)

#define SENSORES_intClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << SENSORES_intClock_CMD_DISABLE_SHIFT))
#define SENSORES_intClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << SENSORES_intClock_CMD_ENABLE_SHIFT))

#define SENSORES_intClock_DIV_FRAC_MASK  (0x000000F8u)
#define SENSORES_intClock_DIV_FRAC_SHIFT (3u)
#define SENSORES_intClock_DIV_INT_MASK   (0xFFFFFF00u)
#define SENSORES_intClock_DIV_INT_SHIFT  (8u)

#else 

#define SENSORES_intClock_DIV_REG        (*(reg32 *)SENSORES_intClock__REGISTER)
#define SENSORES_intClock_ENABLE_REG     SENSORES_intClock_DIV_REG
#define SENSORES_intClock_DIV_FRAC_MASK  SENSORES_intClock__FRAC_MASK
#define SENSORES_intClock_DIV_FRAC_SHIFT (16u)
#define SENSORES_intClock_DIV_INT_MASK   SENSORES_intClock__DIVIDER_MASK
#define SENSORES_intClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_SENSORES_intClock_H) */

/* [] END OF FILE */
