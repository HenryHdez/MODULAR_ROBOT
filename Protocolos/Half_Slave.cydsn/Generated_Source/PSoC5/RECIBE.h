/*******************************************************************************
* File Name: RECIBE.h
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


#if !defined(CY_UART_RECIBE_H)
#define CY_UART_RECIBE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

#define RECIBE_RX_ENABLED                     (0u)
#define RECIBE_TX_ENABLED                     (0u)
#define RECIBE_HD_ENABLED                     (1u)
#define RECIBE_RX_INTERRUPT_ENABLED           (0u)
#define RECIBE_TX_INTERRUPT_ENABLED           (0u)
#define RECIBE_INTERNAL_CLOCK_USED            (1u)
#define RECIBE_RXHW_ADDRESS_ENABLED           (0u)
#define RECIBE_OVER_SAMPLE_COUNT              (8u)
#define RECIBE_PARITY_TYPE                    (0u)
#define RECIBE_PARITY_TYPE_SW                 (0u)
#define RECIBE_BREAK_DETECT                   (0u)
#define RECIBE_BREAK_BITS_TX                  (13u)
#define RECIBE_BREAK_BITS_RX                  (13u)
#define RECIBE_TXCLKGEN_DP                    (1u)
#define RECIBE_USE23POLLING                   (1u)
#define RECIBE_FLOW_CONTROL                   (0u)
#define RECIBE_CLK_FREQ                       (0u)
#define RECIBE_TX_BUFFER_SIZE                 (4u)
#define RECIBE_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(RECIBE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define RECIBE_CONTROL_REG_REMOVED            (0u)
#else
    #define RECIBE_CONTROL_REG_REMOVED            (1u)
#endif /* End RECIBE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct RECIBE_backupStruct_
{
    uint8 enableState;

    #if(RECIBE_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End RECIBE_CONTROL_REG_REMOVED */

} RECIBE_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void RECIBE_Start(void) ;
void RECIBE_Stop(void) ;
uint8 RECIBE_ReadControlRegister(void) ;
void RECIBE_WriteControlRegister(uint8 control) ;

void RECIBE_Init(void) ;
void RECIBE_Enable(void) ;
void RECIBE_SaveConfig(void) ;
void RECIBE_RestoreConfig(void) ;
void RECIBE_Sleep(void) ;
void RECIBE_Wakeup(void) ;

/* Only if RX is enabled */
#if( (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) )

    #if (RECIBE_RX_INTERRUPT_ENABLED)
        #define RECIBE_EnableRxInt()  CyIntEnable (RECIBE_RX_VECT_NUM)
        #define RECIBE_DisableRxInt() CyIntDisable(RECIBE_RX_VECT_NUM)
        CY_ISR_PROTO(RECIBE_RXISR);
    #endif /* RECIBE_RX_INTERRUPT_ENABLED */

    void RECIBE_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void RECIBE_SetRxAddress1(uint8 address) ;
    void RECIBE_SetRxAddress2(uint8 address) ;

    void  RECIBE_SetRxInterruptMode(uint8 intSrc) ;
    uint8 RECIBE_ReadRxData(void) ;
    uint8 RECIBE_ReadRxStatus(void) ;
    uint8 RECIBE_GetChar(void) ;
    uint16 RECIBE_GetByte(void) ;
    uint8 RECIBE_GetRxBufferSize(void)
                                                            ;
    void RECIBE_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define RECIBE_GetRxInterruptSource   RECIBE_ReadRxStatus

#endif /* End (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) */

/* Only if TX is enabled */
#if(RECIBE_TX_ENABLED || RECIBE_HD_ENABLED)

    #if(RECIBE_TX_INTERRUPT_ENABLED)
        #define RECIBE_EnableTxInt()  CyIntEnable (RECIBE_TX_VECT_NUM)
        #define RECIBE_DisableTxInt() CyIntDisable(RECIBE_TX_VECT_NUM)
        #define RECIBE_SetPendingTxInt() CyIntSetPending(RECIBE_TX_VECT_NUM)
        #define RECIBE_ClearPendingTxInt() CyIntClearPending(RECIBE_TX_VECT_NUM)
        CY_ISR_PROTO(RECIBE_TXISR);
    #endif /* RECIBE_TX_INTERRUPT_ENABLED */

    void RECIBE_SetTxInterruptMode(uint8 intSrc) ;
    void RECIBE_WriteTxData(uint8 txDataByte) ;
    uint8 RECIBE_ReadTxStatus(void) ;
    void RECIBE_PutChar(uint8 txDataByte) ;
    void RECIBE_PutString(const char8 string[]) ;
    void RECIBE_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void RECIBE_PutCRLF(uint8 txDataByte) ;
    void RECIBE_ClearTxBuffer(void) ;
    void RECIBE_SetTxAddressMode(uint8 addressMode) ;
    void RECIBE_SendBreak(uint8 retMode) ;
    uint8 RECIBE_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define RECIBE_PutStringConst         RECIBE_PutString
    #define RECIBE_PutArrayConst          RECIBE_PutArray
    #define RECIBE_GetTxInterruptSource   RECIBE_ReadTxStatus

#endif /* End RECIBE_TX_ENABLED || RECIBE_HD_ENABLED */

#if(RECIBE_HD_ENABLED)
    void RECIBE_LoadRxConfig(void) ;
    void RECIBE_LoadTxConfig(void) ;
#endif /* End RECIBE_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RECIBE) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    RECIBE_CyBtldrCommStart(void) CYSMALL ;
    void    RECIBE_CyBtldrCommStop(void) CYSMALL ;
    void    RECIBE_CyBtldrCommReset(void) CYSMALL ;
    cystatus RECIBE_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus RECIBE_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RECIBE)
        #define CyBtldrCommStart    RECIBE_CyBtldrCommStart
        #define CyBtldrCommStop     RECIBE_CyBtldrCommStop
        #define CyBtldrCommReset    RECIBE_CyBtldrCommReset
        #define CyBtldrCommWrite    RECIBE_CyBtldrCommWrite
        #define CyBtldrCommRead     RECIBE_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RECIBE) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define RECIBE_BYTE2BYTE_TIME_OUT (25u)
    #define RECIBE_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define RECIBE_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define RECIBE_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define RECIBE_SET_SPACE      (0x00u)
#define RECIBE_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (RECIBE_TX_ENABLED) || (RECIBE_HD_ENABLED) )
    #if(RECIBE_TX_INTERRUPT_ENABLED)
        #define RECIBE_TX_VECT_NUM            (uint8)RECIBE_TXInternalInterrupt__INTC_NUMBER
        #define RECIBE_TX_PRIOR_NUM           (uint8)RECIBE_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* RECIBE_TX_INTERRUPT_ENABLED */

    #define RECIBE_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define RECIBE_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define RECIBE_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(RECIBE_TX_ENABLED)
        #define RECIBE_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (RECIBE_HD_ENABLED) */
        #define RECIBE_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (RECIBE_TX_ENABLED) */

    #define RECIBE_TX_STS_COMPLETE            (uint8)(0x01u << RECIBE_TX_STS_COMPLETE_SHIFT)
    #define RECIBE_TX_STS_FIFO_EMPTY          (uint8)(0x01u << RECIBE_TX_STS_FIFO_EMPTY_SHIFT)
    #define RECIBE_TX_STS_FIFO_FULL           (uint8)(0x01u << RECIBE_TX_STS_FIFO_FULL_SHIFT)
    #define RECIBE_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << RECIBE_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (RECIBE_TX_ENABLED) || (RECIBE_HD_ENABLED)*/

#if( (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) )
    #if(RECIBE_RX_INTERRUPT_ENABLED)
        #define RECIBE_RX_VECT_NUM            (uint8)RECIBE_RXInternalInterrupt__INTC_NUMBER
        #define RECIBE_RX_PRIOR_NUM           (uint8)RECIBE_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* RECIBE_RX_INTERRUPT_ENABLED */
    #define RECIBE_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define RECIBE_RX_STS_BREAK_SHIFT             (0x01u)
    #define RECIBE_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define RECIBE_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define RECIBE_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define RECIBE_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define RECIBE_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define RECIBE_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define RECIBE_RX_STS_MRKSPC           (uint8)(0x01u << RECIBE_RX_STS_MRKSPC_SHIFT)
    #define RECIBE_RX_STS_BREAK            (uint8)(0x01u << RECIBE_RX_STS_BREAK_SHIFT)
    #define RECIBE_RX_STS_PAR_ERROR        (uint8)(0x01u << RECIBE_RX_STS_PAR_ERROR_SHIFT)
    #define RECIBE_RX_STS_STOP_ERROR       (uint8)(0x01u << RECIBE_RX_STS_STOP_ERROR_SHIFT)
    #define RECIBE_RX_STS_OVERRUN          (uint8)(0x01u << RECIBE_RX_STS_OVERRUN_SHIFT)
    #define RECIBE_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << RECIBE_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define RECIBE_RX_STS_ADDR_MATCH       (uint8)(0x01u << RECIBE_RX_STS_ADDR_MATCH_SHIFT)
    #define RECIBE_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << RECIBE_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define RECIBE_RX_HW_MASK                     (0x7Fu)
#endif /* End (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) */

/* Control Register definitions */
#define RECIBE_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define RECIBE_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define RECIBE_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define RECIBE_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define RECIBE_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define RECIBE_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define RECIBE_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define RECIBE_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define RECIBE_CTRL_HD_SEND               (uint8)(0x01u << RECIBE_CTRL_HD_SEND_SHIFT)
#define RECIBE_CTRL_HD_SEND_BREAK         (uint8)(0x01u << RECIBE_CTRL_HD_SEND_BREAK_SHIFT)
#define RECIBE_CTRL_MARK                  (uint8)(0x01u << RECIBE_CTRL_MARK_SHIFT)
#define RECIBE_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << RECIBE_CTRL_PARITY_TYPE0_SHIFT)
#define RECIBE_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << RECIBE_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define RECIBE_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define RECIBE_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define RECIBE_SEND_BREAK                         (0x00u)
#define RECIBE_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define RECIBE_REINIT                             (0x02u)
#define RECIBE_SEND_WAIT_REINIT                   (0x03u)

#define RECIBE_OVER_SAMPLE_8                      (8u)
#define RECIBE_OVER_SAMPLE_16                     (16u)

#define RECIBE_BIT_CENTER                         (RECIBE_OVER_SAMPLE_COUNT - 2u)

#define RECIBE_FIFO_LENGTH                        (4u)
#define RECIBE_NUMBER_OF_START_BIT                (1u)
#define RECIBE_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define RECIBE_TXBITCTR_BREAKBITS8X   ((RECIBE_BREAK_BITS_TX * RECIBE_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define RECIBE_TXBITCTR_BREAKBITS ((RECIBE_BREAK_BITS_TX * RECIBE_OVER_SAMPLE_COUNT) - 1u)

#define RECIBE_HALF_BIT_COUNT   \
                            (((RECIBE_OVER_SAMPLE_COUNT / 2u) + (RECIBE_USE23POLLING * 1u)) - 2u)
#if (RECIBE_OVER_SAMPLE_COUNT == RECIBE_OVER_SAMPLE_8)
    #define RECIBE_HD_TXBITCTR_INIT   (((RECIBE_BREAK_BITS_TX + \
                            RECIBE_NUMBER_OF_START_BIT) * RECIBE_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define RECIBE_RXBITCTR_INIT  ((((RECIBE_BREAK_BITS_RX + RECIBE_NUMBER_OF_START_BIT) \
                            * RECIBE_OVER_SAMPLE_COUNT) + RECIBE_HALF_BIT_COUNT) - 1u)

#else /* RECIBE_OVER_SAMPLE_COUNT == RECIBE_OVER_SAMPLE_16 */
    #define RECIBE_HD_TXBITCTR_INIT   ((8u * RECIBE_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define RECIBE_RXBITCTR_INIT      (((7u * RECIBE_OVER_SAMPLE_COUNT) - 1u) + \
                                                      RECIBE_HALF_BIT_COUNT)
#endif /* End RECIBE_OVER_SAMPLE_COUNT */

#define RECIBE_HD_RXBITCTR_INIT                   RECIBE_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 RECIBE_initVar;
#if (RECIBE_TX_INTERRUPT_ENABLED && RECIBE_TX_ENABLED)
    extern volatile uint8 RECIBE_txBuffer[RECIBE_TX_BUFFER_SIZE];
    extern volatile uint8 RECIBE_txBufferRead;
    extern uint8 RECIBE_txBufferWrite;
#endif /* (RECIBE_TX_INTERRUPT_ENABLED && RECIBE_TX_ENABLED) */
#if (RECIBE_RX_INTERRUPT_ENABLED && (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED))
    extern uint8 RECIBE_errorStatus;
    extern volatile uint8 RECIBE_rxBuffer[RECIBE_RX_BUFFER_SIZE];
    extern volatile uint8 RECIBE_rxBufferRead;
    extern volatile uint8 RECIBE_rxBufferWrite;
    extern volatile uint8 RECIBE_rxBufferLoopDetect;
    extern volatile uint8 RECIBE_rxBufferOverflow;
    #if (RECIBE_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 RECIBE_rxAddressMode;
        extern volatile uint8 RECIBE_rxAddressDetected;
    #endif /* (RECIBE_RXHW_ADDRESS_ENABLED) */
#endif /* (RECIBE_RX_INTERRUPT_ENABLED && (RECIBE_RX_ENABLED || RECIBE_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define RECIBE__B_UART__AM_SW_BYTE_BYTE 1
#define RECIBE__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define RECIBE__B_UART__AM_HW_BYTE_BY_BYTE 3
#define RECIBE__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define RECIBE__B_UART__AM_NONE 0

#define RECIBE__B_UART__NONE_REVB 0
#define RECIBE__B_UART__EVEN_REVB 1
#define RECIBE__B_UART__ODD_REVB 2
#define RECIBE__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define RECIBE_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define RECIBE_NUMBER_OF_STOP_BITS    (1u)

#if (RECIBE_RXHW_ADDRESS_ENABLED)
    #define RECIBE_RX_ADDRESS_MODE    (0u)
    #define RECIBE_RX_HW_ADDRESS1     (0u)
    #define RECIBE_RX_HW_ADDRESS2     (0u)
#endif /* (RECIBE_RXHW_ADDRESS_ENABLED) */

#define RECIBE_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << RECIBE_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << RECIBE_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << RECIBE_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << RECIBE_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << RECIBE_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << RECIBE_RX_STS_BREAK_SHIFT) \
                                        | (0 << RECIBE_RX_STS_OVERRUN_SHIFT))

#define RECIBE_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << RECIBE_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << RECIBE_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << RECIBE_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << RECIBE_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef RECIBE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define RECIBE_CONTROL_REG \
                            (* (reg8 *) RECIBE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define RECIBE_CONTROL_PTR \
                            (  (reg8 *) RECIBE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End RECIBE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(RECIBE_TX_ENABLED)
    #define RECIBE_TXDATA_REG          (* (reg8 *) RECIBE_BUART_sTX_TxShifter_u0__F0_REG)
    #define RECIBE_TXDATA_PTR          (  (reg8 *) RECIBE_BUART_sTX_TxShifter_u0__F0_REG)
    #define RECIBE_TXDATA_AUX_CTL_REG  (* (reg8 *) RECIBE_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define RECIBE_TXDATA_AUX_CTL_PTR  (  (reg8 *) RECIBE_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define RECIBE_TXSTATUS_REG        (* (reg8 *) RECIBE_BUART_sTX_TxSts__STATUS_REG)
    #define RECIBE_TXSTATUS_PTR        (  (reg8 *) RECIBE_BUART_sTX_TxSts__STATUS_REG)
    #define RECIBE_TXSTATUS_MASK_REG   (* (reg8 *) RECIBE_BUART_sTX_TxSts__MASK_REG)
    #define RECIBE_TXSTATUS_MASK_PTR   (  (reg8 *) RECIBE_BUART_sTX_TxSts__MASK_REG)
    #define RECIBE_TXSTATUS_ACTL_REG   (* (reg8 *) RECIBE_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define RECIBE_TXSTATUS_ACTL_PTR   (  (reg8 *) RECIBE_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(RECIBE_TXCLKGEN_DP)
        #define RECIBE_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define RECIBE_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define RECIBE_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define RECIBE_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define RECIBE_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define RECIBE_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define RECIBE_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define RECIBE_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define RECIBE_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define RECIBE_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) RECIBE_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* RECIBE_TXCLKGEN_DP */

#endif /* End RECIBE_TX_ENABLED */

#if(RECIBE_HD_ENABLED)

    #define RECIBE_TXDATA_REG             (* (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__F1_REG )
    #define RECIBE_TXDATA_PTR             (  (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__F1_REG )
    #define RECIBE_TXDATA_AUX_CTL_REG     (* (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define RECIBE_TXDATA_AUX_CTL_PTR     (  (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define RECIBE_TXSTATUS_REG           (* (reg8 *) RECIBE_BUART_sRX_RxSts__STATUS_REG )
    #define RECIBE_TXSTATUS_PTR           (  (reg8 *) RECIBE_BUART_sRX_RxSts__STATUS_REG )
    #define RECIBE_TXSTATUS_MASK_REG      (* (reg8 *) RECIBE_BUART_sRX_RxSts__MASK_REG )
    #define RECIBE_TXSTATUS_MASK_PTR      (  (reg8 *) RECIBE_BUART_sRX_RxSts__MASK_REG )
    #define RECIBE_TXSTATUS_ACTL_REG      (* (reg8 *) RECIBE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define RECIBE_TXSTATUS_ACTL_PTR      (  (reg8 *) RECIBE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End RECIBE_HD_ENABLED */

#if( (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) )
    #define RECIBE_RXDATA_REG             (* (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__F0_REG )
    #define RECIBE_RXDATA_PTR             (  (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__F0_REG )
    #define RECIBE_RXADDRESS1_REG         (* (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__D0_REG )
    #define RECIBE_RXADDRESS1_PTR         (  (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__D0_REG )
    #define RECIBE_RXADDRESS2_REG         (* (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__D1_REG )
    #define RECIBE_RXADDRESS2_PTR         (  (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__D1_REG )
    #define RECIBE_RXDATA_AUX_CTL_REG     (* (reg8 *) RECIBE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define RECIBE_RXBITCTR_PERIOD_REG    (* (reg8 *) RECIBE_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define RECIBE_RXBITCTR_PERIOD_PTR    (  (reg8 *) RECIBE_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define RECIBE_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) RECIBE_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define RECIBE_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) RECIBE_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define RECIBE_RXBITCTR_COUNTER_REG   (* (reg8 *) RECIBE_BUART_sRX_RxBitCounter__COUNT_REG )
    #define RECIBE_RXBITCTR_COUNTER_PTR   (  (reg8 *) RECIBE_BUART_sRX_RxBitCounter__COUNT_REG )

    #define RECIBE_RXSTATUS_REG           (* (reg8 *) RECIBE_BUART_sRX_RxSts__STATUS_REG )
    #define RECIBE_RXSTATUS_PTR           (  (reg8 *) RECIBE_BUART_sRX_RxSts__STATUS_REG )
    #define RECIBE_RXSTATUS_MASK_REG      (* (reg8 *) RECIBE_BUART_sRX_RxSts__MASK_REG )
    #define RECIBE_RXSTATUS_MASK_PTR      (  (reg8 *) RECIBE_BUART_sRX_RxSts__MASK_REG )
    #define RECIBE_RXSTATUS_ACTL_REG      (* (reg8 *) RECIBE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define RECIBE_RXSTATUS_ACTL_PTR      (  (reg8 *) RECIBE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) */

#if(RECIBE_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define RECIBE_INTCLOCK_CLKEN_REG     (* (reg8 *) RECIBE_IntClock__PM_ACT_CFG)
    #define RECIBE_INTCLOCK_CLKEN_PTR     (  (reg8 *) RECIBE_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define RECIBE_INTCLOCK_CLKEN_MASK    RECIBE_IntClock__PM_ACT_MSK
#endif /* End RECIBE_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(RECIBE_TX_ENABLED)
    #define RECIBE_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End RECIBE_TX_ENABLED */

#if(RECIBE_HD_ENABLED)
    #define RECIBE_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End RECIBE_HD_ENABLED */

#if( (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) )
    #define RECIBE_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define RECIBE_WAIT_1_MS      RECIBE_BL_CHK_DELAY_MS   

#define RECIBE_TXBUFFERSIZE   RECIBE_TX_BUFFER_SIZE
#define RECIBE_RXBUFFERSIZE   RECIBE_RX_BUFFER_SIZE

#if (RECIBE_RXHW_ADDRESS_ENABLED)
    #define RECIBE_RXADDRESSMODE  RECIBE_RX_ADDRESS_MODE
    #define RECIBE_RXHWADDRESS1   RECIBE_RX_HW_ADDRESS1
    #define RECIBE_RXHWADDRESS2   RECIBE_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define RECIBE_RXAddressMode  RECIBE_RXADDRESSMODE
#endif /* (RECIBE_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define RECIBE_initvar                    RECIBE_initVar

#define RECIBE_RX_Enabled                 RECIBE_RX_ENABLED
#define RECIBE_TX_Enabled                 RECIBE_TX_ENABLED
#define RECIBE_HD_Enabled                 RECIBE_HD_ENABLED
#define RECIBE_RX_IntInterruptEnabled     RECIBE_RX_INTERRUPT_ENABLED
#define RECIBE_TX_IntInterruptEnabled     RECIBE_TX_INTERRUPT_ENABLED
#define RECIBE_InternalClockUsed          RECIBE_INTERNAL_CLOCK_USED
#define RECIBE_RXHW_Address_Enabled       RECIBE_RXHW_ADDRESS_ENABLED
#define RECIBE_OverSampleCount            RECIBE_OVER_SAMPLE_COUNT
#define RECIBE_ParityType                 RECIBE_PARITY_TYPE

#if( RECIBE_TX_ENABLED && (RECIBE_TXBUFFERSIZE > RECIBE_FIFO_LENGTH))
    #define RECIBE_TXBUFFER               RECIBE_txBuffer
    #define RECIBE_TXBUFFERREAD           RECIBE_txBufferRead
    #define RECIBE_TXBUFFERWRITE          RECIBE_txBufferWrite
#endif /* End RECIBE_TX_ENABLED */
#if( ( RECIBE_RX_ENABLED || RECIBE_HD_ENABLED ) && \
     (RECIBE_RXBUFFERSIZE > RECIBE_FIFO_LENGTH) )
    #define RECIBE_RXBUFFER               RECIBE_rxBuffer
    #define RECIBE_RXBUFFERREAD           RECIBE_rxBufferRead
    #define RECIBE_RXBUFFERWRITE          RECIBE_rxBufferWrite
    #define RECIBE_RXBUFFERLOOPDETECT     RECIBE_rxBufferLoopDetect
    #define RECIBE_RXBUFFER_OVERFLOW      RECIBE_rxBufferOverflow
#endif /* End RECIBE_RX_ENABLED */

#ifdef RECIBE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define RECIBE_CONTROL                RECIBE_CONTROL_REG
#endif /* End RECIBE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(RECIBE_TX_ENABLED)
    #define RECIBE_TXDATA                 RECIBE_TXDATA_REG
    #define RECIBE_TXSTATUS               RECIBE_TXSTATUS_REG
    #define RECIBE_TXSTATUS_MASK          RECIBE_TXSTATUS_MASK_REG
    #define RECIBE_TXSTATUS_ACTL          RECIBE_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(RECIBE_TXCLKGEN_DP)
        #define RECIBE_TXBITCLKGEN_CTR        RECIBE_TXBITCLKGEN_CTR_REG
        #define RECIBE_TXBITCLKTX_COMPLETE    RECIBE_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define RECIBE_TXBITCTR_PERIOD        RECIBE_TXBITCTR_PERIOD_REG
        #define RECIBE_TXBITCTR_CONTROL       RECIBE_TXBITCTR_CONTROL_REG
        #define RECIBE_TXBITCTR_COUNTER       RECIBE_TXBITCTR_COUNTER_REG
    #endif /* RECIBE_TXCLKGEN_DP */
#endif /* End RECIBE_TX_ENABLED */

#if(RECIBE_HD_ENABLED)
    #define RECIBE_TXDATA                 RECIBE_TXDATA_REG
    #define RECIBE_TXSTATUS               RECIBE_TXSTATUS_REG
    #define RECIBE_TXSTATUS_MASK          RECIBE_TXSTATUS_MASK_REG
    #define RECIBE_TXSTATUS_ACTL          RECIBE_TXSTATUS_ACTL_REG
#endif /* End RECIBE_HD_ENABLED */

#if( (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) )
    #define RECIBE_RXDATA                 RECIBE_RXDATA_REG
    #define RECIBE_RXADDRESS1             RECIBE_RXADDRESS1_REG
    #define RECIBE_RXADDRESS2             RECIBE_RXADDRESS2_REG
    #define RECIBE_RXBITCTR_PERIOD        RECIBE_RXBITCTR_PERIOD_REG
    #define RECIBE_RXBITCTR_CONTROL       RECIBE_RXBITCTR_CONTROL_REG
    #define RECIBE_RXBITCTR_COUNTER       RECIBE_RXBITCTR_COUNTER_REG
    #define RECIBE_RXSTATUS               RECIBE_RXSTATUS_REG
    #define RECIBE_RXSTATUS_MASK          RECIBE_RXSTATUS_MASK_REG
    #define RECIBE_RXSTATUS_ACTL          RECIBE_RXSTATUS_ACTL_REG
#endif /* End  (RECIBE_RX_ENABLED) || (RECIBE_HD_ENABLED) */

#if(RECIBE_INTERNAL_CLOCK_USED)
    #define RECIBE_INTCLOCK_CLKEN         RECIBE_INTCLOCK_CLKEN_REG
#endif /* End RECIBE_INTERNAL_CLOCK_USED */

#define RECIBE_WAIT_FOR_COMLETE_REINIT    RECIBE_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_RECIBE_H */


/* [] END OF FILE */
