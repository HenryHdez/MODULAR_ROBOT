/*******************************************************************************
* File Name: AMUX.c
* Version 1.80
*
*  Description:
*    This file contains functions for the AMuxSeq.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "AMUX.h"

uint8 AMUX_initVar = 0u;


/*******************************************************************************
* Function Name: AMUX_Start
********************************************************************************
* Summary:
*  Disconnect all channels. The next time Next is called, channel 0 will be
*  connected.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMUX_Start(void)
{
    AMUX_DisconnectAll();
    AMUX_initVar = 1u;
}


/*******************************************************************************
* Function Name: AMUX_Init
********************************************************************************
* Summary:
*  Disconnect all channels. The next time Next is called, channel 0 will be
*  connected.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMUX_Init(void)
{
    AMUX_DisconnectAll();
}


/*******************************************************************************
* Function Name: AMUX_Stop
********************************************************************************
* Summary:
*  Disconnect all channels. The next time Next is called, channel 0 will be
*  connected.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMUX_Stop(void)
{
    AMUX_DisconnectAll();
}

#if (AMUX_MUXTYPE == AMUX_MUX_DIFF)

/*******************************************************************************
* Function Name: AMUX_Next
********************************************************************************
* Summary:
*  Disconnects the previous channel and connects the next one in the sequence.
*  When Next is called for the first time after Init, Start, Enable, Stop, or
*  DisconnectAll, it connects channel 0.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMUX_Next(void)
{
    AMUX_CYAMUXSIDE_A_Next();
    AMUX_CYAMUXSIDE_B_Next();
}


/*******************************************************************************
* Function Name: AMUX_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels. The next time Next is called, channel
*  0 will be connected.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMUX_DisconnectAll(void)
{
    AMUX_CYAMUXSIDE_A_DisconnectAll();
    AMUX_CYAMUXSIDE_B_DisconnectAll();
}


/*******************************************************************************
* Function Name: AMUX_GetChannel
********************************************************************************
* Summary:
*  The currently connected channel is retuned. If no channel is connected
*  returns -1.
*
* Parameters:
*  void
*
* Return:
*  The current channel or -1.
*
*******************************************************************************/
int8 AMUX_GetChannel(void)
{
    return AMUX_CYAMUXSIDE_A_curChannel;
}

#else

/*******************************************************************************
* Function Name: AMUX_GetChannel
********************************************************************************
* Summary:
*  The currently connected channel is retuned. If no channel is connected
*  returns -1.
*
* Parameters:
*  void
*
* Return:
*  The current channel or -1.
*
*******************************************************************************/
int8 AMUX_GetChannel(void)
{
    return AMUX_curChannel;
}

#endif


/* [] END OF FILE */
