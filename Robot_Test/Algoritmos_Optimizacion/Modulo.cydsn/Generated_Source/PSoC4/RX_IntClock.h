/*******************************************************************************
* File Name: RX_IntClock.h
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

#if !defined(CY_CLOCK_RX_IntClock_H)
#define CY_CLOCK_RX_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void RX_IntClock_StartEx(uint32 alignClkDiv);
#define RX_IntClock_Start() \
    RX_IntClock_StartEx(RX_IntClock__PA_DIV_ID)

#else

void RX_IntClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void RX_IntClock_Stop(void);

void RX_IntClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 RX_IntClock_GetDividerRegister(void);
uint8  RX_IntClock_GetFractionalDividerRegister(void);

#define RX_IntClock_Enable()                         RX_IntClock_Start()
#define RX_IntClock_Disable()                        RX_IntClock_Stop()
#define RX_IntClock_SetDividerRegister(clkDivider, reset)  \
    RX_IntClock_SetFractionalDividerRegister((clkDivider), 0u)
#define RX_IntClock_SetDivider(clkDivider)           RX_IntClock_SetDividerRegister((clkDivider), 1u)
#define RX_IntClock_SetDividerValue(clkDivider)      RX_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define RX_IntClock_DIV_ID     RX_IntClock__DIV_ID

#define RX_IntClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define RX_IntClock_CTRL_REG   (*(reg32 *)RX_IntClock__CTRL_REGISTER)
#define RX_IntClock_DIV_REG    (*(reg32 *)RX_IntClock__DIV_REGISTER)

#define RX_IntClock_CMD_DIV_SHIFT          (0u)
#define RX_IntClock_CMD_PA_DIV_SHIFT       (8u)
#define RX_IntClock_CMD_DISABLE_SHIFT      (30u)
#define RX_IntClock_CMD_ENABLE_SHIFT       (31u)

#define RX_IntClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << RX_IntClock_CMD_DISABLE_SHIFT))
#define RX_IntClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << RX_IntClock_CMD_ENABLE_SHIFT))

#define RX_IntClock_DIV_FRAC_MASK  (0x000000F8u)
#define RX_IntClock_DIV_FRAC_SHIFT (3u)
#define RX_IntClock_DIV_INT_MASK   (0xFFFFFF00u)
#define RX_IntClock_DIV_INT_SHIFT  (8u)

#else 

#define RX_IntClock_DIV_REG        (*(reg32 *)RX_IntClock__REGISTER)
#define RX_IntClock_ENABLE_REG     RX_IntClock_DIV_REG
#define RX_IntClock_DIV_FRAC_MASK  RX_IntClock__FRAC_MASK
#define RX_IntClock_DIV_FRAC_SHIFT (16u)
#define RX_IntClock_DIV_INT_MASK   RX_IntClock__DIVIDER_MASK
#define RX_IntClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_RX_IntClock_H) */

/* [] END OF FILE */
