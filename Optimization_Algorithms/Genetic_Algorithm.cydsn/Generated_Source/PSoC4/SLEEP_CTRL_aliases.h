/*******************************************************************************
* File Name: SLEEP_CTRL.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_SLEEP_CTRL_ALIASES_H) /* Pins SLEEP_CTRL_ALIASES_H */
#define CY_PINS_SLEEP_CTRL_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define SLEEP_CTRL_0			(SLEEP_CTRL__0__PC)
#define SLEEP_CTRL_0_PS		(SLEEP_CTRL__0__PS)
#define SLEEP_CTRL_0_PC		(SLEEP_CTRL__0__PC)
#define SLEEP_CTRL_0_DR		(SLEEP_CTRL__0__DR)
#define SLEEP_CTRL_0_SHIFT	(SLEEP_CTRL__0__SHIFT)
#define SLEEP_CTRL_0_INTR	((uint16)((uint16)0x0003u << (SLEEP_CTRL__0__SHIFT*2u)))

#define SLEEP_CTRL_INTR_ALL	 ((uint16)(SLEEP_CTRL_0_INTR))


#endif /* End Pins SLEEP_CTRL_ALIASES_H */


/* [] END OF FILE */
