/*******************************************************************************
* File Name: MOTOR.h
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


#if !defined(CY_UART_MOTOR_H)
#define CY_UART_MOTOR_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define MOTOR_RX_ENABLED                     (0u)
#define MOTOR_TX_ENABLED                     (1u)
#define MOTOR_HD_ENABLED                     (0u)
#define MOTOR_RX_INTERRUPT_ENABLED           (1u)
#define MOTOR_TX_INTERRUPT_ENABLED           (0u)
#define MOTOR_INTERNAL_CLOCK_USED            (1u)
#define MOTOR_RXHW_ADDRESS_ENABLED           (0u)
#define MOTOR_OVER_SAMPLE_COUNT              (8u)
#define MOTOR_PARITY_TYPE                    (0u)
#define MOTOR_PARITY_TYPE_SW                 (0u)
#define MOTOR_BREAK_DETECT                   (0u)
#define MOTOR_BREAK_BITS_TX                  (13u)
#define MOTOR_BREAK_BITS_RX                  (13u)
#define MOTOR_TXCLKGEN_DP                    (1u)
#define MOTOR_USE23POLLING                   (1u)
#define MOTOR_FLOW_CONTROL                   (0u)
#define MOTOR_CLK_FREQ                       (0u)
#define MOTOR_TX_BUFFER_SIZE                 (4u)
#define MOTOR_RX_BUFFER_SIZE                 (8u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(MOTOR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define MOTOR_CONTROL_REG_REMOVED            (0u)
#else
    #define MOTOR_CONTROL_REG_REMOVED            (1u)
#endif /* End MOTOR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct MOTOR_backupStruct_
{
    uint8 enableState;

    #if(MOTOR_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End MOTOR_CONTROL_REG_REMOVED */

} MOTOR_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void MOTOR_Start(void) ;
void MOTOR_Stop(void) ;
uint8 MOTOR_ReadControlRegister(void) ;
void MOTOR_WriteControlRegister(uint8 control) ;

void MOTOR_Init(void) ;
void MOTOR_Enable(void) ;
void MOTOR_SaveConfig(void) ;
void MOTOR_RestoreConfig(void) ;
void MOTOR_Sleep(void) ;
void MOTOR_Wakeup(void) ;

/* Only if RX is enabled */
#if( (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) )

    #if (MOTOR_RX_INTERRUPT_ENABLED)
        #define MOTOR_EnableRxInt()  CyIntEnable (MOTOR_RX_VECT_NUM)
        #define MOTOR_DisableRxInt() CyIntDisable(MOTOR_RX_VECT_NUM)
        CY_ISR_PROTO(MOTOR_RXISR);
    #endif /* MOTOR_RX_INTERRUPT_ENABLED */

    void MOTOR_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void MOTOR_SetRxAddress1(uint8 address) ;
    void MOTOR_SetRxAddress2(uint8 address) ;

    void  MOTOR_SetRxInterruptMode(uint8 intSrc) ;
    uint8 MOTOR_ReadRxData(void) ;
    uint8 MOTOR_ReadRxStatus(void) ;
    uint8 MOTOR_GetChar(void) ;
    uint16 MOTOR_GetByte(void) ;
    uint8 MOTOR_GetRxBufferSize(void)
                                                            ;
    void MOTOR_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define MOTOR_GetRxInterruptSource   MOTOR_ReadRxStatus

#endif /* End (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) */

/* Only if TX is enabled */
#if(MOTOR_TX_ENABLED || MOTOR_HD_ENABLED)

    #if(MOTOR_TX_INTERRUPT_ENABLED)
        #define MOTOR_EnableTxInt()  CyIntEnable (MOTOR_TX_VECT_NUM)
        #define MOTOR_DisableTxInt() CyIntDisable(MOTOR_TX_VECT_NUM)
        #define MOTOR_SetPendingTxInt() CyIntSetPending(MOTOR_TX_VECT_NUM)
        #define MOTOR_ClearPendingTxInt() CyIntClearPending(MOTOR_TX_VECT_NUM)
        CY_ISR_PROTO(MOTOR_TXISR);
    #endif /* MOTOR_TX_INTERRUPT_ENABLED */

    void MOTOR_SetTxInterruptMode(uint8 intSrc) ;
    void MOTOR_WriteTxData(uint8 txDataByte) ;
    uint8 MOTOR_ReadTxStatus(void) ;
    void MOTOR_PutChar(uint8 txDataByte) ;
    void MOTOR_PutString(const char8 string[]) ;
    void MOTOR_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void MOTOR_PutCRLF(uint8 txDataByte) ;
    void MOTOR_ClearTxBuffer(void) ;
    void MOTOR_SetTxAddressMode(uint8 addressMode) ;
    void MOTOR_SendBreak(uint8 retMode) ;
    uint8 MOTOR_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define MOTOR_PutStringConst         MOTOR_PutString
    #define MOTOR_PutArrayConst          MOTOR_PutArray
    #define MOTOR_GetTxInterruptSource   MOTOR_ReadTxStatus

#endif /* End MOTOR_TX_ENABLED || MOTOR_HD_ENABLED */

#if(MOTOR_HD_ENABLED)
    void MOTOR_LoadRxConfig(void) ;
    void MOTOR_LoadTxConfig(void) ;
#endif /* End MOTOR_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_MOTOR) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    MOTOR_CyBtldrCommStart(void) CYSMALL ;
    void    MOTOR_CyBtldrCommStop(void) CYSMALL ;
    void    MOTOR_CyBtldrCommReset(void) CYSMALL ;
    cystatus MOTOR_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus MOTOR_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_MOTOR)
        #define CyBtldrCommStart    MOTOR_CyBtldrCommStart
        #define CyBtldrCommStop     MOTOR_CyBtldrCommStop
        #define CyBtldrCommReset    MOTOR_CyBtldrCommReset
        #define CyBtldrCommWrite    MOTOR_CyBtldrCommWrite
        #define CyBtldrCommRead     MOTOR_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_MOTOR) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define MOTOR_BYTE2BYTE_TIME_OUT (25u)
    #define MOTOR_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define MOTOR_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define MOTOR_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define MOTOR_SET_SPACE      (0x00u)
#define MOTOR_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (MOTOR_TX_ENABLED) || (MOTOR_HD_ENABLED) )
    #if(MOTOR_TX_INTERRUPT_ENABLED)
        #define MOTOR_TX_VECT_NUM            (uint8)MOTOR_TXInternalInterrupt__INTC_NUMBER
        #define MOTOR_TX_PRIOR_NUM           (uint8)MOTOR_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* MOTOR_TX_INTERRUPT_ENABLED */

    #define MOTOR_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define MOTOR_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define MOTOR_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(MOTOR_TX_ENABLED)
        #define MOTOR_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (MOTOR_HD_ENABLED) */
        #define MOTOR_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (MOTOR_TX_ENABLED) */

    #define MOTOR_TX_STS_COMPLETE            (uint8)(0x01u << MOTOR_TX_STS_COMPLETE_SHIFT)
    #define MOTOR_TX_STS_FIFO_EMPTY          (uint8)(0x01u << MOTOR_TX_STS_FIFO_EMPTY_SHIFT)
    #define MOTOR_TX_STS_FIFO_FULL           (uint8)(0x01u << MOTOR_TX_STS_FIFO_FULL_SHIFT)
    #define MOTOR_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << MOTOR_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (MOTOR_TX_ENABLED) || (MOTOR_HD_ENABLED)*/

#if( (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) )
    #if(MOTOR_RX_INTERRUPT_ENABLED)
        #define MOTOR_RX_VECT_NUM            (uint8)MOTOR_RXInternalInterrupt__INTC_NUMBER
        #define MOTOR_RX_PRIOR_NUM           (uint8)MOTOR_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* MOTOR_RX_INTERRUPT_ENABLED */
    #define MOTOR_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define MOTOR_RX_STS_BREAK_SHIFT             (0x01u)
    #define MOTOR_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define MOTOR_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define MOTOR_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define MOTOR_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define MOTOR_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define MOTOR_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define MOTOR_RX_STS_MRKSPC           (uint8)(0x01u << MOTOR_RX_STS_MRKSPC_SHIFT)
    #define MOTOR_RX_STS_BREAK            (uint8)(0x01u << MOTOR_RX_STS_BREAK_SHIFT)
    #define MOTOR_RX_STS_PAR_ERROR        (uint8)(0x01u << MOTOR_RX_STS_PAR_ERROR_SHIFT)
    #define MOTOR_RX_STS_STOP_ERROR       (uint8)(0x01u << MOTOR_RX_STS_STOP_ERROR_SHIFT)
    #define MOTOR_RX_STS_OVERRUN          (uint8)(0x01u << MOTOR_RX_STS_OVERRUN_SHIFT)
    #define MOTOR_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << MOTOR_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define MOTOR_RX_STS_ADDR_MATCH       (uint8)(0x01u << MOTOR_RX_STS_ADDR_MATCH_SHIFT)
    #define MOTOR_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << MOTOR_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define MOTOR_RX_HW_MASK                     (0x7Fu)
#endif /* End (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) */

/* Control Register definitions */
#define MOTOR_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define MOTOR_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define MOTOR_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define MOTOR_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define MOTOR_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define MOTOR_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define MOTOR_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define MOTOR_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define MOTOR_CTRL_HD_SEND               (uint8)(0x01u << MOTOR_CTRL_HD_SEND_SHIFT)
#define MOTOR_CTRL_HD_SEND_BREAK         (uint8)(0x01u << MOTOR_CTRL_HD_SEND_BREAK_SHIFT)
#define MOTOR_CTRL_MARK                  (uint8)(0x01u << MOTOR_CTRL_MARK_SHIFT)
#define MOTOR_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << MOTOR_CTRL_PARITY_TYPE0_SHIFT)
#define MOTOR_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << MOTOR_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define MOTOR_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define MOTOR_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define MOTOR_SEND_BREAK                         (0x00u)
#define MOTOR_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define MOTOR_REINIT                             (0x02u)
#define MOTOR_SEND_WAIT_REINIT                   (0x03u)

#define MOTOR_OVER_SAMPLE_8                      (8u)
#define MOTOR_OVER_SAMPLE_16                     (16u)

#define MOTOR_BIT_CENTER                         (MOTOR_OVER_SAMPLE_COUNT - 2u)

#define MOTOR_FIFO_LENGTH                        (4u)
#define MOTOR_NUMBER_OF_START_BIT                (1u)
#define MOTOR_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define MOTOR_TXBITCTR_BREAKBITS8X   ((MOTOR_BREAK_BITS_TX * MOTOR_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define MOTOR_TXBITCTR_BREAKBITS ((MOTOR_BREAK_BITS_TX * MOTOR_OVER_SAMPLE_COUNT) - 1u)

#define MOTOR_HALF_BIT_COUNT   \
                            (((MOTOR_OVER_SAMPLE_COUNT / 2u) + (MOTOR_USE23POLLING * 1u)) - 2u)
#if (MOTOR_OVER_SAMPLE_COUNT == MOTOR_OVER_SAMPLE_8)
    #define MOTOR_HD_TXBITCTR_INIT   (((MOTOR_BREAK_BITS_TX + \
                            MOTOR_NUMBER_OF_START_BIT) * MOTOR_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define MOTOR_RXBITCTR_INIT  ((((MOTOR_BREAK_BITS_RX + MOTOR_NUMBER_OF_START_BIT) \
                            * MOTOR_OVER_SAMPLE_COUNT) + MOTOR_HALF_BIT_COUNT) - 1u)

#else /* MOTOR_OVER_SAMPLE_COUNT == MOTOR_OVER_SAMPLE_16 */
    #define MOTOR_HD_TXBITCTR_INIT   ((8u * MOTOR_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define MOTOR_RXBITCTR_INIT      (((7u * MOTOR_OVER_SAMPLE_COUNT) - 1u) + \
                                                      MOTOR_HALF_BIT_COUNT)
#endif /* End MOTOR_OVER_SAMPLE_COUNT */

#define MOTOR_HD_RXBITCTR_INIT                   MOTOR_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 MOTOR_initVar;
#if (MOTOR_TX_INTERRUPT_ENABLED && MOTOR_TX_ENABLED)
    extern volatile uint8 MOTOR_txBuffer[MOTOR_TX_BUFFER_SIZE];
    extern volatile uint8 MOTOR_txBufferRead;
    extern uint8 MOTOR_txBufferWrite;
#endif /* (MOTOR_TX_INTERRUPT_ENABLED && MOTOR_TX_ENABLED) */
#if (MOTOR_RX_INTERRUPT_ENABLED && (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED))
    extern uint8 MOTOR_errorStatus;
    extern volatile uint8 MOTOR_rxBuffer[MOTOR_RX_BUFFER_SIZE];
    extern volatile uint8 MOTOR_rxBufferRead;
    extern volatile uint8 MOTOR_rxBufferWrite;
    extern volatile uint8 MOTOR_rxBufferLoopDetect;
    extern volatile uint8 MOTOR_rxBufferOverflow;
    #if (MOTOR_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 MOTOR_rxAddressMode;
        extern volatile uint8 MOTOR_rxAddressDetected;
    #endif /* (MOTOR_RXHW_ADDRESS_ENABLED) */
#endif /* (MOTOR_RX_INTERRUPT_ENABLED && (MOTOR_RX_ENABLED || MOTOR_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define MOTOR__B_UART__AM_SW_BYTE_BYTE 1
#define MOTOR__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define MOTOR__B_UART__AM_HW_BYTE_BY_BYTE 3
#define MOTOR__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define MOTOR__B_UART__AM_NONE 0

#define MOTOR__B_UART__NONE_REVB 0
#define MOTOR__B_UART__EVEN_REVB 1
#define MOTOR__B_UART__ODD_REVB 2
#define MOTOR__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define MOTOR_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define MOTOR_NUMBER_OF_STOP_BITS    (1u)

#if (MOTOR_RXHW_ADDRESS_ENABLED)
    #define MOTOR_RX_ADDRESS_MODE    (0u)
    #define MOTOR_RX_HW_ADDRESS1     (0u)
    #define MOTOR_RX_HW_ADDRESS2     (0u)
#endif /* (MOTOR_RXHW_ADDRESS_ENABLED) */

#define MOTOR_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((0 << MOTOR_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << MOTOR_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << MOTOR_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << MOTOR_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << MOTOR_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << MOTOR_RX_STS_BREAK_SHIFT) \
                                        | (0 << MOTOR_RX_STS_OVERRUN_SHIFT))

#define MOTOR_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << MOTOR_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << MOTOR_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << MOTOR_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << MOTOR_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef MOTOR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define MOTOR_CONTROL_REG \
                            (* (reg8 *) MOTOR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define MOTOR_CONTROL_PTR \
                            (  (reg8 *) MOTOR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End MOTOR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(MOTOR_TX_ENABLED)
    #define MOTOR_TXDATA_REG          (* (reg8 *) MOTOR_BUART_sTX_TxShifter_u0__F0_REG)
    #define MOTOR_TXDATA_PTR          (  (reg8 *) MOTOR_BUART_sTX_TxShifter_u0__F0_REG)
    #define MOTOR_TXDATA_AUX_CTL_REG  (* (reg8 *) MOTOR_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define MOTOR_TXDATA_AUX_CTL_PTR  (  (reg8 *) MOTOR_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define MOTOR_TXSTATUS_REG        (* (reg8 *) MOTOR_BUART_sTX_TxSts__STATUS_REG)
    #define MOTOR_TXSTATUS_PTR        (  (reg8 *) MOTOR_BUART_sTX_TxSts__STATUS_REG)
    #define MOTOR_TXSTATUS_MASK_REG   (* (reg8 *) MOTOR_BUART_sTX_TxSts__MASK_REG)
    #define MOTOR_TXSTATUS_MASK_PTR   (  (reg8 *) MOTOR_BUART_sTX_TxSts__MASK_REG)
    #define MOTOR_TXSTATUS_ACTL_REG   (* (reg8 *) MOTOR_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define MOTOR_TXSTATUS_ACTL_PTR   (  (reg8 *) MOTOR_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(MOTOR_TXCLKGEN_DP)
        #define MOTOR_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define MOTOR_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define MOTOR_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define MOTOR_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define MOTOR_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define MOTOR_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define MOTOR_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define MOTOR_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define MOTOR_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define MOTOR_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) MOTOR_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* MOTOR_TXCLKGEN_DP */

#endif /* End MOTOR_TX_ENABLED */

#if(MOTOR_HD_ENABLED)

    #define MOTOR_TXDATA_REG             (* (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__F1_REG )
    #define MOTOR_TXDATA_PTR             (  (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__F1_REG )
    #define MOTOR_TXDATA_AUX_CTL_REG     (* (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define MOTOR_TXDATA_AUX_CTL_PTR     (  (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define MOTOR_TXSTATUS_REG           (* (reg8 *) MOTOR_BUART_sRX_RxSts__STATUS_REG )
    #define MOTOR_TXSTATUS_PTR           (  (reg8 *) MOTOR_BUART_sRX_RxSts__STATUS_REG )
    #define MOTOR_TXSTATUS_MASK_REG      (* (reg8 *) MOTOR_BUART_sRX_RxSts__MASK_REG )
    #define MOTOR_TXSTATUS_MASK_PTR      (  (reg8 *) MOTOR_BUART_sRX_RxSts__MASK_REG )
    #define MOTOR_TXSTATUS_ACTL_REG      (* (reg8 *) MOTOR_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define MOTOR_TXSTATUS_ACTL_PTR      (  (reg8 *) MOTOR_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End MOTOR_HD_ENABLED */

#if( (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) )
    #define MOTOR_RXDATA_REG             (* (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__F0_REG )
    #define MOTOR_RXDATA_PTR             (  (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__F0_REG )
    #define MOTOR_RXADDRESS1_REG         (* (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__D0_REG )
    #define MOTOR_RXADDRESS1_PTR         (  (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__D0_REG )
    #define MOTOR_RXADDRESS2_REG         (* (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__D1_REG )
    #define MOTOR_RXADDRESS2_PTR         (  (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__D1_REG )
    #define MOTOR_RXDATA_AUX_CTL_REG     (* (reg8 *) MOTOR_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define MOTOR_RXBITCTR_PERIOD_REG    (* (reg8 *) MOTOR_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define MOTOR_RXBITCTR_PERIOD_PTR    (  (reg8 *) MOTOR_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define MOTOR_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) MOTOR_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define MOTOR_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) MOTOR_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define MOTOR_RXBITCTR_COUNTER_REG   (* (reg8 *) MOTOR_BUART_sRX_RxBitCounter__COUNT_REG )
    #define MOTOR_RXBITCTR_COUNTER_PTR   (  (reg8 *) MOTOR_BUART_sRX_RxBitCounter__COUNT_REG )

    #define MOTOR_RXSTATUS_REG           (* (reg8 *) MOTOR_BUART_sRX_RxSts__STATUS_REG )
    #define MOTOR_RXSTATUS_PTR           (  (reg8 *) MOTOR_BUART_sRX_RxSts__STATUS_REG )
    #define MOTOR_RXSTATUS_MASK_REG      (* (reg8 *) MOTOR_BUART_sRX_RxSts__MASK_REG )
    #define MOTOR_RXSTATUS_MASK_PTR      (  (reg8 *) MOTOR_BUART_sRX_RxSts__MASK_REG )
    #define MOTOR_RXSTATUS_ACTL_REG      (* (reg8 *) MOTOR_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define MOTOR_RXSTATUS_ACTL_PTR      (  (reg8 *) MOTOR_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) */

#if(MOTOR_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define MOTOR_INTCLOCK_CLKEN_REG     (* (reg8 *) MOTOR_IntClock__PM_ACT_CFG)
    #define MOTOR_INTCLOCK_CLKEN_PTR     (  (reg8 *) MOTOR_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define MOTOR_INTCLOCK_CLKEN_MASK    MOTOR_IntClock__PM_ACT_MSK
#endif /* End MOTOR_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(MOTOR_TX_ENABLED)
    #define MOTOR_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End MOTOR_TX_ENABLED */

#if(MOTOR_HD_ENABLED)
    #define MOTOR_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End MOTOR_HD_ENABLED */

#if( (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) )
    #define MOTOR_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define MOTOR_WAIT_1_MS      MOTOR_BL_CHK_DELAY_MS   

#define MOTOR_TXBUFFERSIZE   MOTOR_TX_BUFFER_SIZE
#define MOTOR_RXBUFFERSIZE   MOTOR_RX_BUFFER_SIZE

#if (MOTOR_RXHW_ADDRESS_ENABLED)
    #define MOTOR_RXADDRESSMODE  MOTOR_RX_ADDRESS_MODE
    #define MOTOR_RXHWADDRESS1   MOTOR_RX_HW_ADDRESS1
    #define MOTOR_RXHWADDRESS2   MOTOR_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define MOTOR_RXAddressMode  MOTOR_RXADDRESSMODE
#endif /* (MOTOR_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define MOTOR_initvar                    MOTOR_initVar

#define MOTOR_RX_Enabled                 MOTOR_RX_ENABLED
#define MOTOR_TX_Enabled                 MOTOR_TX_ENABLED
#define MOTOR_HD_Enabled                 MOTOR_HD_ENABLED
#define MOTOR_RX_IntInterruptEnabled     MOTOR_RX_INTERRUPT_ENABLED
#define MOTOR_TX_IntInterruptEnabled     MOTOR_TX_INTERRUPT_ENABLED
#define MOTOR_InternalClockUsed          MOTOR_INTERNAL_CLOCK_USED
#define MOTOR_RXHW_Address_Enabled       MOTOR_RXHW_ADDRESS_ENABLED
#define MOTOR_OverSampleCount            MOTOR_OVER_SAMPLE_COUNT
#define MOTOR_ParityType                 MOTOR_PARITY_TYPE

#if( MOTOR_TX_ENABLED && (MOTOR_TXBUFFERSIZE > MOTOR_FIFO_LENGTH))
    #define MOTOR_TXBUFFER               MOTOR_txBuffer
    #define MOTOR_TXBUFFERREAD           MOTOR_txBufferRead
    #define MOTOR_TXBUFFERWRITE          MOTOR_txBufferWrite
#endif /* End MOTOR_TX_ENABLED */
#if( ( MOTOR_RX_ENABLED || MOTOR_HD_ENABLED ) && \
     (MOTOR_RXBUFFERSIZE > MOTOR_FIFO_LENGTH) )
    #define MOTOR_RXBUFFER               MOTOR_rxBuffer
    #define MOTOR_RXBUFFERREAD           MOTOR_rxBufferRead
    #define MOTOR_RXBUFFERWRITE          MOTOR_rxBufferWrite
    #define MOTOR_RXBUFFERLOOPDETECT     MOTOR_rxBufferLoopDetect
    #define MOTOR_RXBUFFER_OVERFLOW      MOTOR_rxBufferOverflow
#endif /* End MOTOR_RX_ENABLED */

#ifdef MOTOR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define MOTOR_CONTROL                MOTOR_CONTROL_REG
#endif /* End MOTOR_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(MOTOR_TX_ENABLED)
    #define MOTOR_TXDATA                 MOTOR_TXDATA_REG
    #define MOTOR_TXSTATUS               MOTOR_TXSTATUS_REG
    #define MOTOR_TXSTATUS_MASK          MOTOR_TXSTATUS_MASK_REG
    #define MOTOR_TXSTATUS_ACTL          MOTOR_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(MOTOR_TXCLKGEN_DP)
        #define MOTOR_TXBITCLKGEN_CTR        MOTOR_TXBITCLKGEN_CTR_REG
        #define MOTOR_TXBITCLKTX_COMPLETE    MOTOR_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define MOTOR_TXBITCTR_PERIOD        MOTOR_TXBITCTR_PERIOD_REG
        #define MOTOR_TXBITCTR_CONTROL       MOTOR_TXBITCTR_CONTROL_REG
        #define MOTOR_TXBITCTR_COUNTER       MOTOR_TXBITCTR_COUNTER_REG
    #endif /* MOTOR_TXCLKGEN_DP */
#endif /* End MOTOR_TX_ENABLED */

#if(MOTOR_HD_ENABLED)
    #define MOTOR_TXDATA                 MOTOR_TXDATA_REG
    #define MOTOR_TXSTATUS               MOTOR_TXSTATUS_REG
    #define MOTOR_TXSTATUS_MASK          MOTOR_TXSTATUS_MASK_REG
    #define MOTOR_TXSTATUS_ACTL          MOTOR_TXSTATUS_ACTL_REG
#endif /* End MOTOR_HD_ENABLED */

#if( (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) )
    #define MOTOR_RXDATA                 MOTOR_RXDATA_REG
    #define MOTOR_RXADDRESS1             MOTOR_RXADDRESS1_REG
    #define MOTOR_RXADDRESS2             MOTOR_RXADDRESS2_REG
    #define MOTOR_RXBITCTR_PERIOD        MOTOR_RXBITCTR_PERIOD_REG
    #define MOTOR_RXBITCTR_CONTROL       MOTOR_RXBITCTR_CONTROL_REG
    #define MOTOR_RXBITCTR_COUNTER       MOTOR_RXBITCTR_COUNTER_REG
    #define MOTOR_RXSTATUS               MOTOR_RXSTATUS_REG
    #define MOTOR_RXSTATUS_MASK          MOTOR_RXSTATUS_MASK_REG
    #define MOTOR_RXSTATUS_ACTL          MOTOR_RXSTATUS_ACTL_REG
#endif /* End  (MOTOR_RX_ENABLED) || (MOTOR_HD_ENABLED) */

#if(MOTOR_INTERNAL_CLOCK_USED)
    #define MOTOR_INTCLOCK_CLKEN         MOTOR_INTCLOCK_CLKEN_REG
#endif /* End MOTOR_INTERNAL_CLOCK_USED */

#define MOTOR_WAIT_FOR_COMLETE_REINIT    MOTOR_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_MOTOR_H */


/* [] END OF FILE */
