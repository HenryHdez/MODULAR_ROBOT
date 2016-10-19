/*******************************************************************************
* File Name: AMUX.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the AMuxSeq.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUXSEQ_AMUX_H)
#define CY_AMUXSEQ_AMUX_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define AMUX_MUX_SINGLE 1
#define AMUX_MUX_DIFF   2
#define AMUX_MUXTYPE    1


/***************************************
*        Function Prototypes
***************************************/

void AMUX_Start(void);
void AMUX_Init(void);
void AMUX_Stop(void);
#if (AMUX_MUXTYPE == AMUX_MUX_DIFF)
void AMUX_Next(void);
void AMUX_DisconnectAll(void);
#else
/* The Next and DisconnectAll functions are declared in cyfitter_cfg.h. */
/* void AMUX_Next(void); */
/* void AMUX_DisconnectAll(void); */
#endif
int8 AMUX_GetChannel(void);


/***************************************
*           Global Variables
***************************************/

extern uint8 AMUX_initVar;


/***************************************
*         Parameter Constants
***************************************/
#define AMUX_CHANNELS 4


#endif /* CY_AMUXSEQ_AMUX_H */


/* [] END OF FILE */
