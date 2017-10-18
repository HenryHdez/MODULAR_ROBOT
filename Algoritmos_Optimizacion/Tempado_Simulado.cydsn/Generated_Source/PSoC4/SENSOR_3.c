/***************************************************************************//**
* \file SENSOR_3.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component.
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

#if (SENSOR_3_SCB_MODE_I2C_INC)
    #include "SENSOR_3_I2C_PVT.h"
#endif /* (SENSOR_3_SCB_MODE_I2C_INC) */

#if (SENSOR_3_SCB_MODE_EZI2C_INC)
    #include "SENSOR_3_EZI2C_PVT.h"
#endif /* (SENSOR_3_SCB_MODE_EZI2C_INC) */

#if (SENSOR_3_SCB_MODE_SPI_INC || SENSOR_3_SCB_MODE_UART_INC)
    #include "SENSOR_3_SPI_UART_PVT.h"
#endif /* (SENSOR_3_SCB_MODE_SPI_INC || SENSOR_3_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 SENSOR_3_scbMode = SENSOR_3_SCB_MODE_UNCONFIG;
    uint8 SENSOR_3_scbEnableWake;
    uint8 SENSOR_3_scbEnableIntr;

    /* I2C configuration variables */
    uint8 SENSOR_3_mode;
    uint8 SENSOR_3_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * SENSOR_3_rxBuffer;
    uint8  SENSOR_3_rxDataBits;
    uint32 SENSOR_3_rxBufferSize;

    volatile uint8 * SENSOR_3_txBuffer;
    uint8  SENSOR_3_txDataBits;
    uint32 SENSOR_3_txBufferSize;

    /* EZI2C configuration variables */
    uint8 SENSOR_3_numberOfAddr;
    uint8 SENSOR_3_subAddrSize;
#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** SENSOR_3_initVar indicates whether the SENSOR_3 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the SENSOR_3_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  SENSOR_3_Init() function can be called before the 
*  SENSOR_3_Start() or SENSOR_3_Enable() function.
*/
uint8 SENSOR_3_initVar = 0u;


#if (! (SENSOR_3_SCB_MODE_I2C_CONST_CFG || \
        SENSOR_3_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * SENSOR_3_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent SENSOR_3_Enable() call.
    */
    uint16 SENSOR_3_IntrTxMask = 0u;
#endif /* (! (SENSOR_3_SCB_MODE_I2C_CONST_CFG || \
              SENSOR_3_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (SENSOR_3_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SENSOR_3_CUSTOM_INTR_HANDLER)
    void (*SENSOR_3_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_SENSOR_3_CUSTOM_INTR_HANDLER) */
#endif /* (SENSOR_3_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void SENSOR_3_ScbEnableIntr(void);
static void SENSOR_3_ScbModeStop(void);
static void SENSOR_3_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: SENSOR_3_Init
****************************************************************************//**
*
*  Initializes the SENSOR_3 component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  SENSOR_3_I2CInit, SENSOR_3_SpiInit, 
*  SENSOR_3_UartInit or SENSOR_3_EzI2CInit.
*
*******************************************************************************/
void SENSOR_3_Init(void)
{
#if (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
    if (SENSOR_3_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        SENSOR_3_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (SENSOR_3_SCB_MODE_I2C_CONST_CFG)
    SENSOR_3_I2CInit();

#elif (SENSOR_3_SCB_MODE_SPI_CONST_CFG)
    SENSOR_3_SpiInit();

#elif (SENSOR_3_SCB_MODE_UART_CONST_CFG)
    SENSOR_3_UartInit();

#elif (SENSOR_3_SCB_MODE_EZI2C_CONST_CFG)
    SENSOR_3_EzI2CInit();

#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SENSOR_3_Enable
****************************************************************************//**
*
*  Enables SENSOR_3 component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  SENSOR_3_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The SENSOR_3 configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured SENSOR_3”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void SENSOR_3_Enable(void)
{
#if (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!SENSOR_3_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        SENSOR_3_CTRL_REG |= SENSOR_3_CTRL_ENABLED;

        SENSOR_3_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        SENSOR_3_ScbModePostEnable();
    }
#else
    SENSOR_3_CTRL_REG |= SENSOR_3_CTRL_ENABLED;

    SENSOR_3_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    SENSOR_3_ScbModePostEnable();
#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SENSOR_3_Start
****************************************************************************//**
*
*  Invokes SENSOR_3_Init() and SENSOR_3_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  SENSOR_3_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void SENSOR_3_Start(void)
{
    if (0u == SENSOR_3_initVar)
    {
        SENSOR_3_Init();
        SENSOR_3_initVar = 1u; /* Component was initialized */
    }

    SENSOR_3_Enable();
}


/*******************************************************************************
* Function Name: SENSOR_3_Stop
****************************************************************************//**
*
*  Disables the SENSOR_3 component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function SENSOR_3_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void SENSOR_3_Stop(void)
{
#if (SENSOR_3_SCB_IRQ_INTERNAL)
    SENSOR_3_DisableInt();
#endif /* (SENSOR_3_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    SENSOR_3_ScbModeStop();

    /* Disable SCB IP */
    SENSOR_3_CTRL_REG &= (uint32) ~SENSOR_3_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    SENSOR_3_SetTxInterruptMode(SENSOR_3_NO_INTR_SOURCES);

#if (SENSOR_3_SCB_IRQ_INTERNAL)
    SENSOR_3_ClearPendingInt();
#endif /* (SENSOR_3_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: SENSOR_3_SetRxFifoLevel
****************************************************************************//**
*
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
*  \param level: Level in the RX FIFO to generate RX level interrupt.
*   The range of valid level values is between 0 and RX FIFO depth - 1.
*
*******************************************************************************/
void SENSOR_3_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = SENSOR_3_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~SENSOR_3_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (SENSOR_3_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    SENSOR_3_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: SENSOR_3_SetTxFifoLevel
****************************************************************************//**
*
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has less entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
*  \param level: Level in the TX FIFO to generate TX level interrupt.
*   The range of valid level values is between 0 and TX FIFO depth - 1.
*
*******************************************************************************/
void SENSOR_3_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = SENSOR_3_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~SENSOR_3_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (SENSOR_3_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    SENSOR_3_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (SENSOR_3_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: SENSOR_3_SetCustomInterruptHandler
    ****************************************************************************//**
    *
    *  Registers a function to be called by the internal interrupt handler.
    *  First the function that is registered is called, then the internal interrupt
    *  handler performs any operation such as software buffer management functions
    *  before the interrupt returns.  It is the user's responsibility not to break
    *  the software buffer operations. Only one custom handler is supported, which
    *  is the function provided by the most recent call.
    *  At the initialization time no custom handler is registered.
    *
    *  \param func: Pointer to the function to register.
    *        The value NULL indicates to remove the current custom interrupt
    *        handler.
    *
    *******************************************************************************/
    void SENSOR_3_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_SENSOR_3_CUSTOM_INTR_HANDLER)
        SENSOR_3_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_SENSOR_3_CUSTOM_INTR_HANDLER) */
    }
#endif /* (SENSOR_3_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: SENSOR_3_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void SENSOR_3_ScbEnableIntr(void)
{
#if (SENSOR_3_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != SENSOR_3_scbEnableIntr)
        {
            SENSOR_3_EnableInt();
        }

    #else
        SENSOR_3_EnableInt();

    #endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (SENSOR_3_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: SENSOR_3_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void SENSOR_3_ScbModePostEnable(void)
{
#if (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!SENSOR_3_CY_SCBIP_V1)
    if (SENSOR_3_SCB_MODE_SPI_RUNTM_CFG)
    {
        SENSOR_3_SpiPostEnable();
    }
    else if (SENSOR_3_SCB_MODE_UART_RUNTM_CFG)
    {
        SENSOR_3_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!SENSOR_3_CY_SCBIP_V1) */

#elif (SENSOR_3_SCB_MODE_SPI_CONST_CFG)
    SENSOR_3_SpiPostEnable();

#elif (SENSOR_3_SCB_MODE_UART_CONST_CFG)
    SENSOR_3_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SENSOR_3_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void SENSOR_3_ScbModeStop(void)
{
#if (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
    if (SENSOR_3_SCB_MODE_I2C_RUNTM_CFG)
    {
        SENSOR_3_I2CStop();
    }
    else if (SENSOR_3_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        SENSOR_3_EzI2CStop();
    }
#if (!SENSOR_3_CY_SCBIP_V1)
    else if (SENSOR_3_SCB_MODE_SPI_RUNTM_CFG)
    {
        SENSOR_3_SpiStop();
    }
    else if (SENSOR_3_SCB_MODE_UART_RUNTM_CFG)
    {
        SENSOR_3_UartStop();
    }
#endif /* (!SENSOR_3_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (SENSOR_3_SCB_MODE_I2C_CONST_CFG)
    SENSOR_3_I2CStop();

#elif (SENSOR_3_SCB_MODE_EZI2C_CONST_CFG)
    SENSOR_3_EzI2CStop();

#elif (SENSOR_3_SCB_MODE_SPI_CONST_CFG)
    SENSOR_3_SpiStop();

#elif (SENSOR_3_SCB_MODE_UART_CONST_CFG)
    SENSOR_3_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: SENSOR_3_SetPins
    ****************************************************************************//**
    *
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    *  \param mode:      Mode of SCB operation.
    *  \param subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  \param uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    *******************************************************************************/
    void SENSOR_3_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[SENSOR_3_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!SENSOR_3_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!SENSOR_3_CY_SCBIP_V1) */
        
        uint32 hsiomSel[SENSOR_3_SCB_PINS_NUMBER] = 
        {
            SENSOR_3_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            SENSOR_3_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (SENSOR_3_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (SENSOR_3_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < SENSOR_3_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = SENSOR_3_PIN_DM_ALG_HIZ;
        }

        if ((SENSOR_3_SCB_MODE_I2C   == mode) ||
            (SENSOR_3_SCB_MODE_EZI2C == mode))
        {
        #if (SENSOR_3_RX_SCL_MOSI_PIN)
            hsiomSel[SENSOR_3_RX_SCL_MOSI_PIN_INDEX] = SENSOR_3_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [SENSOR_3_RX_SCL_MOSI_PIN_INDEX] = SENSOR_3_PIN_DM_OD_LO;
        #elif (SENSOR_3_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[SENSOR_3_RX_WAKE_SCL_MOSI_PIN_INDEX] = SENSOR_3_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [SENSOR_3_RX_WAKE_SCL_MOSI_PIN_INDEX] = SENSOR_3_PIN_DM_OD_LO;
        #else
        #endif /* (SENSOR_3_RX_SCL_MOSI_PIN) */
        
        #if (SENSOR_3_TX_SDA_MISO_PIN)
            hsiomSel[SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_PIN_DM_OD_LO;
        #endif /* (SENSOR_3_TX_SDA_MISO_PIN) */
        }
    #if (!SENSOR_3_CY_SCBIP_V1)
        else if (SENSOR_3_SCB_MODE_SPI == mode)
        {
        #if (SENSOR_3_RX_SCL_MOSI_PIN)
            hsiomSel[SENSOR_3_RX_SCL_MOSI_PIN_INDEX] = SENSOR_3_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (SENSOR_3_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[SENSOR_3_RX_WAKE_SCL_MOSI_PIN_INDEX] = SENSOR_3_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (SENSOR_3_RX_SCL_MOSI_PIN) */
        
        #if (SENSOR_3_TX_SDA_MISO_PIN)
            hsiomSel[SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (SENSOR_3_TX_SDA_MISO_PIN) */
        
        #if (SENSOR_3_CTS_SCLK_PIN)
            hsiomSel[SENSOR_3_CTS_SCLK_PIN_INDEX] = SENSOR_3_CTS_SCLK_HSIOM_SEL_SPI;
        #endif /* (SENSOR_3_CTS_SCLK_PIN) */

            if (SENSOR_3_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[SENSOR_3_RX_SCL_MOSI_PIN_INDEX] = SENSOR_3_PIN_DM_DIG_HIZ;
                pinsDm[SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;
                pinsDm[SENSOR_3_CTS_SCLK_PIN_INDEX] = SENSOR_3_PIN_DM_DIG_HIZ;

            #if (SENSOR_3_RTS_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[SENSOR_3_RTS_SS0_PIN_INDEX] = SENSOR_3_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm  [SENSOR_3_RTS_SS0_PIN_INDEX] = SENSOR_3_PIN_DM_DIG_HIZ;
            #endif /* (SENSOR_3_RTS_SS0_PIN) */

            #if (SENSOR_3_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= SENSOR_3_TX_SDA_MISO_PIN_MASK;
            #endif /* (SENSOR_3_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[SENSOR_3_RX_SCL_MOSI_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;
                pinsDm[SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_PIN_DM_DIG_HIZ;
                pinsDm[SENSOR_3_CTS_SCLK_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;

            #if (SENSOR_3_RTS_SS0_PIN)
                hsiomSel [SENSOR_3_RTS_SS0_PIN_INDEX] = SENSOR_3_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm   [SENSOR_3_RTS_SS0_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;
                pinsInBuf |= SENSOR_3_RTS_SS0_PIN_MASK;
            #endif /* (SENSOR_3_RTS_SS0_PIN) */

            #if (SENSOR_3_SS1_PIN)
                hsiomSel [SENSOR_3_SS1_PIN_INDEX] = SENSOR_3_SS1_HSIOM_SEL_SPI;
                pinsDm   [SENSOR_3_SS1_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;
                pinsInBuf |= SENSOR_3_SS1_PIN_MASK;
            #endif /* (SENSOR_3_SS1_PIN) */

            #if (SENSOR_3_SS2_PIN)
                hsiomSel [SENSOR_3_SS2_PIN_INDEX] = SENSOR_3_SS2_HSIOM_SEL_SPI;
                pinsDm   [SENSOR_3_SS2_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;
                pinsInBuf |= SENSOR_3_SS2_PIN_MASK;
            #endif /* (SENSOR_3_SS2_PIN) */

            #if (SENSOR_3_SS3_PIN)
                hsiomSel [SENSOR_3_SS3_PIN_INDEX] = SENSOR_3_SS3_HSIOM_SEL_SPI;
                pinsDm   [SENSOR_3_SS3_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;
                pinsInBuf |= SENSOR_3_SS3_PIN_MASK;
            #endif /* (SENSOR_3_SS3_PIN) */

                /* Disable input buffers */
            #if (SENSOR_3_RX_SCL_MOSI_PIN)
                pinsInBuf |= SENSOR_3_RX_SCL_MOSI_PIN_MASK;
            #elif (SENSOR_3_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= SENSOR_3_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (SENSOR_3_RX_SCL_MOSI_PIN) */

            #if (SENSOR_3_CTS_SCLK_PIN)
                pinsInBuf |= SENSOR_3_CTS_SCLK_PIN_MASK;
            #endif /* (SENSOR_3_CTS_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (SENSOR_3_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (SENSOR_3_TX_SDA_MISO_PIN)
                hsiomSel[SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_PIN_DM_OD_LO;
            #endif /* (SENSOR_3_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (SENSOR_3_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (SENSOR_3_RX_SCL_MOSI_PIN)
                    hsiomSel[SENSOR_3_RX_SCL_MOSI_PIN_INDEX] = SENSOR_3_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [SENSOR_3_RX_SCL_MOSI_PIN_INDEX] = SENSOR_3_PIN_DM_DIG_HIZ;
                #elif (SENSOR_3_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[SENSOR_3_RX_WAKE_SCL_MOSI_PIN_INDEX] = SENSOR_3_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [SENSOR_3_RX_WAKE_SCL_MOSI_PIN_INDEX] = SENSOR_3_PIN_DM_DIG_HIZ;
                #else
                #endif /* (SENSOR_3_RX_SCL_MOSI_PIN) */
                }

                if (0u != (SENSOR_3_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (SENSOR_3_TX_SDA_MISO_PIN)
                    hsiomSel[SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [SENSOR_3_TX_SDA_MISO_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= SENSOR_3_TX_SDA_MISO_PIN_MASK;
                #endif /* (SENSOR_3_TX_SDA_MISO_PIN) */
                }

            #if !(SENSOR_3_CY_SCBIP_V0 || SENSOR_3_CY_SCBIP_V1)
                if (SENSOR_3_UART_MODE_STD == subMode)
                {
                    if (0u != (SENSOR_3_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (SENSOR_3_CTS_SCLK_PIN)
                        hsiomSel[SENSOR_3_CTS_SCLK_PIN_INDEX] = SENSOR_3_CTS_SCLK_HSIOM_SEL_UART;
                        pinsDm  [SENSOR_3_CTS_SCLK_PIN_INDEX] = SENSOR_3_PIN_DM_DIG_HIZ;
                    #endif /* (SENSOR_3_CTS_SCLK_PIN) */
                    }

                    if (0u != (SENSOR_3_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (SENSOR_3_RTS_SS0_PIN)
                        hsiomSel[SENSOR_3_RTS_SS0_PIN_INDEX] = SENSOR_3_RTS_SS0_HSIOM_SEL_UART;
                        pinsDm  [SENSOR_3_RTS_SS0_PIN_INDEX] = SENSOR_3_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= SENSOR_3_RTS_SS0_PIN_MASK;
                    #endif /* (SENSOR_3_RTS_SS0_PIN) */
                    }
                }
            #endif /* !(SENSOR_3_CY_SCBIP_V0 || SENSOR_3_CY_SCBIP_V1) */
            }
        }
    #endif /* (!SENSOR_3_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (SENSOR_3_RX_SCL_MOSI_PIN)
        SENSOR_3_SET_HSIOM_SEL(SENSOR_3_RX_SCL_MOSI_HSIOM_REG,
                                       SENSOR_3_RX_SCL_MOSI_HSIOM_MASK,
                                       SENSOR_3_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[SENSOR_3_RX_SCL_MOSI_PIN_INDEX]);

        SENSOR_3_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[SENSOR_3_RX_SCL_MOSI_PIN_INDEX]);

        #if (!SENSOR_3_CY_SCBIP_V1)
            SENSOR_3_SET_INP_DIS(SENSOR_3_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         SENSOR_3_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & SENSOR_3_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!SENSOR_3_CY_SCBIP_V1) */
    
    #elif (SENSOR_3_RX_WAKE_SCL_MOSI_PIN)
        SENSOR_3_SET_HSIOM_SEL(SENSOR_3_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       SENSOR_3_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       SENSOR_3_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[SENSOR_3_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        SENSOR_3_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[SENSOR_3_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        SENSOR_3_SET_INP_DIS(SENSOR_3_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     SENSOR_3_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & SENSOR_3_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        SENSOR_3_SET_INCFG_TYPE(SENSOR_3_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        SENSOR_3_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        SENSOR_3_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        SENSOR_3_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (SENSOR_3_RX_WAKE_SCL_MOSI_PIN) */

    #if (SENSOR_3_TX_SDA_MISO_PIN)
        SENSOR_3_SET_HSIOM_SEL(SENSOR_3_TX_SDA_MISO_HSIOM_REG,
                                       SENSOR_3_TX_SDA_MISO_HSIOM_MASK,
                                       SENSOR_3_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[SENSOR_3_TX_SDA_MISO_PIN_INDEX]);

        SENSOR_3_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[SENSOR_3_TX_SDA_MISO_PIN_INDEX]);

    #if (!SENSOR_3_CY_SCBIP_V1)
        SENSOR_3_SET_INP_DIS(SENSOR_3_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     SENSOR_3_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & SENSOR_3_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!SENSOR_3_CY_SCBIP_V1) */
    #endif /* (SENSOR_3_RX_SCL_MOSI_PIN) */

    #if (SENSOR_3_CTS_SCLK_PIN)
        SENSOR_3_SET_HSIOM_SEL(SENSOR_3_CTS_SCLK_HSIOM_REG,
                                       SENSOR_3_CTS_SCLK_HSIOM_MASK,
                                       SENSOR_3_CTS_SCLK_HSIOM_POS,
                                       hsiomSel[SENSOR_3_CTS_SCLK_PIN_INDEX]);

        SENSOR_3_uart_cts_spi_sclk_SetDriveMode((uint8) pinsDm[SENSOR_3_CTS_SCLK_PIN_INDEX]);

        SENSOR_3_SET_INP_DIS(SENSOR_3_uart_cts_spi_sclk_INP_DIS,
                                     SENSOR_3_uart_cts_spi_sclk_MASK,
                                     (0u != (pinsInBuf & SENSOR_3_CTS_SCLK_PIN_MASK)));
    #endif /* (SENSOR_3_CTS_SCLK_PIN) */

    #if (SENSOR_3_RTS_SS0_PIN)
        SENSOR_3_SET_HSIOM_SEL(SENSOR_3_RTS_SS0_HSIOM_REG,
                                       SENSOR_3_RTS_SS0_HSIOM_MASK,
                                       SENSOR_3_RTS_SS0_HSIOM_POS,
                                       hsiomSel[SENSOR_3_RTS_SS0_PIN_INDEX]);

        SENSOR_3_uart_rts_spi_ss0_SetDriveMode((uint8) pinsDm[SENSOR_3_RTS_SS0_PIN_INDEX]);

        SENSOR_3_SET_INP_DIS(SENSOR_3_uart_rts_spi_ss0_INP_DIS,
                                     SENSOR_3_uart_rts_spi_ss0_MASK,
                                     (0u != (pinsInBuf & SENSOR_3_RTS_SS0_PIN_MASK)));
    #endif /* (SENSOR_3_RTS_SS0_PIN) */

    #if (SENSOR_3_SS1_PIN)
        SENSOR_3_SET_HSIOM_SEL(SENSOR_3_SS1_HSIOM_REG,
                                       SENSOR_3_SS1_HSIOM_MASK,
                                       SENSOR_3_SS1_HSIOM_POS,
                                       hsiomSel[SENSOR_3_SS1_PIN_INDEX]);

        SENSOR_3_spi_ss1_SetDriveMode((uint8) pinsDm[SENSOR_3_SS1_PIN_INDEX]);

        SENSOR_3_SET_INP_DIS(SENSOR_3_spi_ss1_INP_DIS,
                                     SENSOR_3_spi_ss1_MASK,
                                     (0u != (pinsInBuf & SENSOR_3_SS1_PIN_MASK)));
    #endif /* (SENSOR_3_SS1_PIN) */

    #if (SENSOR_3_SS2_PIN)
        SENSOR_3_SET_HSIOM_SEL(SENSOR_3_SS2_HSIOM_REG,
                                       SENSOR_3_SS2_HSIOM_MASK,
                                       SENSOR_3_SS2_HSIOM_POS,
                                       hsiomSel[SENSOR_3_SS2_PIN_INDEX]);

        SENSOR_3_spi_ss2_SetDriveMode((uint8) pinsDm[SENSOR_3_SS2_PIN_INDEX]);

        SENSOR_3_SET_INP_DIS(SENSOR_3_spi_ss2_INP_DIS,
                                     SENSOR_3_spi_ss2_MASK,
                                     (0u != (pinsInBuf & SENSOR_3_SS2_PIN_MASK)));
    #endif /* (SENSOR_3_SS2_PIN) */

    #if (SENSOR_3_SS3_PIN)
        SENSOR_3_SET_HSIOM_SEL(SENSOR_3_SS3_HSIOM_REG,
                                       SENSOR_3_SS3_HSIOM_MASK,
                                       SENSOR_3_SS3_HSIOM_POS,
                                       hsiomSel[SENSOR_3_SS3_PIN_INDEX]);

        SENSOR_3_spi_ss3_SetDriveMode((uint8) pinsDm[SENSOR_3_SS3_PIN_INDEX]);

        SENSOR_3_SET_INP_DIS(SENSOR_3_spi_ss3_INP_DIS,
                                     SENSOR_3_spi_ss3_MASK,
                                     (0u != (pinsInBuf & SENSOR_3_SS3_PIN_MASK)));
    #endif /* (SENSOR_3_SS3_PIN) */
    }

#endif /* (SENSOR_3_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (SENSOR_3_CY_SCBIP_V0 || SENSOR_3_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: SENSOR_3_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void SENSOR_3_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (SENSOR_3_CTRL_REG & SENSOR_3_CTRL_EC_AM_MODE)) &&
            (0u == (SENSOR_3_I2C_CTRL_REG & SENSOR_3_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            SENSOR_3_CTRL_REG &= ~SENSOR_3_CTRL_EC_AM_MODE;
            SENSOR_3_CTRL_REG |=  SENSOR_3_CTRL_EC_AM_MODE;
        }

        SENSOR_3_I2C_SLAVE_CMD_REG = SENSOR_3_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (SENSOR_3_CY_SCBIP_V0 || SENSOR_3_CY_SCBIP_V1) */


/* [] END OF FILE */
