/*******************************************************************************
* File Name: FUERZA_XBEE.h  
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

#if !defined(CY_PINS_FUERZA_XBEE_ALIASES_H) /* Pins FUERZA_XBEE_ALIASES_H */
#define CY_PINS_FUERZA_XBEE_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define FUERZA_XBEE_0			(FUERZA_XBEE__0__PC)
#define FUERZA_XBEE_0_PS		(FUERZA_XBEE__0__PS)
#define FUERZA_XBEE_0_PC		(FUERZA_XBEE__0__PC)
#define FUERZA_XBEE_0_DR		(FUERZA_XBEE__0__DR)
#define FUERZA_XBEE_0_SHIFT	(FUERZA_XBEE__0__SHIFT)
#define FUERZA_XBEE_0_INTR	((uint16)((uint16)0x0003u << (FUERZA_XBEE__0__SHIFT*2u)))

#define FUERZA_XBEE_INTR_ALL	 ((uint16)(FUERZA_XBEE_0_INTR))


#endif /* End Pins FUERZA_XBEE_ALIASES_H */


/* [] END OF FILE */
