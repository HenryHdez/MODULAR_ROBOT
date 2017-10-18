/*******************************************************************************
* File Name: SENSOR_RX.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_SENSOR_RX_H)
#define CY_UART_SENSOR_RX_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

#define SENSOR_RX_RX_ENABLED                     (1u)
#define SENSOR_RX_TX_ENABLED                     (0u)
#define SENSOR_RX_HD_ENABLED                     (0u)
#define SENSOR_RX_RX_INTERRUPT_ENABLED           (1u)
#define SENSOR_RX_TX_INTERRUPT_ENABLED           (0u)
#define SENSOR_RX_INTERNAL_CLOCK_USED            (1u)
#define SENSOR_RX_RXHW_ADDRESS_ENABLED           (0u)
#define SENSOR_RX_OVER_SAMPLE_COUNT              (8u)
#define SENSOR_RX_PARITY_TYPE                    (0u)
#define SENSOR_RX_PARITY_TYPE_SW                 (0u)
#define SENSOR_RX_BREAK_DETECT                   (0u)
#define SENSOR_RX_BREAK_BITS_TX                  (13u)
#define SENSOR_RX_BREAK_BITS_RX                  (13u)
#define SENSOR_RX_TXCLKGEN_DP                    (1u)
#define SENSOR_RX_USE23POLLING                   (1u)
#define SENSOR_RX_FLOW_CONTROL                   (0u)
#define SENSOR_RX_CLK_FREQ                       (0u)
#define SENSOR_RX_TX_BUFFER_SIZE                 (4u)
#define SENSOR_RX_RX_BUFFER_SIZE                 (10u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(SENSOR_RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define SENSOR_RX_CONTROL_REG_REMOVED            (0u)
#else
    #define SENSOR_RX_CONTROL_REG_REMOVED            (1u)
#endif /* End SENSOR_RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct SENSOR_RX_backupStruct_
{
    uint8 enableState;

    #if(SENSOR_RX_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End SENSOR_RX_CONTROL_REG_REMOVED */

} SENSOR_RX_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void SENSOR_RX_Start(void) ;
void SENSOR_RX_Stop(void) ;
uint8 SENSOR_RX_ReadControlRegister(void) ;
void SENSOR_RX_WriteControlRegister(uint8 control) ;

void SENSOR_RX_Init(void) ;
void SENSOR_RX_Enable(void) ;
void SENSOR_RX_SaveConfig(void) ;
void SENSOR_RX_RestoreConfig(void) ;
void SENSOR_RX_Sleep(void) ;
void SENSOR_RX_Wakeup(void) ;

/* Only if RX is enabled */
#if( (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) )

    #if (SENSOR_RX_RX_INTERRUPT_ENABLED)
        #define SENSOR_RX_EnableRxInt()  CyIntEnable (SENSOR_RX_RX_VECT_NUM)
        #define SENSOR_RX_DisableRxInt() CyIntDisable(SENSOR_RX_RX_VECT_NUM)
        CY_ISR_PROTO(SENSOR_RX_RXISR);
    #endif /* SENSOR_RX_RX_INTERRUPT_ENABLED */

    void SENSOR_RX_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void SENSOR_RX_SetRxAddress1(uint8 address) ;
    void SENSOR_RX_SetRxAddress2(uint8 address) ;

    void  SENSOR_RX_SetRxInterruptMode(uint8 intSrc) ;
    uint8 SENSOR_RX_ReadRxData(void) ;
    uint8 SENSOR_RX_ReadRxStatus(void) ;
    uint8 SENSOR_RX_GetChar(void) ;
    uint16 SENSOR_RX_GetByte(void) ;
    uint8 SENSOR_RX_GetRxBufferSize(void)
                                                            ;
    void SENSOR_RX_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define SENSOR_RX_GetRxInterruptSource   SENSOR_RX_ReadRxStatus

#endif /* End (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) */

/* Only if TX is enabled */
#if(SENSOR_RX_TX_ENABLED || SENSOR_RX_HD_ENABLED)

    #if(SENSOR_RX_TX_INTERRUPT_ENABLED)
        #define SENSOR_RX_EnableTxInt()  CyIntEnable (SENSOR_RX_TX_VECT_NUM)
        #define SENSOR_RX_DisableTxInt() CyIntDisable(SENSOR_RX_TX_VECT_NUM)
        #define SENSOR_RX_SetPendingTxInt() CyIntSetPending(SENSOR_RX_TX_VECT_NUM)
        #define SENSOR_RX_ClearPendingTxInt() CyIntClearPending(SENSOR_RX_TX_VECT_NUM)
        CY_ISR_PROTO(SENSOR_RX_TXISR);
    #endif /* SENSOR_RX_TX_INTERRUPT_ENABLED */

    void SENSOR_RX_SetTxInterruptMode(uint8 intSrc) ;
    void SENSOR_RX_WriteTxData(uint8 txDataByte) ;
    uint8 SENSOR_RX_ReadTxStatus(void) ;
    void SENSOR_RX_PutChar(uint8 txDataByte) ;
    void SENSOR_RX_PutString(const char8 string[]) ;
    void SENSOR_RX_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void SENSOR_RX_PutCRLF(uint8 txDataByte) ;
    void SENSOR_RX_ClearTxBuffer(void) ;
    void SENSOR_RX_SetTxAddressMode(uint8 addressMode) ;
    void SENSOR_RX_SendBreak(uint8 retMode) ;
    uint8 SENSOR_RX_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define SENSOR_RX_PutStringConst         SENSOR_RX_PutString
    #define SENSOR_RX_PutArrayConst          SENSOR_RX_PutArray
    #define SENSOR_RX_GetTxInterruptSource   SENSOR_RX_ReadTxStatus

#endif /* End SENSOR_RX_TX_ENABLED || SENSOR_RX_HD_ENABLED */

#if(SENSOR_RX_HD_ENABLED)
    void SENSOR_RX_LoadRxConfig(void) ;
    void SENSOR_RX_LoadTxConfig(void) ;
#endif /* End SENSOR_RX_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SENSOR_RX) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    SENSOR_RX_CyBtldrCommStart(void) CYSMALL ;
    void    SENSOR_RX_CyBtldrCommStop(void) CYSMALL ;
    void    SENSOR_RX_CyBtldrCommReset(void) CYSMALL ;
    cystatus SENSOR_RX_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus SENSOR_RX_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SENSOR_RX)
        #define CyBtldrCommStart    SENSOR_RX_CyBtldrCommStart
        #define CyBtldrCommStop     SENSOR_RX_CyBtldrCommStop
        #define CyBtldrCommReset    SENSOR_RX_CyBtldrCommReset
        #define CyBtldrCommWrite    SENSOR_RX_CyBtldrCommWrite
        #define CyBtldrCommRead     SENSOR_RX_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SENSOR_RX) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define SENSOR_RX_BYTE2BYTE_TIME_OUT (25u)
    #define SENSOR_RX_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define SENSOR_RX_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define SENSOR_RX_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define SENSOR_RX_SET_SPACE      (0x00u)
#define SENSOR_RX_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (SENSOR_RX_TX_ENABLED) || (SENSOR_RX_HD_ENABLED) )
    #if(SENSOR_RX_TX_INTERRUPT_ENABLED)
        #define SENSOR_RX_TX_VECT_NUM            (uint8)SENSOR_RX_TXInternalInterrupt__INTC_NUMBER
        #define SENSOR_RX_TX_PRIOR_NUM           (uint8)SENSOR_RX_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* SENSOR_RX_TX_INTERRUPT_ENABLED */

    #define SENSOR_RX_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define SENSOR_RX_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define SENSOR_RX_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(SENSOR_RX_TX_ENABLED)
        #define SENSOR_RX_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (SENSOR_RX_HD_ENABLED) */
        #define SENSOR_RX_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (SENSOR_RX_TX_ENABLED) */

    #define SENSOR_RX_TX_STS_COMPLETE            (uint8)(0x01u << SENSOR_RX_TX_STS_COMPLETE_SHIFT)
    #define SENSOR_RX_TX_STS_FIFO_EMPTY          (uint8)(0x01u << SENSOR_RX_TX_STS_FIFO_EMPTY_SHIFT)
    #define SENSOR_RX_TX_STS_FIFO_FULL           (uint8)(0x01u << SENSOR_RX_TX_STS_FIFO_FULL_SHIFT)
    #define SENSOR_RX_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << SENSOR_RX_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (SENSOR_RX_TX_ENABLED) || (SENSOR_RX_HD_ENABLED)*/

#if( (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) )
    #if(SENSOR_RX_RX_INTERRUPT_ENABLED)
        #define SENSOR_RX_RX_VECT_NUM            (uint8)SENSOR_RX_RXInternalInterrupt__INTC_NUMBER
        #define SENSOR_RX_RX_PRIOR_NUM           (uint8)SENSOR_RX_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* SENSOR_RX_RX_INTERRUPT_ENABLED */
    #define SENSOR_RX_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define SENSOR_RX_RX_STS_BREAK_SHIFT             (0x01u)
    #define SENSOR_RX_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define SENSOR_RX_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define SENSOR_RX_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define SENSOR_RX_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define SENSOR_RX_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define SENSOR_RX_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define SENSOR_RX_RX_STS_MRKSPC           (uint8)(0x01u << SENSOR_RX_RX_STS_MRKSPC_SHIFT)
    #define SENSOR_RX_RX_STS_BREAK            (uint8)(0x01u << SENSOR_RX_RX_STS_BREAK_SHIFT)
    #define SENSOR_RX_RX_STS_PAR_ERROR        (uint8)(0x01u << SENSOR_RX_RX_STS_PAR_ERROR_SHIFT)
    #define SENSOR_RX_RX_STS_STOP_ERROR       (uint8)(0x01u << SENSOR_RX_RX_STS_STOP_ERROR_SHIFT)
    #define SENSOR_RX_RX_STS_OVERRUN          (uint8)(0x01u << SENSOR_RX_RX_STS_OVERRUN_SHIFT)
    #define SENSOR_RX_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << SENSOR_RX_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define SENSOR_RX_RX_STS_ADDR_MATCH       (uint8)(0x01u << SENSOR_RX_RX_STS_ADDR_MATCH_SHIFT)
    #define SENSOR_RX_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << SENSOR_RX_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define SENSOR_RX_RX_HW_MASK                     (0x7Fu)
#endif /* End (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) */

/* Control Register definitions */
#define SENSOR_RX_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define SENSOR_RX_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define SENSOR_RX_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define SENSOR_RX_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define SENSOR_RX_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define SENSOR_RX_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define SENSOR_RX_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define SENSOR_RX_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define SENSOR_RX_CTRL_HD_SEND               (uint8)(0x01u << SENSOR_RX_CTRL_HD_SEND_SHIFT)
#define SENSOR_RX_CTRL_HD_SEND_BREAK         (uint8)(0x01u << SENSOR_RX_CTRL_HD_SEND_BREAK_SHIFT)
#define SENSOR_RX_CTRL_MARK                  (uint8)(0x01u << SENSOR_RX_CTRL_MARK_SHIFT)
#define SENSOR_RX_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << SENSOR_RX_CTRL_PARITY_TYPE0_SHIFT)
#define SENSOR_RX_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << SENSOR_RX_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define SENSOR_RX_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define SENSOR_RX_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define SENSOR_RX_SEND_BREAK                         (0x00u)
#define SENSOR_RX_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define SENSOR_RX_REINIT                             (0x02u)
#define SENSOR_RX_SEND_WAIT_REINIT                   (0x03u)

#define SENSOR_RX_OVER_SAMPLE_8                      (8u)
#define SENSOR_RX_OVER_SAMPLE_16                     (16u)

#define SENSOR_RX_BIT_CENTER                         (SENSOR_RX_OVER_SAMPLE_COUNT - 2u)

#define SENSOR_RX_FIFO_LENGTH                        (4u)
#define SENSOR_RX_NUMBER_OF_START_BIT                (1u)
#define SENSOR_RX_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define SENSOR_RX_TXBITCTR_BREAKBITS8X   ((SENSOR_RX_BREAK_BITS_TX * SENSOR_RX_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define SENSOR_RX_TXBITCTR_BREAKBITS ((SENSOR_RX_BREAK_BITS_TX * SENSOR_RX_OVER_SAMPLE_COUNT) - 1u)

#define SENSOR_RX_HALF_BIT_COUNT   \
                            (((SENSOR_RX_OVER_SAMPLE_COUNT / 2u) + (SENSOR_RX_USE23POLLING * 1u)) - 2u)
#if (SENSOR_RX_OVER_SAMPLE_COUNT == SENSOR_RX_OVER_SAMPLE_8)
    #define SENSOR_RX_HD_TXBITCTR_INIT   (((SENSOR_RX_BREAK_BITS_TX + \
                            SENSOR_RX_NUMBER_OF_START_BIT) * SENSOR_RX_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define SENSOR_RX_RXBITCTR_INIT  ((((SENSOR_RX_BREAK_BITS_RX + SENSOR_RX_NUMBER_OF_START_BIT) \
                            * SENSOR_RX_OVER_SAMPLE_COUNT) + SENSOR_RX_HALF_BIT_COUNT) - 1u)

#else /* SENSOR_RX_OVER_SAMPLE_COUNT == SENSOR_RX_OVER_SAMPLE_16 */
    #define SENSOR_RX_HD_TXBITCTR_INIT   ((8u * SENSOR_RX_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define SENSOR_RX_RXBITCTR_INIT      (((7u * SENSOR_RX_OVER_SAMPLE_COUNT) - 1u) + \
                                                      SENSOR_RX_HALF_BIT_COUNT)
#endif /* End SENSOR_RX_OVER_SAMPLE_COUNT */

#define SENSOR_RX_HD_RXBITCTR_INIT                   SENSOR_RX_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 SENSOR_RX_initVar;
#if (SENSOR_RX_TX_INTERRUPT_ENABLED && SENSOR_RX_TX_ENABLED)
    extern volatile uint8 SENSOR_RX_txBuffer[SENSOR_RX_TX_BUFFER_SIZE];
    extern volatile uint8 SENSOR_RX_txBufferRead;
    extern uint8 SENSOR_RX_txBufferWrite;
#endif /* (SENSOR_RX_TX_INTERRUPT_ENABLED && SENSOR_RX_TX_ENABLED) */
#if (SENSOR_RX_RX_INTERRUPT_ENABLED && (SENSOR_RX_RX_ENABLED || SENSOR_RX_HD_ENABLED))
    extern uint8 SENSOR_RX_errorStatus;
    extern volatile uint8 SENSOR_RX_rxBuffer[SENSOR_RX_RX_BUFFER_SIZE];
    extern volatile uint8 SENSOR_RX_rxBufferRead;
    extern volatile uint8 SENSOR_RX_rxBufferWrite;
    extern volatile uint8 SENSOR_RX_rxBufferLoopDetect;
    extern volatile uint8 SENSOR_RX_rxBufferOverflow;
    #if (SENSOR_RX_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 SENSOR_RX_rxAddressMode;
        extern volatile uint8 SENSOR_RX_rxAddressDetected;
    #endif /* (SENSOR_RX_RXHW_ADDRESS_ENABLED) */
#endif /* (SENSOR_RX_RX_INTERRUPT_ENABLED && (SENSOR_RX_RX_ENABLED || SENSOR_RX_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define SENSOR_RX__B_UART__AM_SW_BYTE_BYTE 1
#define SENSOR_RX__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define SENSOR_RX__B_UART__AM_HW_BYTE_BY_BYTE 3
#define SENSOR_RX__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define SENSOR_RX__B_UART__AM_NONE 0

#define SENSOR_RX__B_UART__NONE_REVB 0
#define SENSOR_RX__B_UART__EVEN_REVB 1
#define SENSOR_RX__B_UART__ODD_REVB 2
#define SENSOR_RX__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define SENSOR_RX_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define SENSOR_RX_NUMBER_OF_STOP_BITS    (1u)

#if (SENSOR_RX_RXHW_ADDRESS_ENABLED)
    #define SENSOR_RX_RX_ADDRESS_MODE    (0u)
    #define SENSOR_RX_RX_HW_ADDRESS1     (0u)
    #define SENSOR_RX_RX_HW_ADDRESS2     (0u)
#endif /* (SENSOR_RX_RXHW_ADDRESS_ENABLED) */

#define SENSOR_RX_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << SENSOR_RX_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << SENSOR_RX_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << SENSOR_RX_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << SENSOR_RX_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << SENSOR_RX_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << SENSOR_RX_RX_STS_BREAK_SHIFT) \
                                        | (0 << SENSOR_RX_RX_STS_OVERRUN_SHIFT))

#define SENSOR_RX_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << SENSOR_RX_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << SENSOR_RX_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << SENSOR_RX_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << SENSOR_RX_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef SENSOR_RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define SENSOR_RX_CONTROL_REG \
                            (* (reg8 *) SENSOR_RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define SENSOR_RX_CONTROL_PTR \
                            (  (reg8 *) SENSOR_RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End SENSOR_RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(SENSOR_RX_TX_ENABLED)
    #define SENSOR_RX_TXDATA_REG          (* (reg8 *) SENSOR_RX_BUART_sTX_TxShifter_u0__F0_REG)
    #define SENSOR_RX_TXDATA_PTR          (  (reg8 *) SENSOR_RX_BUART_sTX_TxShifter_u0__F0_REG)
    #define SENSOR_RX_TXDATA_AUX_CTL_REG  (* (reg8 *) SENSOR_RX_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define SENSOR_RX_TXDATA_AUX_CTL_PTR  (  (reg8 *) SENSOR_RX_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define SENSOR_RX_TXSTATUS_REG        (* (reg8 *) SENSOR_RX_BUART_sTX_TxSts__STATUS_REG)
    #define SENSOR_RX_TXSTATUS_PTR        (  (reg8 *) SENSOR_RX_BUART_sTX_TxSts__STATUS_REG)
    #define SENSOR_RX_TXSTATUS_MASK_REG   (* (reg8 *) SENSOR_RX_BUART_sTX_TxSts__MASK_REG)
    #define SENSOR_RX_TXSTATUS_MASK_PTR   (  (reg8 *) SENSOR_RX_BUART_sTX_TxSts__MASK_REG)
    #define SENSOR_RX_TXSTATUS_ACTL_REG   (* (reg8 *) SENSOR_RX_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define SENSOR_RX_TXSTATUS_ACTL_PTR   (  (reg8 *) SENSOR_RX_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(SENSOR_RX_TXCLKGEN_DP)
        #define SENSOR_RX_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define SENSOR_RX_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define SENSOR_RX_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define SENSOR_RX_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define SENSOR_RX_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define SENSOR_RX_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define SENSOR_RX_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define SENSOR_RX_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define SENSOR_RX_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define SENSOR_RX_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) SENSOR_RX_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* SENSOR_RX_TXCLKGEN_DP */

#endif /* End SENSOR_RX_TX_ENABLED */

#if(SENSOR_RX_HD_ENABLED)

    #define SENSOR_RX_TXDATA_REG             (* (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__F1_REG )
    #define SENSOR_RX_TXDATA_PTR             (  (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__F1_REG )
    #define SENSOR_RX_TXDATA_AUX_CTL_REG     (* (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define SENSOR_RX_TXDATA_AUX_CTL_PTR     (  (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define SENSOR_RX_TXSTATUS_REG           (* (reg8 *) SENSOR_RX_BUART_sRX_RxSts__STATUS_REG )
    #define SENSOR_RX_TXSTATUS_PTR           (  (reg8 *) SENSOR_RX_BUART_sRX_RxSts__STATUS_REG )
    #define SENSOR_RX_TXSTATUS_MASK_REG      (* (reg8 *) SENSOR_RX_BUART_sRX_RxSts__MASK_REG )
    #define SENSOR_RX_TXSTATUS_MASK_PTR      (  (reg8 *) SENSOR_RX_BUART_sRX_RxSts__MASK_REG )
    #define SENSOR_RX_TXSTATUS_ACTL_REG      (* (reg8 *) SENSOR_RX_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define SENSOR_RX_TXSTATUS_ACTL_PTR      (  (reg8 *) SENSOR_RX_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End SENSOR_RX_HD_ENABLED */

#if( (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) )
    #define SENSOR_RX_RXDATA_REG             (* (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__F0_REG )
    #define SENSOR_RX_RXDATA_PTR             (  (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__F0_REG )
    #define SENSOR_RX_RXADDRESS1_REG         (* (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__D0_REG )
    #define SENSOR_RX_RXADDRESS1_PTR         (  (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__D0_REG )
    #define SENSOR_RX_RXADDRESS2_REG         (* (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__D1_REG )
    #define SENSOR_RX_RXADDRESS2_PTR         (  (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__D1_REG )
    #define SENSOR_RX_RXDATA_AUX_CTL_REG     (* (reg8 *) SENSOR_RX_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define SENSOR_RX_RXBITCTR_PERIOD_REG    (* (reg8 *) SENSOR_RX_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define SENSOR_RX_RXBITCTR_PERIOD_PTR    (  (reg8 *) SENSOR_RX_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define SENSOR_RX_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) SENSOR_RX_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define SENSOR_RX_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) SENSOR_RX_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define SENSOR_RX_RXBITCTR_COUNTER_REG   (* (reg8 *) SENSOR_RX_BUART_sRX_RxBitCounter__COUNT_REG )
    #define SENSOR_RX_RXBITCTR_COUNTER_PTR   (  (reg8 *) SENSOR_RX_BUART_sRX_RxBitCounter__COUNT_REG )

    #define SENSOR_RX_RXSTATUS_REG           (* (reg8 *) SENSOR_RX_BUART_sRX_RxSts__STATUS_REG )
    #define SENSOR_RX_RXSTATUS_PTR           (  (reg8 *) SENSOR_RX_BUART_sRX_RxSts__STATUS_REG )
    #define SENSOR_RX_RXSTATUS_MASK_REG      (* (reg8 *) SENSOR_RX_BUART_sRX_RxSts__MASK_REG )
    #define SENSOR_RX_RXSTATUS_MASK_PTR      (  (reg8 *) SENSOR_RX_BUART_sRX_RxSts__MASK_REG )
    #define SENSOR_RX_RXSTATUS_ACTL_REG      (* (reg8 *) SENSOR_RX_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define SENSOR_RX_RXSTATUS_ACTL_PTR      (  (reg8 *) SENSOR_RX_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) */

#if(SENSOR_RX_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define SENSOR_RX_INTCLOCK_CLKEN_REG     (* (reg8 *) SENSOR_RX_IntClock__PM_ACT_CFG)
    #define SENSOR_RX_INTCLOCK_CLKEN_PTR     (  (reg8 *) SENSOR_RX_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define SENSOR_RX_INTCLOCK_CLKEN_MASK    SENSOR_RX_IntClock__PM_ACT_MSK
#endif /* End SENSOR_RX_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(SENSOR_RX_TX_ENABLED)
    #define SENSOR_RX_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End SENSOR_RX_TX_ENABLED */

#if(SENSOR_RX_HD_ENABLED)
    #define SENSOR_RX_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End SENSOR_RX_HD_ENABLED */

#if( (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) )
    #define SENSOR_RX_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define SENSOR_RX_WAIT_1_MS      SENSOR_RX_BL_CHK_DELAY_MS   

#define SENSOR_RX_TXBUFFERSIZE   SENSOR_RX_TX_BUFFER_SIZE
#define SENSOR_RX_RXBUFFERSIZE   SENSOR_RX_RX_BUFFER_SIZE

#if (SENSOR_RX_RXHW_ADDRESS_ENABLED)
    #define SENSOR_RX_RXADDRESSMODE  SENSOR_RX_RX_ADDRESS_MODE
    #define SENSOR_RX_RXHWADDRESS1   SENSOR_RX_RX_HW_ADDRESS1
    #define SENSOR_RX_RXHWADDRESS2   SENSOR_RX_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define SENSOR_RX_RXAddressMode  SENSOR_RX_RXADDRESSMODE
#endif /* (SENSOR_RX_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define SENSOR_RX_initvar                    SENSOR_RX_initVar

#define SENSOR_RX_RX_Enabled                 SENSOR_RX_RX_ENABLED
#define SENSOR_RX_TX_Enabled                 SENSOR_RX_TX_ENABLED
#define SENSOR_RX_HD_Enabled                 SENSOR_RX_HD_ENABLED
#define SENSOR_RX_RX_IntInterruptEnabled     SENSOR_RX_RX_INTERRUPT_ENABLED
#define SENSOR_RX_TX_IntInterruptEnabled     SENSOR_RX_TX_INTERRUPT_ENABLED
#define SENSOR_RX_InternalClockUsed          SENSOR_RX_INTERNAL_CLOCK_USED
#define SENSOR_RX_RXHW_Address_Enabled       SENSOR_RX_RXHW_ADDRESS_ENABLED
#define SENSOR_RX_OverSampleCount            SENSOR_RX_OVER_SAMPLE_COUNT
#define SENSOR_RX_ParityType                 SENSOR_RX_PARITY_TYPE

#if( SENSOR_RX_TX_ENABLED && (SENSOR_RX_TXBUFFERSIZE > SENSOR_RX_FIFO_LENGTH))
    #define SENSOR_RX_TXBUFFER               SENSOR_RX_txBuffer
    #define SENSOR_RX_TXBUFFERREAD           SENSOR_RX_txBufferRead
    #define SENSOR_RX_TXBUFFERWRITE          SENSOR_RX_txBufferWrite
#endif /* End SENSOR_RX_TX_ENABLED */
#if( ( SENSOR_RX_RX_ENABLED || SENSOR_RX_HD_ENABLED ) && \
     (SENSOR_RX_RXBUFFERSIZE > SENSOR_RX_FIFO_LENGTH) )
    #define SENSOR_RX_RXBUFFER               SENSOR_RX_rxBuffer
    #define SENSOR_RX_RXBUFFERREAD           SENSOR_RX_rxBufferRead
    #define SENSOR_RX_RXBUFFERWRITE          SENSOR_RX_rxBufferWrite
    #define SENSOR_RX_RXBUFFERLOOPDETECT     SENSOR_RX_rxBufferLoopDetect
    #define SENSOR_RX_RXBUFFER_OVERFLOW      SENSOR_RX_rxBufferOverflow
#endif /* End SENSOR_RX_RX_ENABLED */

#ifdef SENSOR_RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define SENSOR_RX_CONTROL                SENSOR_RX_CONTROL_REG
#endif /* End SENSOR_RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(SENSOR_RX_TX_ENABLED)
    #define SENSOR_RX_TXDATA                 SENSOR_RX_TXDATA_REG
    #define SENSOR_RX_TXSTATUS               SENSOR_RX_TXSTATUS_REG
    #define SENSOR_RX_TXSTATUS_MASK          SENSOR_RX_TXSTATUS_MASK_REG
    #define SENSOR_RX_TXSTATUS_ACTL          SENSOR_RX_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(SENSOR_RX_TXCLKGEN_DP)
        #define SENSOR_RX_TXBITCLKGEN_CTR        SENSOR_RX_TXBITCLKGEN_CTR_REG
        #define SENSOR_RX_TXBITCLKTX_COMPLETE    SENSOR_RX_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define SENSOR_RX_TXBITCTR_PERIOD        SENSOR_RX_TXBITCTR_PERIOD_REG
        #define SENSOR_RX_TXBITCTR_CONTROL       SENSOR_RX_TXBITCTR_CONTROL_REG
        #define SENSOR_RX_TXBITCTR_COUNTER       SENSOR_RX_TXBITCTR_COUNTER_REG
    #endif /* SENSOR_RX_TXCLKGEN_DP */
#endif /* End SENSOR_RX_TX_ENABLED */

#if(SENSOR_RX_HD_ENABLED)
    #define SENSOR_RX_TXDATA                 SENSOR_RX_TXDATA_REG
    #define SENSOR_RX_TXSTATUS               SENSOR_RX_TXSTATUS_REG
    #define SENSOR_RX_TXSTATUS_MASK          SENSOR_RX_TXSTATUS_MASK_REG
    #define SENSOR_RX_TXSTATUS_ACTL          SENSOR_RX_TXSTATUS_ACTL_REG
#endif /* End SENSOR_RX_HD_ENABLED */

#if( (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) )
    #define SENSOR_RX_RXDATA                 SENSOR_RX_RXDATA_REG
    #define SENSOR_RX_RXADDRESS1             SENSOR_RX_RXADDRESS1_REG
    #define SENSOR_RX_RXADDRESS2             SENSOR_RX_RXADDRESS2_REG
    #define SENSOR_RX_RXBITCTR_PERIOD        SENSOR_RX_RXBITCTR_PERIOD_REG
    #define SENSOR_RX_RXBITCTR_CONTROL       SENSOR_RX_RXBITCTR_CONTROL_REG
    #define SENSOR_RX_RXBITCTR_COUNTER       SENSOR_RX_RXBITCTR_COUNTER_REG
    #define SENSOR_RX_RXSTATUS               SENSOR_RX_RXSTATUS_REG
    #define SENSOR_RX_RXSTATUS_MASK          SENSOR_RX_RXSTATUS_MASK_REG
    #define SENSOR_RX_RXSTATUS_ACTL          SENSOR_RX_RXSTATUS_ACTL_REG
#endif /* End  (SENSOR_RX_RX_ENABLED) || (SENSOR_RX_HD_ENABLED) */

#if(SENSOR_RX_INTERNAL_CLOCK_USED)
    #define SENSOR_RX_INTCLOCK_CLKEN         SENSOR_RX_INTCLOCK_CLKEN_REG
#endif /* End SENSOR_RX_INTERNAL_CLOCK_USED */

#define SENSOR_RX_WAIT_FOR_COMLETE_REINIT    SENSOR_RX_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_SENSOR_RX_H */


/* [] END OF FILE */
