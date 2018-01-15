/***************************************************************************//**
* \file SENSOR_3_I2C.c
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

#include "SENSOR_3_PVT.h"
#include "SENSOR_3_I2C_PVT.h"


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 SENSOR_3_state;  /* Current state of I2C FSM */

#if(SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const SENSOR_3_I2C_INIT_STRUCT SENSOR_3_configI2C =
    {
        SENSOR_3_I2C_MODE,
        SENSOR_3_I2C_OVS_FACTOR_LOW,
        SENSOR_3_I2C_OVS_FACTOR_HIGH,
        SENSOR_3_I2C_MEDIAN_FILTER_ENABLE,
        SENSOR_3_I2C_SLAVE_ADDRESS,
        SENSOR_3_I2C_SLAVE_ADDRESS_MASK,
        SENSOR_3_I2C_ACCEPT_ADDRESS,
        SENSOR_3_I2C_WAKE_ENABLE,
        SENSOR_3_I2C_BYTE_MODE_ENABLE,
        SENSOR_3_I2C_DATA_RATE,
        SENSOR_3_I2C_ACCEPT_GENERAL_CALL,
    };

    /*******************************************************************************
    * Function Name: SENSOR_3_I2CInit
    ****************************************************************************//**
    *
    *
    *  Configures the SENSOR_3 for I2C operation.
    *
    *  This function is intended specifically to be used when the SENSOR_3 
    *  configuration is set to “Unconfigured SENSOR_3” in the customizer. 
    *  After initializing the SENSOR_3 in I2C mode using this function, 
    *  the component can be enabled using the SENSOR_3_Start() or 
    * SENSOR_3_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void SENSOR_3_I2CInit(const SENSOR_3_I2C_INIT_STRUCT *config)
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
            SENSOR_3_SetPins(SENSOR_3_SCB_MODE_I2C, SENSOR_3_DUMMY_PARAM,
                                     SENSOR_3_DUMMY_PARAM);

            /* Store internal configuration */
            SENSOR_3_scbMode       = (uint8) SENSOR_3_SCB_MODE_I2C;
            SENSOR_3_scbEnableWake = (uint8) config->enableWake;
            SENSOR_3_scbEnableIntr = (uint8) SENSOR_3_SCB_IRQ_INTERNAL;

            SENSOR_3_mode          = (uint8) config->mode;
            SENSOR_3_acceptAddr    = (uint8) config->acceptAddr;

        #if (SENSOR_3_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            SENSOR_3_SET_I2C_CFG_SDA_FILT_TRIM(SENSOR_3_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (SENSOR_3_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((SENSOR_3_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= SENSOR_3_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (SENSOR_3_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                SENSOR_3_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = SENSOR_3_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                SENSOR_3_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = SENSOR_3_DIGITAL_FILTER_ENABLE;
            }

        #if (!SENSOR_3_CY_SCBIP_V0)
            locEnableWake = (SENSOR_3_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!SENSOR_3_CY_SCBIP_V0) */

            /* Configure I2C interface */
            SENSOR_3_CTRL_REG     = SENSOR_3_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            SENSOR_3_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            SENSOR_3_GET_CTRL_EC_AM_MODE (locEnableWake);

            SENSOR_3_I2C_CTRL_REG = SENSOR_3_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                    SENSOR_3_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)                          |
                    SENSOR_3_GET_I2C_CTRL_S_GENERAL_IGNORE((uint32)(0u == config->acceptGeneralAddr))    |
                    SENSOR_3_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode);

            /* Configure RX direction */
            SENSOR_3_RX_CTRL_REG      = SENSOR_3_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                SENSOR_3_I2C_RX_CTRL;
            SENSOR_3_RX_FIFO_CTRL_REG = SENSOR_3_CLEAR_REG;

            /* Set default address and mask */
            SENSOR_3_RX_MATCH_REG    = ((SENSOR_3_I2C_SLAVE) ?
                                                (SENSOR_3_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 SENSOR_3_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (SENSOR_3_CLEAR_REG));


            /* Configure TX direction */
            SENSOR_3_TX_CTRL_REG      = SENSOR_3_I2C_TX_CTRL;
            SENSOR_3_TX_FIFO_CTRL_REG = SENSOR_3_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (SENSOR_3_ISR_NUMBER);
            CyIntSetPriority(SENSOR_3_ISR_NUMBER, SENSOR_3_ISR_PRIORITY);
            (void) CyIntSetVector(SENSOR_3_ISR_NUMBER, &SENSOR_3_I2C_ISR);

            /* Configure interrupt sources */
        #if(!SENSOR_3_CY_SCBIP_V1)
            SENSOR_3_INTR_SPI_EC_MASK_REG = SENSOR_3_NO_INTR_SOURCES;
        #endif /* (!SENSOR_3_CY_SCBIP_V1) */

            SENSOR_3_INTR_I2C_EC_MASK_REG = SENSOR_3_NO_INTR_SOURCES;
            SENSOR_3_INTR_RX_MASK_REG     = SENSOR_3_NO_INTR_SOURCES;
            SENSOR_3_INTR_TX_MASK_REG     = SENSOR_3_NO_INTR_SOURCES;

            SENSOR_3_INTR_SLAVE_MASK_REG  = ((SENSOR_3_I2C_SLAVE) ?
                            (SENSOR_3_GET_INTR_SLAVE_I2C_GENERAL(config->acceptGeneralAddr) |
                             SENSOR_3_I2C_INTR_SLAVE_MASK) : (SENSOR_3_CLEAR_REG));

            SENSOR_3_INTR_MASTER_MASK_REG = ((SENSOR_3_I2C_MASTER) ?
                                                     (SENSOR_3_I2C_INTR_MASTER_MASK) :
                                                     (SENSOR_3_CLEAR_REG));

            /* Configure global variables */
            SENSOR_3_state = SENSOR_3_I2C_FSM_IDLE;

            /* Internal slave variables */
            SENSOR_3_slStatus        = 0u;
            SENSOR_3_slRdBufIndex    = 0u;
            SENSOR_3_slWrBufIndex    = 0u;
            SENSOR_3_slOverFlowCount = 0u;

            /* Internal master variables */
            SENSOR_3_mstrStatus     = 0u;
            SENSOR_3_mstrRdBufIndex = 0u;
            SENSOR_3_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: SENSOR_3_I2CInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the I2C operation.
    *
    *******************************************************************************/
    void SENSOR_3_I2CInit(void)
    {
    #if(SENSOR_3_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        SENSOR_3_SET_I2C_CFG_SDA_FILT_TRIM(SENSOR_3_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (SENSOR_3_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        SENSOR_3_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        SENSOR_3_CTRL_REG     = SENSOR_3_I2C_DEFAULT_CTRL;
        SENSOR_3_I2C_CTRL_REG = SENSOR_3_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        SENSOR_3_RX_CTRL_REG      = SENSOR_3_I2C_DEFAULT_RX_CTRL;
        SENSOR_3_RX_FIFO_CTRL_REG = SENSOR_3_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        SENSOR_3_RX_MATCH_REG     = SENSOR_3_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        SENSOR_3_TX_CTRL_REG      = SENSOR_3_I2C_DEFAULT_TX_CTRL;
        SENSOR_3_TX_FIFO_CTRL_REG = SENSOR_3_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (SENSOR_3_ISR_NUMBER);
        CyIntSetPriority(SENSOR_3_ISR_NUMBER, SENSOR_3_ISR_PRIORITY);
    #if(!SENSOR_3_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(SENSOR_3_ISR_NUMBER, &SENSOR_3_I2C_ISR);
    #endif /* (SENSOR_3_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!SENSOR_3_CY_SCBIP_V1)
        SENSOR_3_INTR_SPI_EC_MASK_REG = SENSOR_3_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!SENSOR_3_CY_SCBIP_V1) */

        SENSOR_3_INTR_I2C_EC_MASK_REG = SENSOR_3_I2C_DEFAULT_INTR_I2C_EC_MASK;
        SENSOR_3_INTR_SLAVE_MASK_REG  = SENSOR_3_I2C_DEFAULT_INTR_SLAVE_MASK;
        SENSOR_3_INTR_MASTER_MASK_REG = SENSOR_3_I2C_DEFAULT_INTR_MASTER_MASK;
        SENSOR_3_INTR_RX_MASK_REG     = SENSOR_3_I2C_DEFAULT_INTR_RX_MASK;
        SENSOR_3_INTR_TX_MASK_REG     = SENSOR_3_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        SENSOR_3_state = SENSOR_3_I2C_FSM_IDLE;

    #if(SENSOR_3_I2C_SLAVE)
        /* Internal slave variable */
        SENSOR_3_slStatus        = 0u;
        SENSOR_3_slRdBufIndex    = 0u;
        SENSOR_3_slWrBufIndex    = 0u;
        SENSOR_3_slOverFlowCount = 0u;
    #endif /* (SENSOR_3_I2C_SLAVE) */

    #if(SENSOR_3_I2C_MASTER)
    /* Internal master variable */
        SENSOR_3_mstrStatus     = 0u;
        SENSOR_3_mstrRdBufIndex = 0u;
        SENSOR_3_mstrWrBufIndex = 0u;
    #endif /* (SENSOR_3_I2C_MASTER) */
    }
#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: SENSOR_3_I2CStop
****************************************************************************//**
*
*  Resets the I2C FSM into the default state.
*
*******************************************************************************/
void SENSOR_3_I2CStop(void)
{
    SENSOR_3_state = SENSOR_3_I2C_FSM_IDLE;
}


#if(SENSOR_3_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: SENSOR_3_I2CSaveConfig
    ****************************************************************************//**
    *
    *  Enables SENSOR_3_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void SENSOR_3_I2CSaveConfig(void)
    {
    #if (!SENSOR_3_CY_SCBIP_V0)
        #if (SENSOR_3_I2C_MULTI_MASTER_SLAVE_CONST && SENSOR_3_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (SENSOR_3_CTRL_REG & SENSOR_3_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                SENSOR_3_Stop();
                SENSOR_3_CTRL_REG |= SENSOR_3_CTRL_EC_AM_MODE;
                SENSOR_3_Enable();
            }
        #endif /* (SENSOR_3_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (SENSOR_3_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            SENSOR_3_SCBCLK_Stop();
        #endif /* (SENSOR_3_SCB_CLK_INTERNAL) */
    #endif /* (!SENSOR_3_CY_SCBIP_V0) */

        SENSOR_3_SetI2CExtClkInterruptMode(SENSOR_3_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: SENSOR_3_I2CRestoreConfig
    ****************************************************************************//**
    *
    *  Disables SENSOR_3_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void SENSOR_3_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        SENSOR_3_SetI2CExtClkInterruptMode(SENSOR_3_NO_INTR_SOURCES);

    #if (!SENSOR_3_CY_SCBIP_V0)
        #if (SENSOR_3_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            SENSOR_3_SCBCLK_Start();
        #endif /* (SENSOR_3_SCB_CLK_INTERNAL) */
    #endif /* (!SENSOR_3_CY_SCBIP_V0) */
    }
#endif /* (SENSOR_3_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
