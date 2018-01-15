/*******************************************************************************
* File Name: SENSOR_1_SCBCLK.h
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

#if !defined(CY_CLOCK_SENSOR_1_SCBCLK_H)
#define CY_CLOCK_SENSOR_1_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void SENSOR_1_SCBCLK_StartEx(uint32 alignClkDiv);
#define SENSOR_1_SCBCLK_Start() \
    SENSOR_1_SCBCLK_StartEx(SENSOR_1_SCBCLK__PA_DIV_ID)

#else

void SENSOR_1_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void SENSOR_1_SCBCLK_Stop(void);

void SENSOR_1_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 SENSOR_1_SCBCLK_GetDividerRegister(void);
uint8  SENSOR_1_SCBCLK_GetFractionalDividerRegister(void);

#define SENSOR_1_SCBCLK_Enable()                         SENSOR_1_SCBCLK_Start()
#define SENSOR_1_SCBCLK_Disable()                        SENSOR_1_SCBCLK_Stop()
#define SENSOR_1_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    SENSOR_1_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define SENSOR_1_SCBCLK_SetDivider(clkDivider)           SENSOR_1_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define SENSOR_1_SCBCLK_SetDividerValue(clkDivider)      SENSOR_1_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define SENSOR_1_SCBCLK_DIV_ID     SENSOR_1_SCBCLK__DIV_ID

#define SENSOR_1_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define SENSOR_1_SCBCLK_CTRL_REG   (*(reg32 *)SENSOR_1_SCBCLK__CTRL_REGISTER)
#define SENSOR_1_SCBCLK_DIV_REG    (*(reg32 *)SENSOR_1_SCBCLK__DIV_REGISTER)

#define SENSOR_1_SCBCLK_CMD_DIV_SHIFT          (0u)
#define SENSOR_1_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define SENSOR_1_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define SENSOR_1_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define SENSOR_1_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << SENSOR_1_SCBCLK_CMD_DISABLE_SHIFT))
#define SENSOR_1_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << SENSOR_1_SCBCLK_CMD_ENABLE_SHIFT))

#define SENSOR_1_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define SENSOR_1_SCBCLK_DIV_FRAC_SHIFT (3u)
#define SENSOR_1_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define SENSOR_1_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define SENSOR_1_SCBCLK_DIV_REG        (*(reg32 *)SENSOR_1_SCBCLK__REGISTER)
#define SENSOR_1_SCBCLK_ENABLE_REG     SENSOR_1_SCBCLK_DIV_REG
#define SENSOR_1_SCBCLK_DIV_FRAC_MASK  SENSOR_1_SCBCLK__FRAC_MASK
#define SENSOR_1_SCBCLK_DIV_FRAC_SHIFT (16u)
#define SENSOR_1_SCBCLK_DIV_INT_MASK   SENSOR_1_SCBCLK__DIVIDER_MASK
#define SENSOR_1_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_SENSOR_1_SCBCLK_H) */

/* [] END OF FILE */
