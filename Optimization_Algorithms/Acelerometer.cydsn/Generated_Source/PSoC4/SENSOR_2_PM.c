/***************************************************************************//**
* \file SENSOR_2_PM.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SENSOR_2.h"
#include "SENSOR_2_PVT.h"

#if(SENSOR_2_SCB_MODE_I2C_INC)
    #include "SENSOR_2_I2C_PVT.h"
#endif /* (SENSOR_2_SCB_MODE_I2C_INC) */

#if(SENSOR_2_SCB_MODE_EZI2C_INC)
    #include "SENSOR_2_EZI2C_PVT.h"
#endif /* (SENSOR_2_SCB_MODE_EZI2C_INC) */

#if(SENSOR_2_SCB_MODE_SPI_INC || SENSOR_2_SCB_MODE_UART_INC)
    #include "SENSOR_2_SPI_UART_PVT.h"
#endif /* (SENSOR_2_SCB_MODE_SPI_INC || SENSOR_2_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG || \
   (SENSOR_2_SCB_MODE_I2C_CONST_CFG   && (!SENSOR_2_I2C_WAKE_ENABLE_CONST))   || \
   (SENSOR_2_SCB_MODE_EZI2C_CONST_CFG && (!SENSOR_2_EZI2C_WAKE_ENABLE_CONST)) || \
   (SENSOR_2_SCB_MODE_SPI_CONST_CFG   && (!SENSOR_2_SPI_WAKE_ENABLE_CONST))   || \
   (SENSOR_2_SCB_MODE_UART_CONST_CFG  && (!SENSOR_2_UART_WAKE_ENABLE_CONST)))

    SENSOR_2_BACKUP_STRUCT SENSOR_2_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: SENSOR_2_Sleep
****************************************************************************//**
*
*  Prepares the SENSOR_2 component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the SENSOR_2_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void SENSOR_2_Sleep(void)
{
#if(SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG)

    if(SENSOR_2_SCB_WAKE_ENABLE_CHECK)
    {
        if(SENSOR_2_SCB_MODE_I2C_RUNTM_CFG)
        {
            SENSOR_2_I2CSaveConfig();
        }
        else if(SENSOR_2_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SENSOR_2_EzI2CSaveConfig();
        }
    #if(!SENSOR_2_CY_SCBIP_V1)
        else if(SENSOR_2_SCB_MODE_SPI_RUNTM_CFG)
        {
            SENSOR_2_SpiSaveConfig();
        }
        else if(SENSOR_2_SCB_MODE_UART_RUNTM_CFG)
        {
            SENSOR_2_UartSaveConfig();
        }
    #endif /* (!SENSOR_2_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        SENSOR_2_backup.enableState = (uint8) SENSOR_2_GET_CTRL_ENABLED;

        if(0u != SENSOR_2_backup.enableState)
        {
            SENSOR_2_Stop();
        }
    }

#else

    #if (SENSOR_2_SCB_MODE_I2C_CONST_CFG && SENSOR_2_I2C_WAKE_ENABLE_CONST)
        SENSOR_2_I2CSaveConfig();

    #elif (SENSOR_2_SCB_MODE_EZI2C_CONST_CFG && SENSOR_2_EZI2C_WAKE_ENABLE_CONST)
        SENSOR_2_EzI2CSaveConfig();

    #elif (SENSOR_2_SCB_MODE_SPI_CONST_CFG && SENSOR_2_SPI_WAKE_ENABLE_CONST)
        SENSOR_2_SpiSaveConfig();

    #elif (SENSOR_2_SCB_MODE_UART_CONST_CFG && SENSOR_2_UART_WAKE_ENABLE_CONST)
        SENSOR_2_UartSaveConfig();

    #else

        SENSOR_2_backup.enableState = (uint8) SENSOR_2_GET_CTRL_ENABLED;

        if(0u != SENSOR_2_backup.enableState)
        {
            SENSOR_2_Stop();
        }

    #endif /* defined (SENSOR_2_SCB_MODE_I2C_CONST_CFG) && (SENSOR_2_I2C_WAKE_ENABLE_CONST) */

#endif /* (SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SENSOR_2_Wakeup
****************************************************************************//**
*
*  Prepares the SENSOR_2 component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the SENSOR_2_Wakeup() function without first calling the 
*   SENSOR_2_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void SENSOR_2_Wakeup(void)
{
#if(SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG)

    if(SENSOR_2_SCB_WAKE_ENABLE_CHECK)
    {
        if(SENSOR_2_SCB_MODE_I2C_RUNTM_CFG)
        {
            SENSOR_2_I2CRestoreConfig();
        }
        else if(SENSOR_2_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SENSOR_2_EzI2CRestoreConfig();
        }
    #if(!SENSOR_2_CY_SCBIP_V1)
        else if(SENSOR_2_SCB_MODE_SPI_RUNTM_CFG)
        {
            SENSOR_2_SpiRestoreConfig();
        }
        else if(SENSOR_2_SCB_MODE_UART_RUNTM_CFG)
        {
            SENSOR_2_UartRestoreConfig();
        }
    #endif /* (!SENSOR_2_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != SENSOR_2_backup.enableState)
        {
            SENSOR_2_Enable();
        }
    }

#else

    #if (SENSOR_2_SCB_MODE_I2C_CONST_CFG  && SENSOR_2_I2C_WAKE_ENABLE_CONST)
        SENSOR_2_I2CRestoreConfig();

    #elif (SENSOR_2_SCB_MODE_EZI2C_CONST_CFG && SENSOR_2_EZI2C_WAKE_ENABLE_CONST)
        SENSOR_2_EzI2CRestoreConfig();

    #elif (SENSOR_2_SCB_MODE_SPI_CONST_CFG && SENSOR_2_SPI_WAKE_ENABLE_CONST)
        SENSOR_2_SpiRestoreConfig();

    #elif (SENSOR_2_SCB_MODE_UART_CONST_CFG && SENSOR_2_UART_WAKE_ENABLE_CONST)
        SENSOR_2_UartRestoreConfig();

    #else

        if(0u != SENSOR_2_backup.enableState)
        {
            SENSOR_2_Enable();
        }

    #endif /* (SENSOR_2_I2C_WAKE_ENABLE_CONST) */

#endif /* (SENSOR_2_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
