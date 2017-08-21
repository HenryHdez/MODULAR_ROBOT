/***************************************************************************//**
* \file SENSOR_4_I2C.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  I2C mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SENSOR_4_PVT.h"
#include "SENSOR_4_I2C_PVT.h"


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 SENSOR_4_state;  /* Current state of I2C FSM */

#if(SENSOR_4_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const SENSOR_4_I2C_INIT_STRUCT SENSOR_4_configI2C =
    {
        SENSOR_4_I2C_MODE,
        SENSOR_4_I2C_OVS_FACTOR_LOW,
        SENSOR_4_I2C_OVS_FACTOR_HIGH,
        SENSOR_4_I2C_MEDIAN_FILTER_ENABLE,
        SENSOR_4_I2C_SLAVE_ADDRESS,
        SENSOR_4_I2C_SLAVE_ADDRESS_MASK,
        SENSOR_4_I2C_ACCEPT_ADDRESS,
        SENSOR_4_I2C_WAKE_ENABLE,
        SENSOR_4_I2C_BYTE_MODE_ENABLE,
        SENSOR_4_I2C_DATA_RATE,
        SENSOR_4_I2C_ACCEPT_GENERAL_CALL,
    };

    /*******************************************************************************
    * Function Name: SENSOR_4_I2CInit
    ****************************************************************************//**
    *
    *
    *  Configures the SENSOR_4 for I2C operation.
    *
    *  This function is intended specifically to be used when the SENSOR_4 
    *  configuration is set to “Unconfigured SENSOR_4” in the customizer. 
    *  After initializing the SENSOR_4 in I2C mode using this function, 
    *  the component can be enabled using the SENSOR_4_Start() or 
    * SENSOR_4_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void SENSOR_4_I2CInit(const SENSOR_4_I2C_INIT_STRUCT *config)
    {
        uint32 medianFilter;
        uint32 locEnableWake;

        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            SENSOR_4_SetPins(SENSOR_4_SCB_MODE_I2C, SENSOR_4_DUMMY_PARAM,
                                     SENSOR_4_DUMMY_PARAM);

            /* Store internal configuration */
            SENSOR_4_scbMode       = (uint8) SENSOR_4_SCB_MODE_I2C;
            SENSOR_4_scbEnableWake = (uint8) config->enableWake;
            SENSOR_4_scbEnableIntr = (uint8) SENSOR_4_SCB_IRQ_INTERNAL;

            SENSOR_4_mode          = (uint8) config->mode;
            SENSOR_4_acceptAddr    = (uint8) config->acceptAddr;

        #if (SENSOR_4_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            SENSOR_4_SET_I2C_CFG_SDA_FILT_TRIM(SENSOR_4_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (SENSOR_4_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((SENSOR_4_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= SENSOR_4_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (SENSOR_4_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                SENSOR_4_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = SENSOR_4_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                SENSOR_4_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = SENSOR_4_DIGITAL_FILTER_ENABLE;
            }

        #if (!SENSOR_4_CY_SCBIP_V0)
            locEnableWake = (SENSOR_4_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!SENSOR_4_CY_SCBIP_V0) */

            /* Configure I2C interface */
            SENSOR_4_CTRL_REG     = SENSOR_4_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            SENSOR_4_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            SENSOR_4_GET_CTRL_EC_AM_MODE (locEnableWake);

            SENSOR_4_I2C_CTRL_REG = SENSOR_4_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                    SENSOR_4_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)                          |
                    SENSOR_4_GET_I2C_CTRL_S_GENERAL_IGNORE((uint32)(0u == config->acceptGeneralAddr))    |
                    SENSOR_4_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode);

            /* Configure RX direction */
            SENSOR_4_RX_CTRL_REG      = SENSOR_4_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                SENSOR_4_I2C_RX_CTRL;
            SENSOR_4_RX_FIFO_CTRL_REG = SENSOR_4_CLEAR_REG;

            /* Set default address and mask */
            SENSOR_4_RX_MATCH_REG    = ((SENSOR_4_I2C_SLAVE) ?
                                                (SENSOR_4_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 SENSOR_4_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (SENSOR_4_CLEAR_REG));


            /* Configure TX direction */
            SENSOR_4_TX_CTRL_REG      = SENSOR_4_I2C_TX_CTRL;
            SENSOR_4_TX_FIFO_CTRL_REG = SENSOR_4_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (SENSOR_4_ISR_NUMBER);
            CyIntSetPriority(SENSOR_4_ISR_NUMBER, SENSOR_4_ISR_PRIORITY);
            (void) CyIntSetVector(SENSOR_4_ISR_NUMBER, &SENSOR_4_I2C_ISR);

            /* Configure interrupt sources */
        #if(!SENSOR_4_CY_SCBIP_V1)
            SENSOR_4_INTR_SPI_EC_MASK_REG = SENSOR_4_NO_INTR_SOURCES;
        #endif /* (!SENSOR_4_CY_SCBIP_V1) */

            SENSOR_4_INTR_I2C_EC_MASK_REG = SENSOR_4_NO_INTR_SOURCES;
            SENSOR_4_INTR_RX_MASK_REG     = SENSOR_4_NO_INTR_SOURCES;
            SENSOR_4_INTR_TX_MASK_REG     = SENSOR_4_NO_INTR_SOURCES;

            SENSOR_4_INTR_SLAVE_MASK_REG  = ((SENSOR_4_I2C_SLAVE) ?
                            (SENSOR_4_GET_INTR_SLAVE_I2C_GENERAL(config->acceptGeneralAddr) |
                             SENSOR_4_I2C_INTR_SLAVE_MASK) : (SENSOR_4_CLEAR_REG));

            SENSOR_4_INTR_MASTER_MASK_REG = ((SENSOR_4_I2C_MASTER) ?
                                                     (SENSOR_4_I2C_INTR_MASTER_MASK) :
                                                     (SENSOR_4_CLEAR_REG));

            /* Configure global variables */
            SENSOR_4_state = SENSOR_4_I2C_FSM_IDLE;

            /* Internal slave variables */
            SENSOR_4_slStatus        = 0u;
            SENSOR_4_slRdBufIndex    = 0u;
            SENSOR_4_slWrBufIndex    = 0u;
            SENSOR_4_slOverFlowCount = 0u;

            /* Internal master variables */
            SENSOR_4_mstrStatus     = 0u;
            SENSOR_4_mstrRdBufIndex = 0u;
            SENSOR_4_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: SENSOR_4_I2CInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the I2C operation.
    *
    *******************************************************************************/
    void SENSOR_4_I2CInit(void)
    {
    #if(SENSOR_4_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        SENSOR_4_SET_I2C_CFG_SDA_FILT_TRIM(SENSOR_4_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (SENSOR_4_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        SENSOR_4_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        SENSOR_4_CTRL_REG     = SENSOR_4_I2C_DEFAULT_CTRL;
        SENSOR_4_I2C_CTRL_REG = SENSOR_4_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        SENSOR_4_RX_CTRL_REG      = SENSOR_4_I2C_DEFAULT_RX_CTRL;
        SENSOR_4_RX_FIFO_CTRL_REG = SENSOR_4_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        SENSOR_4_RX_MATCH_REG     = SENSOR_4_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        SENSOR_4_TX_CTRL_REG      = SENSOR_4_I2C_DEFAULT_TX_CTRL;
        SENSOR_4_TX_FIFO_CTRL_REG = SENSOR_4_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (SENSOR_4_ISR_NUMBER);
        CyIntSetPriority(SENSOR_4_ISR_NUMBER, SENSOR_4_ISR_PRIORITY);
    #if(!SENSOR_4_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(SENSOR_4_ISR_NUMBER, &SENSOR_4_I2C_ISR);
    #endif /* (SENSOR_4_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!SENSOR_4_CY_SCBIP_V1)
        SENSOR_4_INTR_SPI_EC_MASK_REG = SENSOR_4_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!SENSOR_4_CY_SCBIP_V1) */

        SENSOR_4_INTR_I2C_EC_MASK_REG = SENSOR_4_I2C_DEFAULT_INTR_I2C_EC_MASK;
        SENSOR_4_INTR_SLAVE_MASK_REG  = SENSOR_4_I2C_DEFAULT_INTR_SLAVE_MASK;
        SENSOR_4_INTR_MASTER_MASK_REG = SENSOR_4_I2C_DEFAULT_INTR_MASTER_MASK;
        SENSOR_4_INTR_RX_MASK_REG     = SENSOR_4_I2C_DEFAULT_INTR_RX_MASK;
        SENSOR_4_INTR_TX_MASK_REG     = SENSOR_4_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        SENSOR_4_state = SENSOR_4_I2C_FSM_IDLE;

    #if(SENSOR_4_I2C_SLAVE)
        /* Internal slave variable */
        SENSOR_4_slStatus        = 0u;
        SENSOR_4_slRdBufIndex    = 0u;
        SENSOR_4_slWrBufIndex    = 0u;
        SENSOR_4_slOverFlowCount = 0u;
    #endif /* (SENSOR_4_I2C_SLAVE) */

    #if(SENSOR_4_I2C_MASTER)
    /* Internal master variable */
        SENSOR_4_mstrStatus     = 0u;
        SENSOR_4_mstrRdBufIndex = 0u;
        SENSOR_4_mstrWrBufIndex = 0u;
    #endif /* (SENSOR_4_I2C_MASTER) */
    }
#endif /* (SENSOR_4_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: SENSOR_4_I2CStop
****************************************************************************//**
*
*  Resets the I2C FSM into the default state.
*
*******************************************************************************/
void SENSOR_4_I2CStop(void)
{
    SENSOR_4_state = SENSOR_4_I2C_FSM_IDLE;
}


#if(SENSOR_4_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: SENSOR_4_I2CSaveConfig
    ****************************************************************************//**
    *
    *  Enables SENSOR_4_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void SENSOR_4_I2CSaveConfig(void)
    {
    #if (!SENSOR_4_CY_SCBIP_V0)
        #if (SENSOR_4_I2C_MULTI_MASTER_SLAVE_CONST && SENSOR_4_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (SENSOR_4_CTRL_REG & SENSOR_4_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                SENSOR_4_Stop();
                SENSOR_4_CTRL_REG |= SENSOR_4_CTRL_EC_AM_MODE;
                SENSOR_4_Enable();
            }
        #endif /* (SENSOR_4_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (SENSOR_4_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            SENSOR_4_SCBCLK_Stop();
        #endif /* (SENSOR_4_SCB_CLK_INTERNAL) */
    #endif /* (!SENSOR_4_CY_SCBIP_V0) */

        SENSOR_4_SetI2CExtClkInterruptMode(SENSOR_4_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: SENSOR_4_I2CRestoreConfig
    ****************************************************************************//**
    *
    *  Disables SENSOR_4_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void SENSOR_4_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        SENSOR_4_SetI2CExtClkInterruptMode(SENSOR_4_NO_INTR_SOURCES);

    #if (!SENSOR_4_CY_SCBIP_V0)
        #if (SENSOR_4_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            SENSOR_4_SCBCLK_Start();
        #endif /* (SENSOR_4_SCB_CLK_INTERNAL) */
    #endif /* (!SENSOR_4_CY_SCBIP_V0) */
    }
#endif /* (SENSOR_4_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
