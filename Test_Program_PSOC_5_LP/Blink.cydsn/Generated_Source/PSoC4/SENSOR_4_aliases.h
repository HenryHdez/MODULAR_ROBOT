/*******************************************************************************
* File Name: SENSOR_4.h  
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

#if !defined(CY_PINS_SENSOR_4_ALIASES_H) /* Pins SENSOR_4_ALIASES_H */
#define CY_PINS_SENSOR_4_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define SENSOR_4_0			(SENSOR_4__0__PC)
#define SENSOR_4_0_PS		(SENSOR_4__0__PS)
#define SENSOR_4_0_PC		(SENSOR_4__0__PC)
#define SENSOR_4_0_DR		(SENSOR_4__0__DR)
#define SENSOR_4_0_SHIFT	(SENSOR_4__0__SHIFT)
#define SENSOR_4_0_INTR	((uint16)((uint16)0x0003u << (SENSOR_4__0__SHIFT*2u)))

#define SENSOR_4_INTR_ALL	 ((uint16)(SENSOR_4_0_INTR))


#endif /* End Pins SENSOR_4_ALIASES_H */


/* [] END OF FILE */
