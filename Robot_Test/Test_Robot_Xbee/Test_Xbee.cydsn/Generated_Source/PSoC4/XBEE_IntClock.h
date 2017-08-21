/*******************************************************************************
* File Name: XBEE_IntClock.h
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

#if !defined(CY_CLOCK_XBEE_IntClock_H)
#define CY_CLOCK_XBEE_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void XBEE_IntClock_StartEx(uint32 alignClkDiv);
#define XBEE_IntClock_Start() \
    XBEE_IntClock_StartEx(XBEE_IntClock__PA_DIV_ID)

#else

void XBEE_IntClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void XBEE_IntClock_Stop(void);

void XBEE_IntClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 XBEE_IntClock_GetDividerRegister(void);
uint8  XBEE_IntClock_GetFractionalDividerRegister(void);

#define XBEE_IntClock_Enable()                         XBEE_IntClock_Start()
#define XBEE_IntClock_Disable()                        XBEE_IntClock_Stop()
#define XBEE_IntClock_SetDividerRegister(clkDivider, reset)  \
    XBEE_IntClock_SetFractionalDividerRegister((clkDivider), 0u)
#define XBEE_IntClock_SetDivider(clkDivider)           XBEE_IntClock_SetDividerRegister((clkDivider), 1u)
#define XBEE_IntClock_SetDividerValue(clkDivider)      XBEE_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define XBEE_IntClock_DIV_ID     XBEE_IntClock__DIV_ID

#define XBEE_IntClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define XBEE_IntClock_CTRL_REG   (*(reg32 *)XBEE_IntClock__CTRL_REGISTER)
#define XBEE_IntClock_DIV_REG    (*(reg32 *)XBEE_IntClock__DIV_REGISTER)

#define XBEE_IntClock_CMD_DIV_SHIFT          (0u)
#define XBEE_IntClock_CMD_PA_DIV_SHIFT       (8u)
#define XBEE_IntClock_CMD_DISABLE_SHIFT      (30u)
#define XBEE_IntClock_CMD_ENABLE_SHIFT       (31u)

#define XBEE_IntClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << XBEE_IntClock_CMD_DISABLE_SHIFT))
#define XBEE_IntClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << XBEE_IntClock_CMD_ENABLE_SHIFT))

#define XBEE_IntClock_DIV_FRAC_MASK  (0x000000F8u)
#define XBEE_IntClock_DIV_FRAC_SHIFT (3u)
#define XBEE_IntClock_DIV_INT_MASK   (0xFFFFFF00u)
#define XBEE_IntClock_DIV_INT_SHIFT  (8u)

#else 

#define XBEE_IntClock_DIV_REG        (*(reg32 *)XBEE_IntClock__REGISTER)
#define XBEE_IntClock_ENABLE_REG     XBEE_IntClock_DIV_REG
#define XBEE_IntClock_DIV_FRAC_MASK  XBEE_IntClock__FRAC_MASK
#define XBEE_IntClock_DIV_FRAC_SHIFT (16u)
#define XBEE_IntClock_DIV_INT_MASK   XBEE_IntClock__DIVIDER_MASK
#define XBEE_IntClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_XBEE_IntClock_H) */

/* [] END OF FILE */
