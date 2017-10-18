/*******************************************************************************
* File Name: RESET_XBEE.h  
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

#if !defined(CY_PINS_RESET_XBEE_ALIASES_H) /* Pins RESET_XBEE_ALIASES_H */
#define CY_PINS_RESET_XBEE_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define RESET_XBEE_0			(RESET_XBEE__0__PC)
#define RESET_XBEE_0_PS		(RESET_XBEE__0__PS)
#define RESET_XBEE_0_PC		(RESET_XBEE__0__PC)
#define RESET_XBEE_0_DR		(RESET_XBEE__0__DR)
#define RESET_XBEE_0_SHIFT	(RESET_XBEE__0__SHIFT)
#define RESET_XBEE_0_INTR	((uint16)((uint16)0x0003u << (RESET_XBEE__0__SHIFT*2u)))

#define RESET_XBEE_INTR_ALL	 ((uint16)(RESET_XBEE_0_INTR))


#endif /* End Pins RESET_XBEE_ALIASES_H */


/* [] END OF FILE */
