/*******************************************************************************
* File Name: X.h  
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

#if !defined(CY_PINS_X_ALIASES_H) /* Pins X_ALIASES_H */
#define CY_PINS_X_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define X_0			(X__0__PC)
#define X_0_PS		(X__0__PS)
#define X_0_PC		(X__0__PC)
#define X_0_DR		(X__0__DR)
#define X_0_SHIFT	(X__0__SHIFT)
#define X_0_INTR	((uint16)((uint16)0x0003u << (X__0__SHIFT*2u)))

#define X_INTR_ALL	 ((uint16)(X_0_INTR))


#endif /* End Pins X_ALIASES_H */


/* [] END OF FILE */
