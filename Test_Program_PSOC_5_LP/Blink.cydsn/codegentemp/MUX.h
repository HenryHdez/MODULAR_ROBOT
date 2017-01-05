/*******************************************************************************
* File Name: MUX.h
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

#if !defined(CY_AMUXSEQ_MUX_H)
#define CY_AMUXSEQ_MUX_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define MUX_MUX_SINGLE 1
#define MUX_MUX_DIFF   2
#define MUX_MUXTYPE    1


/***************************************
*        Function Prototypes
***************************************/

void MUX_Start(void);
void MUX_Init(void);
void MUX_Stop(void);
#if (MUX_MUXTYPE == MUX_MUX_DIFF)
void MUX_Next(void);
void MUX_DisconnectAll(void);
#else
/* The Next and DisconnectAll functions are declared in cyfitter_cfg.h. */
/* void MUX_Next(void); */
/* void MUX_DisconnectAll(void); */
#endif
int8 MUX_GetChannel(void);


/***************************************
*           Global Variables
***************************************/

extern uint8 MUX_initVar;


/***************************************
*         Parameter Constants
***************************************/
#define MUX_CHANNELS 4


#endif /* CY_AMUXSEQ_MUX_H */


/* [] END OF FILE */
