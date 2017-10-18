/***************************************************************************//**
* \file SENSOR_3_PM.c
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

#include "SENSOR_3.h"
#include "SENSOR_3_PVT.h"

#if(SENSOR_3_SCB_MODE_I2C_INC)
    #include "SENSOR_3_I2C_PVT.h"
#endif /* (SENSOR_3_SCB_MODE_I2C_INC) */

#if(SENSOR_3_SCB_MODE_EZI2C_INC)
    #include "SENSOR_3_EZI2C_PVT.h"
#endif /* (SENSOR_3_SCB_MODE_EZI2C_INC) */

#if(SENSOR_3_SCB_MODE_SPI_INC || SENSOR_3_SCB_MODE_UART_INC)
    #include "SENSOR_3_SPI_UART_PVT.h"
#endif /* (SENSOR_3_SCB_MODE_SPI_INC || SENSOR_3_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG || \
   (SENSOR_3_SCB_MODE_I2C_CONST_CFG   && (!SENSOR_3_I2C_WAKE_ENABLE_CONST))   || \
   (SENSOR_3_SCB_MODE_EZI2C_CONST_CFG && (!SENSOR_3_EZI2C_WAKE_ENABLE_CONST)) || \
   (SENSOR_3_SCB_MODE_SPI_CONST_CFG   && (!SENSOR_3_SPI_WAKE_ENABLE_CONST))   || \
   (SENSOR_3_SCB_MODE_UART_CONST_CFG  && (!SENSOR_3_UART_WAKE_ENABLE_CONST)))

    SENSOR_3_BACKUP_STRUCT SENSOR_3_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: SENSOR_3_Sleep
****************************************************************************//**
*
*  Prepares the SENSOR_3 component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the SENSOR_3_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void SENSOR_3_Sleep(void)
{
#if(SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)

    if(SENSOR_3_SCB_WAKE_ENABLE_CHECK)
    {
        if(SENSOR_3_SCB_MODE_I2C_RUNTM_CFG)
        {
            SENSOR_3_I2CSaveConfig();
        }
        else if(SENSOR_3_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SENSOR_3_EzI2CSaveConfig();
        }
    #if(!SENSOR_3_CY_SCBIP_V1)
        else if(SENSOR_3_SCB_MODE_SPI_RUNTM_CFG)
        {
            SENSOR_3_SpiSaveConfig();
        }
        else if(SENSOR_3_SCB_MODE_UART_RUNTM_CFG)
        {
            SENSOR_3_UartSaveConfig();
        }
    #endif /* (!SENSOR_3_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        SENSOR_3_backup.enableState = (uint8) SENSOR_3_GET_CTRL_ENABLED;

        if(0u != SENSOR_3_backup.enableState)
        {
            SENSOR_3_Stop();
        }
    }

#else

    #if (SENSOR_3_SCB_MODE_I2C_CONST_CFG && SENSOR_3_I2C_WAKE_ENABLE_CONST)
        SENSOR_3_I2CSaveConfig();

    #elif (SENSOR_3_SCB_MODE_EZI2C_CONST_CFG && SENSOR_3_EZI2C_WAKE_ENABLE_CONST)
        SENSOR_3_EzI2CSaveConfig();

    #elif (SENSOR_3_SCB_MODE_SPI_CONST_CFG && SENSOR_3_SPI_WAKE_ENABLE_CONST)
        SENSOR_3_SpiSaveConfig();

    #elif (SENSOR_3_SCB_MODE_UART_CONST_CFG && SENSOR_3_UART_WAKE_ENABLE_CONST)
        SENSOR_3_UartSaveConfig();

    #else

        SENSOR_3_backup.enableState = (uint8) SENSOR_3_GET_CTRL_ENABLED;

        if(0u != SENSOR_3_backup.enableState)
        {
            SENSOR_3_Stop();
        }

    #endif /* defined (SENSOR_3_SCB_MODE_I2C_CONST_CFG) && (SENSOR_3_I2C_WAKE_ENABLE_CONST) */

#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SENSOR_3_Wakeup
****************************************************************************//**
*
*  Prepares the SENSOR_3 component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the SENSOR_3_Wakeup() function without first calling the 
*   SENSOR_3_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void SENSOR_3_Wakeup(void)
{
#if(SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)

    if(SENSOR_3_SCB_WAKE_ENABLE_CHECK)
    {
        if(SENSOR_3_SCB_MODE_I2C_RUNTM_CFG)
        {
            SENSOR_3_I2CRestoreConfig();
        }
        else if(SENSOR_3_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SENSOR_3_EzI2CRestoreConfig();
        }
    #if(!SENSOR_3_CY_SCBIP_V1)
        else if(SENSOR_3_SCB_MODE_SPI_RUNTM_CFG)
        {
            SENSOR_3_SpiRestoreConfig();
        }
        else if(SENSOR_3_SCB_MODE_UART_RUNTM_CFG)
        {
            SENSOR_3_UartRestoreConfig();
        }
    #endif /* (!SENSOR_3_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != SENSOR_3_backup.enableState)
        {
            SENSOR_3_Enable();
        }
    }

#else

    #if (SENSOR_3_SCB_MODE_I2C_CONST_CFG  && SENSOR_3_I2C_WAKE_ENABLE_CONST)
        SENSOR_3_I2CRestoreConfig();

    #elif (SENSOR_3_SCB_MODE_EZI2C_CONST_CFG && SENSOR_3_EZI2C_WAKE_ENABLE_CONST)
        SENSOR_3_EzI2CRestoreConfig();

    #elif (SENSOR_3_SCB_MODE_SPI_CONST_CFG && SENSOR_3_SPI_WAKE_ENABLE_CONST)
        SENSOR_3_SpiRestoreConfig();

    #elif (SENSOR_3_SCB_MODE_UART_CONST_CFG && SENSOR_3_UART_WAKE_ENABLE_CONST)
        SENSOR_3_UartRestoreConfig();

    #else

        if(0u != SENSOR_3_backup.enableState)
        {
            SENSOR_3_Enable();
        }

    #endif /* (SENSOR_3_I2C_WAKE_ENABLE_CONST) */

#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
