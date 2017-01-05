/*******************************************************************************
* File Name: MUX.c
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

#include "MUX.h"

uint8 MUX_initVar = 0u;


/*******************************************************************************
* Function Name: MUX_Start
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
void MUX_Start(void)
{
    MUX_DisconnectAll();
    MUX_initVar = 1u;
}


/*******************************************************************************
* Function Name: MUX_Init
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
void MUX_Init(void)
{
    MUX_DisconnectAll();
}


/*******************************************************************************
* Function Name: MUX_Stop
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
void MUX_Stop(void)
{
    MUX_DisconnectAll();
}

#if (MUX_MUXTYPE == MUX_MUX_DIFF)

/*******************************************************************************
* Function Name: MUX_Next
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
void MUX_Next(void)
{
    MUX_CYAMUXSIDE_A_Next();
    MUX_CYAMUXSIDE_B_Next();
}


/*******************************************************************************
* Function Name: MUX_DisconnectAll
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
void MUX_DisconnectAll(void)
{
    MUX_CYAMUXSIDE_A_DisconnectAll();
    MUX_CYAMUXSIDE_B_DisconnectAll();
}


/*******************************************************************************
* Function Name: MUX_GetChannel
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
int8 MUX_GetChannel(void)
{
    return MUX_CYAMUXSIDE_A_curChannel;
}

#else

/*******************************************************************************
* Function Name: MUX_GetChannel
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
int8 MUX_GetChannel(void)
{
    return MUX_curChannel;
}

#endif


/* [] END OF FILE */
