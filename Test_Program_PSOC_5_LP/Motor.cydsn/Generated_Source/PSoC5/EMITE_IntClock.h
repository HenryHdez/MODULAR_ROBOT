/*******************************************************************************
* File Name: EMITE_IntClock.h
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

#if !defined(CY_CLOCK_EMITE_IntClock_H)
#define CY_CLOCK_EMITE_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void EMITE_IntClock_Start(void) ;
void EMITE_IntClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void EMITE_IntClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void EMITE_IntClock_StandbyPower(uint8 state) ;
void EMITE_IntClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 EMITE_IntClock_GetDividerRegister(void) ;
void EMITE_IntClock_SetModeRegister(uint8 modeBitMask) ;
void EMITE_IntClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 EMITE_IntClock_GetModeRegister(void) ;
void EMITE_IntClock_SetSourceRegister(uint8 clkSource) ;
uint8 EMITE_IntClock_GetSourceRegister(void) ;
#if defined(EMITE_IntClock__CFG3)
void EMITE_IntClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 EMITE_IntClock_GetPhaseRegister(void) ;
#endif /* defined(EMITE_IntClock__CFG3) */

#define EMITE_IntClock_Enable()                       EMITE_IntClock_Start()
#define EMITE_IntClock_Disable()                      EMITE_IntClock_Stop()
#define EMITE_IntClock_SetDivider(clkDivider)         EMITE_IntClock_SetDividerRegister(clkDivider, 1u)
#define EMITE_IntClock_SetDividerValue(clkDivider)    EMITE_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define EMITE_IntClock_SetMode(clkMode)               EMITE_IntClock_SetModeRegister(clkMode)
#define EMITE_IntClock_SetSource(clkSource)           EMITE_IntClock_SetSourceRegister(clkSource)
#if defined(EMITE_IntClock__CFG3)
#define EMITE_IntClock_SetPhase(clkPhase)             EMITE_IntClock_SetPhaseRegister(clkPhase)
#define EMITE_IntClock_SetPhaseValue(clkPhase)        EMITE_IntClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(EMITE_IntClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define EMITE_IntClock_CLKEN              (* (reg8 *) EMITE_IntClock__PM_ACT_CFG)
#define EMITE_IntClock_CLKEN_PTR          ((reg8 *) EMITE_IntClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define EMITE_IntClock_CLKSTBY            (* (reg8 *) EMITE_IntClock__PM_STBY_CFG)
#define EMITE_IntClock_CLKSTBY_PTR        ((reg8 *) EMITE_IntClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define EMITE_IntClock_DIV_LSB            (* (reg8 *) EMITE_IntClock__CFG0)
#define EMITE_IntClock_DIV_LSB_PTR        ((reg8 *) EMITE_IntClock__CFG0)
#define EMITE_IntClock_DIV_PTR            ((reg16 *) EMITE_IntClock__CFG0)

/* Clock MSB divider configuration register. */
#define EMITE_IntClock_DIV_MSB            (* (reg8 *) EMITE_IntClock__CFG1)
#define EMITE_IntClock_DIV_MSB_PTR        ((reg8 *) EMITE_IntClock__CFG1)

/* Mode and source configuration register */
#define EMITE_IntClock_MOD_SRC            (* (reg8 *) EMITE_IntClock__CFG2)
#define EMITE_IntClock_MOD_SRC_PTR        ((reg8 *) EMITE_IntClock__CFG2)

#if defined(EMITE_IntClock__CFG3)
/* Analog clock phase configuration register */
#define EMITE_IntClock_PHASE              (* (reg8 *) EMITE_IntClock__CFG3)
#define EMITE_IntClock_PHASE_PTR          ((reg8 *) EMITE_IntClock__CFG3)
#endif /* defined(EMITE_IntClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define EMITE_IntClock_CLKEN_MASK         EMITE_IntClock__PM_ACT_MSK
#define EMITE_IntClock_CLKSTBY_MASK       EMITE_IntClock__PM_STBY_MSK

/* CFG2 field masks */
#define EMITE_IntClock_SRC_SEL_MSK        EMITE_IntClock__CFG2_SRC_SEL_MASK
#define EMITE_IntClock_MODE_MASK          (~(EMITE_IntClock_SRC_SEL_MSK))

#if defined(EMITE_IntClock__CFG3)
/* CFG3 phase mask */
#define EMITE_IntClock_PHASE_MASK         EMITE_IntClock__CFG3_PHASE_DLY_MASK
#endif /* defined(EMITE_IntClock__CFG3) */

#endif /* CY_CLOCK_EMITE_IntClock_H */


/* [] END OF FILE */
