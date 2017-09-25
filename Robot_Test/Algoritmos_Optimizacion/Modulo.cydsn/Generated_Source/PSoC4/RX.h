/*******************************************************************************
* File Name: RX.h
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


#if !defined(CY_UART_RX_H)
#define CY_UART_RX_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define RX_RX_ENABLED                     (1u)
#define RX_TX_ENABLED                     (0u)
#define RX_HD_ENABLED                     (0u)
#define RX_RX_INTERRUPT_ENABLED           (1u)
#define RX_TX_INTERRUPT_ENABLED           (0u)
#define RX_INTERNAL_CLOCK_USED            (1u)
#define RX_RXHW_ADDRESS_ENABLED           (0u)
#define RX_OVER_SAMPLE_COUNT              (8u)
#define RX_PARITY_TYPE                    (0u)
#define RX_PARITY_TYPE_SW                 (0u)
#define RX_BREAK_DETECT                   (0u)
#define RX_BREAK_BITS_TX                  (13u)
#define RX_BREAK_BITS_RX                  (13u)
#define RX_TXCLKGEN_DP                    (1u)
#define RX_USE23POLLING                   (0u)
#define RX_FLOW_CONTROL                   (0u)
#define RX_CLK_FREQ                       (0u)
#define RX_TX_BUFFER_SIZE                 (4u)
#define RX_RX_BUFFER_SIZE                 (16u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define RX_CONTROL_REG_REMOVED            (0u)
#else
    #define RX_CONTROL_REG_REMOVED            (1u)
#endif /* End RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct RX_backupStruct_
{
    uint8 enableState;

    #if(RX_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End RX_CONTROL_REG_REMOVED */

} RX_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void RX_Start(void) ;
void RX_Stop(void) ;
uint8 RX_ReadControlRegister(void) ;
void RX_WriteControlRegister(uint8 control) ;

void RX_Init(void) ;
void RX_Enable(void) ;
void RX_SaveConfig(void) ;
void RX_RestoreConfig(void) ;
void RX_Sleep(void) ;
void RX_Wakeup(void) ;

/* Only if RX is enabled */
#if( (RX_RX_ENABLED) || (RX_HD_ENABLED) )

    #if (RX_RX_INTERRUPT_ENABLED)
        #define RX_EnableRxInt()  CyIntEnable (RX_RX_VECT_NUM)
        #define RX_DisableRxInt() CyIntDisable(RX_RX_VECT_NUM)
        CY_ISR_PROTO(RX_RXISR);
    #endif /* RX_RX_INTERRUPT_ENABLED */

    void RX_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void RX_SetRxAddress1(uint8 address) ;
    void RX_SetRxAddress2(uint8 address) ;

    void  RX_SetRxInterruptMode(uint8 intSrc) ;
    uint8 RX_ReadRxData(void) ;
    uint8 RX_ReadRxStatus(void) ;
    uint8 RX_GetChar(void) ;
    uint16 RX_GetByte(void) ;
    uint8 RX_GetRxBufferSize(void)
                                                            ;
    void RX_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define RX_GetRxInterruptSource   RX_ReadRxStatus

#endif /* End (RX_RX_ENABLED) || (RX_HD_ENABLED) */

/* Only if TX is enabled */
#if(RX_TX_ENABLED || RX_HD_ENABLED)

    #if(RX_TX_INTERRUPT_ENABLED)
        #define RX_EnableTxInt()  CyIntEnable (RX_TX_VECT_NUM)
        #define RX_DisableTxInt() CyIntDisable(RX_TX_VECT_NUM)
        #define RX_SetPendingTxInt() CyIntSetPending(RX_TX_VECT_NUM)
        #define RX_ClearPendingTxInt() CyIntClearPending(RX_TX_VECT_NUM)
        CY_ISR_PROTO(RX_TXISR);
    #endif /* RX_TX_INTERRUPT_ENABLED */

    void RX_SetTxInterruptMode(uint8 intSrc) ;
    void RX_WriteTxData(uint8 txDataByte) ;
    uint8 RX_ReadTxStatus(void) ;
    void RX_PutChar(uint8 txDataByte) ;
    void RX_PutString(const char8 string[]) ;
    void RX_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void RX_PutCRLF(uint8 txDataByte) ;
    void RX_ClearTxBuffer(void) ;
    void RX_SetTxAddressMode(uint8 addressMode) ;
    void RX_SendBreak(uint8 retMode) ;
    uint8 RX_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define RX_PutStringConst         RX_PutString
    #define RX_PutArrayConst          RX_PutArray
    #define RX_GetTxInterruptSource   RX_ReadTxStatus

#endif /* End RX_TX_ENABLED || RX_HD_ENABLED */

#if(RX_HD_ENABLED)
    void RX_LoadRxConfig(void) ;
    void RX_LoadTxConfig(void) ;
#endif /* End RX_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RX) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    RX_CyBtldrCommStart(void) CYSMALL ;
    void    RX_CyBtldrCommStop(void) CYSMALL ;
    void    RX_CyBtldrCommReset(void) CYSMALL ;
    cystatus RX_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus RX_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RX)
        #define CyBtldrCommStart    RX_CyBtldrCommStart
        #define CyBtldrCommStop     RX_CyBtldrCommStop
        #define CyBtldrCommReset    RX_CyBtldrCommReset
        #define CyBtldrCommWrite    RX_CyBtldrCommWrite
        #define CyBtldrCommRead     RX_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RX) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define RX_BYTE2BYTE_TIME_OUT (25u)
    #define RX_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define RX_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define RX_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define RX_SET_SPACE      (0x00u)
#define RX_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (RX_TX_ENABLED) || (RX_HD_ENABLED) )
    #if(RX_TX_INTERRUPT_ENABLED)
        #define RX_TX_VECT_NUM            (uint8)RX_TXInternalInterrupt__INTC_NUMBER
        #define RX_TX_PRIOR_NUM           (uint8)RX_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* RX_TX_INTERRUPT_ENABLED */

    #define RX_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define RX_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define RX_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(RX_TX_ENABLED)
        #define RX_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (RX_HD_ENABLED) */
        #define RX_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (RX_TX_ENABLED) */

    #define RX_TX_STS_COMPLETE            (uint8)(0x01u << RX_TX_STS_COMPLETE_SHIFT)
    #define RX_TX_STS_FIFO_EMPTY          (uint8)(0x01u << RX_TX_STS_FIFO_EMPTY_SHIFT)
    #define RX_TX_STS_FIFO_FULL           (uint8)(0x01u << RX_TX_STS_FIFO_FULL_SHIFT)
    #define RX_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << RX_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (RX_TX_ENABLED) || (RX_HD_ENABLED)*/

#if( (RX_RX_ENABLED) || (RX_HD_ENABLED) )
    #if(RX_RX_INTERRUPT_ENABLED)
        #define RX_RX_VECT_NUM            (uint8)RX_RXInternalInterrupt__INTC_NUMBER
        #define RX_RX_PRIOR_NUM           (uint8)RX_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* RX_RX_INTERRUPT_ENABLED */
    #define RX_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define RX_RX_STS_BREAK_SHIFT             (0x01u)
    #define RX_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define RX_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define RX_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define RX_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define RX_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define RX_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define RX_RX_STS_MRKSPC           (uint8)(0x01u << RX_RX_STS_MRKSPC_SHIFT)
    #define RX_RX_STS_BREAK            (uint8)(0x01u << RX_RX_STS_BREAK_SHIFT)
    #define RX_RX_STS_PAR_ERROR        (uint8)(0x01u << RX_RX_STS_PAR_ERROR_SHIFT)
    #define RX_RX_STS_STOP_ERROR       (uint8)(0x01u << RX_RX_STS_STOP_ERROR_SHIFT)
    #define RX_RX_STS_OVERRUN          (uint8)(0x01u << RX_RX_STS_OVERRUN_SHIFT)
    #define RX_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << RX_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define RX_RX_STS_ADDR_MATCH       (uint8)(0x01u << RX_RX_STS_ADDR_MATCH_SHIFT)
    #define RX_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << RX_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define RX_RX_HW_MASK                     (0x7Fu)
#endif /* End (RX_RX_ENABLED) || (RX_HD_ENABLED) */

/* Control Register definitions */
#define RX_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define RX_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define RX_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define RX_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define RX_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define RX_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define RX_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define RX_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define RX_CTRL_HD_SEND               (uint8)(0x01u << RX_CTRL_HD_SEND_SHIFT)
#define RX_CTRL_HD_SEND_BREAK         (uint8)(0x01u << RX_CTRL_HD_SEND_BREAK_SHIFT)
#define RX_CTRL_MARK                  (uint8)(0x01u << RX_CTRL_MARK_SHIFT)
#define RX_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << RX_CTRL_PARITY_TYPE0_SHIFT)
#define RX_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << RX_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define RX_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define RX_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define RX_SEND_BREAK                         (0x00u)
#define RX_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define RX_REINIT                             (0x02u)
#define RX_SEND_WAIT_REINIT                   (0x03u)

#define RX_OVER_SAMPLE_8                      (8u)
#define RX_OVER_SAMPLE_16                     (16u)

#define RX_BIT_CENTER                         (RX_OVER_SAMPLE_COUNT - 2u)

#define RX_FIFO_LENGTH                        (4u)
#define RX_NUMBER_OF_START_BIT                (1u)
#define RX_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define RX_TXBITCTR_BREAKBITS8X   ((RX_BREAK_BITS_TX * RX_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define RX_TXBITCTR_BREAKBITS ((RX_BREAK_BITS_TX * RX_OVER_SAMPLE_COUNT) - 1u)

#define RX_HALF_BIT_COUNT   \
                            (((RX_OVER_SAMPLE_COUNT / 2u) + (RX_USE23POLLING * 1u)) - 2u)
#if (RX_OVER_SAMPLE_COUNT == RX_OVER_SAMPLE_8)
    #define RX_HD_TXBITCTR_INIT   (((RX_BREAK_BITS_TX + \
                            RX_NUMBER_OF_START_BIT) * RX_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define RX_RXBITCTR_INIT  ((((RX_BREAK_BITS_RX + RX_NUMBER_OF_START_BIT) \
                            * RX_OVER_SAMPLE_COUNT) + RX_HALF_BIT_COUNT) - 1u)

#else /* RX_OVER_SAMPLE_COUNT == RX_OVER_SAMPLE_16 */
    #define RX_HD_TXBITCTR_INIT   ((8u * RX_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define RX_RXBITCTR_INIT      (((7u * RX_OVER_SAMPLE_COUNT) - 1u) + \
                                                      RX_HALF_BIT_COUNT)
#endif /* End RX_OVER_SAMPLE_COUNT */

#define RX_HD_RXBITCTR_INIT                   RX_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 RX_initVar;
#if (RX_TX_INTERRUPT_ENABLED && RX_TX_ENABLED)
    extern volatile uint8 RX_txBuffer[RX_TX_BUFFER_SIZE];
    extern volatile uint8 RX_txBufferRead;
    extern uint8 RX_txBufferWrite;
#endif /* (RX_TX_INTERRUPT_ENABLED && RX_TX_ENABLED) */
#if (RX_RX_INTERRUPT_ENABLED && (RX_RX_ENABLED || RX_HD_ENABLED))
    extern uint8 RX_errorStatus;
    extern volatile uint8 RX_rxBuffer[RX_RX_BUFFER_SIZE];
    extern volatile uint8 RX_rxBufferRead;
    extern volatile uint8 RX_rxBufferWrite;
    extern volatile uint8 RX_rxBufferLoopDetect;
    extern volatile uint8 RX_rxBufferOverflow;
    #if (RX_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 RX_rxAddressMode;
        extern volatile uint8 RX_rxAddressDetected;
    #endif /* (RX_RXHW_ADDRESS_ENABLED) */
#endif /* (RX_RX_INTERRUPT_ENABLED && (RX_RX_ENABLED || RX_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define RX__B_UART__AM_SW_BYTE_BYTE 1
#define RX__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define RX__B_UART__AM_HW_BYTE_BY_BYTE 3
#define RX__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define RX__B_UART__AM_NONE 0

#define RX__B_UART__NONE_REVB 0
#define RX__B_UART__EVEN_REVB 1
#define RX__B_UART__ODD_REVB 2
#define RX__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define RX_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define RX_NUMBER_OF_STOP_BITS    (1u)

#if (RX_RXHW_ADDRESS_ENABLED)
    #define RX_RX_ADDRESS_MODE    (0u)
    #define RX_RX_HW_ADDRESS1     (0u)
    #define RX_RX_HW_ADDRESS2     (0u)
#endif /* (RX_RXHW_ADDRESS_ENABLED) */

#define RX_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << RX_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << RX_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << RX_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << RX_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << RX_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << RX_RX_STS_BREAK_SHIFT) \
                                        | (0 << RX_RX_STS_OVERRUN_SHIFT))

#define RX_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << RX_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << RX_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << RX_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << RX_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define RX_CONTROL_REG \
                            (* (reg8 *) RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define RX_CONTROL_PTR \
                            (  (reg8 *) RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(RX_TX_ENABLED)
    #define RX_TXDATA_REG          (* (reg8 *) RX_BUART_sTX_TxShifter_u0__F0_REG)
    #define RX_TXDATA_PTR          (  (reg8 *) RX_BUART_sTX_TxShifter_u0__F0_REG)
    #define RX_TXDATA_AUX_CTL_REG  (* (reg8 *) RX_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define RX_TXDATA_AUX_CTL_PTR  (  (reg8 *) RX_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define RX_TXSTATUS_REG        (* (reg8 *) RX_BUART_sTX_TxSts__STATUS_REG)
    #define RX_TXSTATUS_PTR        (  (reg8 *) RX_BUART_sTX_TxSts__STATUS_REG)
    #define RX_TXSTATUS_MASK_REG   (* (reg8 *) RX_BUART_sTX_TxSts__MASK_REG)
    #define RX_TXSTATUS_MASK_PTR   (  (reg8 *) RX_BUART_sTX_TxSts__MASK_REG)
    #define RX_TXSTATUS_ACTL_REG   (* (reg8 *) RX_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define RX_TXSTATUS_ACTL_PTR   (  (reg8 *) RX_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(RX_TXCLKGEN_DP)
        #define RX_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) RX_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define RX_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) RX_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define RX_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) RX_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define RX_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) RX_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define RX_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) RX_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define RX_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) RX_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define RX_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) RX_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define RX_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) RX_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define RX_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) RX_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define RX_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) RX_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* RX_TXCLKGEN_DP */

#endif /* End RX_TX_ENABLED */

#if(RX_HD_ENABLED)

    #define RX_TXDATA_REG             (* (reg8 *) RX_BUART_sRX_RxShifter_u0__F1_REG )
    #define RX_TXDATA_PTR             (  (reg8 *) RX_BUART_sRX_RxShifter_u0__F1_REG )
    #define RX_TXDATA_AUX_CTL_REG     (* (reg8 *) RX_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define RX_TXDATA_AUX_CTL_PTR     (  (reg8 *) RX_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define RX_TXSTATUS_REG           (* (reg8 *) RX_BUART_sRX_RxSts__STATUS_REG )
    #define RX_TXSTATUS_PTR           (  (reg8 *) RX_BUART_sRX_RxSts__STATUS_REG )
    #define RX_TXSTATUS_MASK_REG      (* (reg8 *) RX_BUART_sRX_RxSts__MASK_REG )
    #define RX_TXSTATUS_MASK_PTR      (  (reg8 *) RX_BUART_sRX_RxSts__MASK_REG )
    #define RX_TXSTATUS_ACTL_REG      (* (reg8 *) RX_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define RX_TXSTATUS_ACTL_PTR      (  (reg8 *) RX_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End RX_HD_ENABLED */

#if( (RX_RX_ENABLED) || (RX_HD_ENABLED) )
    #define RX_RXDATA_REG             (* (reg8 *) RX_BUART_sRX_RxShifter_u0__F0_REG )
    #define RX_RXDATA_PTR             (  (reg8 *) RX_BUART_sRX_RxShifter_u0__F0_REG )
    #define RX_RXADDRESS1_REG         (* (reg8 *) RX_BUART_sRX_RxShifter_u0__D0_REG )
    #define RX_RXADDRESS1_PTR         (  (reg8 *) RX_BUART_sRX_RxShifter_u0__D0_REG )
    #define RX_RXADDRESS2_REG         (* (reg8 *) RX_BUART_sRX_RxShifter_u0__D1_REG )
    #define RX_RXADDRESS2_PTR         (  (reg8 *) RX_BUART_sRX_RxShifter_u0__D1_REG )
    #define RX_RXDATA_AUX_CTL_REG     (* (reg8 *) RX_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define RX_RXBITCTR_PERIOD_REG    (* (reg8 *) RX_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define RX_RXBITCTR_PERIOD_PTR    (  (reg8 *) RX_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define RX_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) RX_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define RX_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) RX_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define RX_RXBITCTR_COUNTER_REG   (* (reg8 *) RX_BUART_sRX_RxBitCounter__COUNT_REG )
    #define RX_RXBITCTR_COUNTER_PTR   (  (reg8 *) RX_BUART_sRX_RxBitCounter__COUNT_REG )

    #define RX_RXSTATUS_REG           (* (reg8 *) RX_BUART_sRX_RxSts__STATUS_REG )
    #define RX_RXSTATUS_PTR           (  (reg8 *) RX_BUART_sRX_RxSts__STATUS_REG )
    #define RX_RXSTATUS_MASK_REG      (* (reg8 *) RX_BUART_sRX_RxSts__MASK_REG )
    #define RX_RXSTATUS_MASK_PTR      (  (reg8 *) RX_BUART_sRX_RxSts__MASK_REG )
    #define RX_RXSTATUS_ACTL_REG      (* (reg8 *) RX_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define RX_RXSTATUS_ACTL_PTR      (  (reg8 *) RX_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (RX_RX_ENABLED) || (RX_HD_ENABLED) */

#if(RX_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define RX_INTCLOCK_CLKEN_REG     (* (reg8 *) RX_IntClock__PM_ACT_CFG)
    #define RX_INTCLOCK_CLKEN_PTR     (  (reg8 *) RX_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define RX_INTCLOCK_CLKEN_MASK    RX_IntClock__PM_ACT_MSK
#endif /* End RX_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(RX_TX_ENABLED)
    #define RX_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End RX_TX_ENABLED */

#if(RX_HD_ENABLED)
    #define RX_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End RX_HD_ENABLED */

#if( (RX_RX_ENABLED) || (RX_HD_ENABLED) )
    #define RX_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (RX_RX_ENABLED) || (RX_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define RX_WAIT_1_MS      RX_BL_CHK_DELAY_MS   

#define RX_TXBUFFERSIZE   RX_TX_BUFFER_SIZE
#define RX_RXBUFFERSIZE   RX_RX_BUFFER_SIZE

#if (RX_RXHW_ADDRESS_ENABLED)
    #define RX_RXADDRESSMODE  RX_RX_ADDRESS_MODE
    #define RX_RXHWADDRESS1   RX_RX_HW_ADDRESS1
    #define RX_RXHWADDRESS2   RX_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define RX_RXAddressMode  RX_RXADDRESSMODE
#endif /* (RX_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define RX_initvar                    RX_initVar

#define RX_RX_Enabled                 RX_RX_ENABLED
#define RX_TX_Enabled                 RX_TX_ENABLED
#define RX_HD_Enabled                 RX_HD_ENABLED
#define RX_RX_IntInterruptEnabled     RX_RX_INTERRUPT_ENABLED
#define RX_TX_IntInterruptEnabled     RX_TX_INTERRUPT_ENABLED
#define RX_InternalClockUsed          RX_INTERNAL_CLOCK_USED
#define RX_RXHW_Address_Enabled       RX_RXHW_ADDRESS_ENABLED
#define RX_OverSampleCount            RX_OVER_SAMPLE_COUNT
#define RX_ParityType                 RX_PARITY_TYPE

#if( RX_TX_ENABLED && (RX_TXBUFFERSIZE > RX_FIFO_LENGTH))
    #define RX_TXBUFFER               RX_txBuffer
    #define RX_TXBUFFERREAD           RX_txBufferRead
    #define RX_TXBUFFERWRITE          RX_txBufferWrite
#endif /* End RX_TX_ENABLED */
#if( ( RX_RX_ENABLED || RX_HD_ENABLED ) && \
     (RX_RXBUFFERSIZE > RX_FIFO_LENGTH) )
    #define RX_RXBUFFER               RX_rxBuffer
    #define RX_RXBUFFERREAD           RX_rxBufferRead
    #define RX_RXBUFFERWRITE          RX_rxBufferWrite
    #define RX_RXBUFFERLOOPDETECT     RX_rxBufferLoopDetect
    #define RX_RXBUFFER_OVERFLOW      RX_rxBufferOverflow
#endif /* End RX_RX_ENABLED */

#ifdef RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define RX_CONTROL                RX_CONTROL_REG
#endif /* End RX_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(RX_TX_ENABLED)
    #define RX_TXDATA                 RX_TXDATA_REG
    #define RX_TXSTATUS               RX_TXSTATUS_REG
    #define RX_TXSTATUS_MASK          RX_TXSTATUS_MASK_REG
    #define RX_TXSTATUS_ACTL          RX_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(RX_TXCLKGEN_DP)
        #define RX_TXBITCLKGEN_CTR        RX_TXBITCLKGEN_CTR_REG
        #define RX_TXBITCLKTX_COMPLETE    RX_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define RX_TXBITCTR_PERIOD        RX_TXBITCTR_PERIOD_REG
        #define RX_TXBITCTR_CONTROL       RX_TXBITCTR_CONTROL_REG
        #define RX_TXBITCTR_COUNTER       RX_TXBITCTR_COUNTER_REG
    #endif /* RX_TXCLKGEN_DP */
#endif /* End RX_TX_ENABLED */

#if(RX_HD_ENABLED)
    #define RX_TXDATA                 RX_TXDATA_REG
    #define RX_TXSTATUS               RX_TXSTATUS_REG
    #define RX_TXSTATUS_MASK          RX_TXSTATUS_MASK_REG
    #define RX_TXSTATUS_ACTL          RX_TXSTATUS_ACTL_REG
#endif /* End RX_HD_ENABLED */

#if( (RX_RX_ENABLED) || (RX_HD_ENABLED) )
    #define RX_RXDATA                 RX_RXDATA_REG
    #define RX_RXADDRESS1             RX_RXADDRESS1_REG
    #define RX_RXADDRESS2             RX_RXADDRESS2_REG
    #define RX_RXBITCTR_PERIOD        RX_RXBITCTR_PERIOD_REG
    #define RX_RXBITCTR_CONTROL       RX_RXBITCTR_CONTROL_REG
    #define RX_RXBITCTR_COUNTER       RX_RXBITCTR_COUNTER_REG
    #define RX_RXSTATUS               RX_RXSTATUS_REG
    #define RX_RXSTATUS_MASK          RX_RXSTATUS_MASK_REG
    #define RX_RXSTATUS_ACTL          RX_RXSTATUS_ACTL_REG
#endif /* End  (RX_RX_ENABLED) || (RX_HD_ENABLED) */

#if(RX_INTERNAL_CLOCK_USED)
    #define RX_INTCLOCK_CLKEN         RX_INTCLOCK_CLKEN_REG
#endif /* End RX_INTERNAL_CLOCK_USED */

#define RX_WAIT_FOR_COMLETE_REINIT    RX_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_RX_H */


/* [] END OF FILE */
