/*******************************************************************************
* File Name: EMITE.h
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


#if !defined(CY_UART_EMITE_H)
#define CY_UART_EMITE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

#define EMITE_RX_ENABLED                     (1u)
#define EMITE_TX_ENABLED                     (1u)
#define EMITE_HD_ENABLED                     (0u)
#define EMITE_RX_INTERRUPT_ENABLED           (0u)
#define EMITE_TX_INTERRUPT_ENABLED           (0u)
#define EMITE_INTERNAL_CLOCK_USED            (1u)
#define EMITE_RXHW_ADDRESS_ENABLED           (0u)
#define EMITE_OVER_SAMPLE_COUNT              (8u)
#define EMITE_PARITY_TYPE                    (0u)
#define EMITE_PARITY_TYPE_SW                 (0u)
#define EMITE_BREAK_DETECT                   (0u)
#define EMITE_BREAK_BITS_TX                  (13u)
#define EMITE_BREAK_BITS_RX                  (13u)
#define EMITE_TXCLKGEN_DP                    (1u)
#define EMITE_USE23POLLING                   (1u)
#define EMITE_FLOW_CONTROL                   (0u)
#define EMITE_CLK_FREQ                       (0u)
#define EMITE_TX_BUFFER_SIZE                 (4u)
#define EMITE_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(EMITE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define EMITE_CONTROL_REG_REMOVED            (0u)
#else
    #define EMITE_CONTROL_REG_REMOVED            (1u)
#endif /* End EMITE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct EMITE_backupStruct_
{
    uint8 enableState;

    #if(EMITE_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End EMITE_CONTROL_REG_REMOVED */

} EMITE_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void EMITE_Start(void) ;
void EMITE_Stop(void) ;
uint8 EMITE_ReadControlRegister(void) ;
void EMITE_WriteControlRegister(uint8 control) ;

void EMITE_Init(void) ;
void EMITE_Enable(void) ;
void EMITE_SaveConfig(void) ;
void EMITE_RestoreConfig(void) ;
void EMITE_Sleep(void) ;
void EMITE_Wakeup(void) ;

/* Only if RX is enabled */
#if( (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) )

    #if (EMITE_RX_INTERRUPT_ENABLED)
        #define EMITE_EnableRxInt()  CyIntEnable (EMITE_RX_VECT_NUM)
        #define EMITE_DisableRxInt() CyIntDisable(EMITE_RX_VECT_NUM)
        CY_ISR_PROTO(EMITE_RXISR);
    #endif /* EMITE_RX_INTERRUPT_ENABLED */

    void EMITE_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void EMITE_SetRxAddress1(uint8 address) ;
    void EMITE_SetRxAddress2(uint8 address) ;

    void  EMITE_SetRxInterruptMode(uint8 intSrc) ;
    uint8 EMITE_ReadRxData(void) ;
    uint8 EMITE_ReadRxStatus(void) ;
    uint8 EMITE_GetChar(void) ;
    uint16 EMITE_GetByte(void) ;
    uint8 EMITE_GetRxBufferSize(void)
                                                            ;
    void EMITE_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define EMITE_GetRxInterruptSource   EMITE_ReadRxStatus

#endif /* End (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) */

/* Only if TX is enabled */
#if(EMITE_TX_ENABLED || EMITE_HD_ENABLED)

    #if(EMITE_TX_INTERRUPT_ENABLED)
        #define EMITE_EnableTxInt()  CyIntEnable (EMITE_TX_VECT_NUM)
        #define EMITE_DisableTxInt() CyIntDisable(EMITE_TX_VECT_NUM)
        #define EMITE_SetPendingTxInt() CyIntSetPending(EMITE_TX_VECT_NUM)
        #define EMITE_ClearPendingTxInt() CyIntClearPending(EMITE_TX_VECT_NUM)
        CY_ISR_PROTO(EMITE_TXISR);
    #endif /* EMITE_TX_INTERRUPT_ENABLED */

    void EMITE_SetTxInterruptMode(uint8 intSrc) ;
    void EMITE_WriteTxData(uint8 txDataByte) ;
    uint8 EMITE_ReadTxStatus(void) ;
    void EMITE_PutChar(uint8 txDataByte) ;
    void EMITE_PutString(const char8 string[]) ;
    void EMITE_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void EMITE_PutCRLF(uint8 txDataByte) ;
    void EMITE_ClearTxBuffer(void) ;
    void EMITE_SetTxAddressMode(uint8 addressMode) ;
    void EMITE_SendBreak(uint8 retMode) ;
    uint8 EMITE_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define EMITE_PutStringConst         EMITE_PutString
    #define EMITE_PutArrayConst          EMITE_PutArray
    #define EMITE_GetTxInterruptSource   EMITE_ReadTxStatus

#endif /* End EMITE_TX_ENABLED || EMITE_HD_ENABLED */

#if(EMITE_HD_ENABLED)
    void EMITE_LoadRxConfig(void) ;
    void EMITE_LoadTxConfig(void) ;
#endif /* End EMITE_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_EMITE) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    EMITE_CyBtldrCommStart(void) CYSMALL ;
    void    EMITE_CyBtldrCommStop(void) CYSMALL ;
    void    EMITE_CyBtldrCommReset(void) CYSMALL ;
    cystatus EMITE_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus EMITE_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_EMITE)
        #define CyBtldrCommStart    EMITE_CyBtldrCommStart
        #define CyBtldrCommStop     EMITE_CyBtldrCommStop
        #define CyBtldrCommReset    EMITE_CyBtldrCommReset
        #define CyBtldrCommWrite    EMITE_CyBtldrCommWrite
        #define CyBtldrCommRead     EMITE_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_EMITE) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define EMITE_BYTE2BYTE_TIME_OUT (25u)
    #define EMITE_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define EMITE_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define EMITE_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define EMITE_SET_SPACE      (0x00u)
#define EMITE_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (EMITE_TX_ENABLED) || (EMITE_HD_ENABLED) )
    #if(EMITE_TX_INTERRUPT_ENABLED)
        #define EMITE_TX_VECT_NUM            (uint8)EMITE_TXInternalInterrupt__INTC_NUMBER
        #define EMITE_TX_PRIOR_NUM           (uint8)EMITE_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* EMITE_TX_INTERRUPT_ENABLED */

    #define EMITE_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define EMITE_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define EMITE_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(EMITE_TX_ENABLED)
        #define EMITE_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (EMITE_HD_ENABLED) */
        #define EMITE_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (EMITE_TX_ENABLED) */

    #define EMITE_TX_STS_COMPLETE            (uint8)(0x01u << EMITE_TX_STS_COMPLETE_SHIFT)
    #define EMITE_TX_STS_FIFO_EMPTY          (uint8)(0x01u << EMITE_TX_STS_FIFO_EMPTY_SHIFT)
    #define EMITE_TX_STS_FIFO_FULL           (uint8)(0x01u << EMITE_TX_STS_FIFO_FULL_SHIFT)
    #define EMITE_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << EMITE_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (EMITE_TX_ENABLED) || (EMITE_HD_ENABLED)*/

#if( (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) )
    #if(EMITE_RX_INTERRUPT_ENABLED)
        #define EMITE_RX_VECT_NUM            (uint8)EMITE_RXInternalInterrupt__INTC_NUMBER
        #define EMITE_RX_PRIOR_NUM           (uint8)EMITE_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* EMITE_RX_INTERRUPT_ENABLED */
    #define EMITE_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define EMITE_RX_STS_BREAK_SHIFT             (0x01u)
    #define EMITE_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define EMITE_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define EMITE_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define EMITE_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define EMITE_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define EMITE_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define EMITE_RX_STS_MRKSPC           (uint8)(0x01u << EMITE_RX_STS_MRKSPC_SHIFT)
    #define EMITE_RX_STS_BREAK            (uint8)(0x01u << EMITE_RX_STS_BREAK_SHIFT)
    #define EMITE_RX_STS_PAR_ERROR        (uint8)(0x01u << EMITE_RX_STS_PAR_ERROR_SHIFT)
    #define EMITE_RX_STS_STOP_ERROR       (uint8)(0x01u << EMITE_RX_STS_STOP_ERROR_SHIFT)
    #define EMITE_RX_STS_OVERRUN          (uint8)(0x01u << EMITE_RX_STS_OVERRUN_SHIFT)
    #define EMITE_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << EMITE_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define EMITE_RX_STS_ADDR_MATCH       (uint8)(0x01u << EMITE_RX_STS_ADDR_MATCH_SHIFT)
    #define EMITE_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << EMITE_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define EMITE_RX_HW_MASK                     (0x7Fu)
#endif /* End (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) */

/* Control Register definitions */
#define EMITE_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define EMITE_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define EMITE_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define EMITE_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define EMITE_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define EMITE_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define EMITE_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define EMITE_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define EMITE_CTRL_HD_SEND               (uint8)(0x01u << EMITE_CTRL_HD_SEND_SHIFT)
#define EMITE_CTRL_HD_SEND_BREAK         (uint8)(0x01u << EMITE_CTRL_HD_SEND_BREAK_SHIFT)
#define EMITE_CTRL_MARK                  (uint8)(0x01u << EMITE_CTRL_MARK_SHIFT)
#define EMITE_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << EMITE_CTRL_PARITY_TYPE0_SHIFT)
#define EMITE_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << EMITE_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define EMITE_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define EMITE_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define EMITE_SEND_BREAK                         (0x00u)
#define EMITE_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define EMITE_REINIT                             (0x02u)
#define EMITE_SEND_WAIT_REINIT                   (0x03u)

#define EMITE_OVER_SAMPLE_8                      (8u)
#define EMITE_OVER_SAMPLE_16                     (16u)

#define EMITE_BIT_CENTER                         (EMITE_OVER_SAMPLE_COUNT - 2u)

#define EMITE_FIFO_LENGTH                        (4u)
#define EMITE_NUMBER_OF_START_BIT                (1u)
#define EMITE_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define EMITE_TXBITCTR_BREAKBITS8X   ((EMITE_BREAK_BITS_TX * EMITE_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define EMITE_TXBITCTR_BREAKBITS ((EMITE_BREAK_BITS_TX * EMITE_OVER_SAMPLE_COUNT) - 1u)

#define EMITE_HALF_BIT_COUNT   \
                            (((EMITE_OVER_SAMPLE_COUNT / 2u) + (EMITE_USE23POLLING * 1u)) - 2u)
#if (EMITE_OVER_SAMPLE_COUNT == EMITE_OVER_SAMPLE_8)
    #define EMITE_HD_TXBITCTR_INIT   (((EMITE_BREAK_BITS_TX + \
                            EMITE_NUMBER_OF_START_BIT) * EMITE_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define EMITE_RXBITCTR_INIT  ((((EMITE_BREAK_BITS_RX + EMITE_NUMBER_OF_START_BIT) \
                            * EMITE_OVER_SAMPLE_COUNT) + EMITE_HALF_BIT_COUNT) - 1u)

#else /* EMITE_OVER_SAMPLE_COUNT == EMITE_OVER_SAMPLE_16 */
    #define EMITE_HD_TXBITCTR_INIT   ((8u * EMITE_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define EMITE_RXBITCTR_INIT      (((7u * EMITE_OVER_SAMPLE_COUNT) - 1u) + \
                                                      EMITE_HALF_BIT_COUNT)
#endif /* End EMITE_OVER_SAMPLE_COUNT */

#define EMITE_HD_RXBITCTR_INIT                   EMITE_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 EMITE_initVar;
#if (EMITE_TX_INTERRUPT_ENABLED && EMITE_TX_ENABLED)
    extern volatile uint8 EMITE_txBuffer[EMITE_TX_BUFFER_SIZE];
    extern volatile uint8 EMITE_txBufferRead;
    extern uint8 EMITE_txBufferWrite;
#endif /* (EMITE_TX_INTERRUPT_ENABLED && EMITE_TX_ENABLED) */
#if (EMITE_RX_INTERRUPT_ENABLED && (EMITE_RX_ENABLED || EMITE_HD_ENABLED))
    extern uint8 EMITE_errorStatus;
    extern volatile uint8 EMITE_rxBuffer[EMITE_RX_BUFFER_SIZE];
    extern volatile uint8 EMITE_rxBufferRead;
    extern volatile uint8 EMITE_rxBufferWrite;
    extern volatile uint8 EMITE_rxBufferLoopDetect;
    extern volatile uint8 EMITE_rxBufferOverflow;
    #if (EMITE_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 EMITE_rxAddressMode;
        extern volatile uint8 EMITE_rxAddressDetected;
    #endif /* (EMITE_RXHW_ADDRESS_ENABLED) */
#endif /* (EMITE_RX_INTERRUPT_ENABLED && (EMITE_RX_ENABLED || EMITE_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define EMITE__B_UART__AM_SW_BYTE_BYTE 1
#define EMITE__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define EMITE__B_UART__AM_HW_BYTE_BY_BYTE 3
#define EMITE__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define EMITE__B_UART__AM_NONE 0

#define EMITE__B_UART__NONE_REVB 0
#define EMITE__B_UART__EVEN_REVB 1
#define EMITE__B_UART__ODD_REVB 2
#define EMITE__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define EMITE_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define EMITE_NUMBER_OF_STOP_BITS    (1u)

#if (EMITE_RXHW_ADDRESS_ENABLED)
    #define EMITE_RX_ADDRESS_MODE    (0u)
    #define EMITE_RX_HW_ADDRESS1     (0u)
    #define EMITE_RX_HW_ADDRESS2     (0u)
#endif /* (EMITE_RXHW_ADDRESS_ENABLED) */

#define EMITE_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((0 << EMITE_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << EMITE_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << EMITE_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << EMITE_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << EMITE_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << EMITE_RX_STS_BREAK_SHIFT) \
                                        | (0 << EMITE_RX_STS_OVERRUN_SHIFT))

#define EMITE_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << EMITE_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << EMITE_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << EMITE_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << EMITE_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef EMITE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define EMITE_CONTROL_REG \
                            (* (reg8 *) EMITE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define EMITE_CONTROL_PTR \
                            (  (reg8 *) EMITE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End EMITE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(EMITE_TX_ENABLED)
    #define EMITE_TXDATA_REG          (* (reg8 *) EMITE_BUART_sTX_TxShifter_u0__F0_REG)
    #define EMITE_TXDATA_PTR          (  (reg8 *) EMITE_BUART_sTX_TxShifter_u0__F0_REG)
    #define EMITE_TXDATA_AUX_CTL_REG  (* (reg8 *) EMITE_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define EMITE_TXDATA_AUX_CTL_PTR  (  (reg8 *) EMITE_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define EMITE_TXSTATUS_REG        (* (reg8 *) EMITE_BUART_sTX_TxSts__STATUS_REG)
    #define EMITE_TXSTATUS_PTR        (  (reg8 *) EMITE_BUART_sTX_TxSts__STATUS_REG)
    #define EMITE_TXSTATUS_MASK_REG   (* (reg8 *) EMITE_BUART_sTX_TxSts__MASK_REG)
    #define EMITE_TXSTATUS_MASK_PTR   (  (reg8 *) EMITE_BUART_sTX_TxSts__MASK_REG)
    #define EMITE_TXSTATUS_ACTL_REG   (* (reg8 *) EMITE_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define EMITE_TXSTATUS_ACTL_PTR   (  (reg8 *) EMITE_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(EMITE_TXCLKGEN_DP)
        #define EMITE_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define EMITE_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define EMITE_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define EMITE_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define EMITE_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define EMITE_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define EMITE_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define EMITE_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define EMITE_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define EMITE_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) EMITE_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* EMITE_TXCLKGEN_DP */

#endif /* End EMITE_TX_ENABLED */

#if(EMITE_HD_ENABLED)

    #define EMITE_TXDATA_REG             (* (reg8 *) EMITE_BUART_sRX_RxShifter_u0__F1_REG )
    #define EMITE_TXDATA_PTR             (  (reg8 *) EMITE_BUART_sRX_RxShifter_u0__F1_REG )
    #define EMITE_TXDATA_AUX_CTL_REG     (* (reg8 *) EMITE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define EMITE_TXDATA_AUX_CTL_PTR     (  (reg8 *) EMITE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define EMITE_TXSTATUS_REG           (* (reg8 *) EMITE_BUART_sRX_RxSts__STATUS_REG )
    #define EMITE_TXSTATUS_PTR           (  (reg8 *) EMITE_BUART_sRX_RxSts__STATUS_REG )
    #define EMITE_TXSTATUS_MASK_REG      (* (reg8 *) EMITE_BUART_sRX_RxSts__MASK_REG )
    #define EMITE_TXSTATUS_MASK_PTR      (  (reg8 *) EMITE_BUART_sRX_RxSts__MASK_REG )
    #define EMITE_TXSTATUS_ACTL_REG      (* (reg8 *) EMITE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define EMITE_TXSTATUS_ACTL_PTR      (  (reg8 *) EMITE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End EMITE_HD_ENABLED */

#if( (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) )
    #define EMITE_RXDATA_REG             (* (reg8 *) EMITE_BUART_sRX_RxShifter_u0__F0_REG )
    #define EMITE_RXDATA_PTR             (  (reg8 *) EMITE_BUART_sRX_RxShifter_u0__F0_REG )
    #define EMITE_RXADDRESS1_REG         (* (reg8 *) EMITE_BUART_sRX_RxShifter_u0__D0_REG )
    #define EMITE_RXADDRESS1_PTR         (  (reg8 *) EMITE_BUART_sRX_RxShifter_u0__D0_REG )
    #define EMITE_RXADDRESS2_REG         (* (reg8 *) EMITE_BUART_sRX_RxShifter_u0__D1_REG )
    #define EMITE_RXADDRESS2_PTR         (  (reg8 *) EMITE_BUART_sRX_RxShifter_u0__D1_REG )
    #define EMITE_RXDATA_AUX_CTL_REG     (* (reg8 *) EMITE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define EMITE_RXBITCTR_PERIOD_REG    (* (reg8 *) EMITE_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define EMITE_RXBITCTR_PERIOD_PTR    (  (reg8 *) EMITE_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define EMITE_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) EMITE_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define EMITE_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) EMITE_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define EMITE_RXBITCTR_COUNTER_REG   (* (reg8 *) EMITE_BUART_sRX_RxBitCounter__COUNT_REG )
    #define EMITE_RXBITCTR_COUNTER_PTR   (  (reg8 *) EMITE_BUART_sRX_RxBitCounter__COUNT_REG )

    #define EMITE_RXSTATUS_REG           (* (reg8 *) EMITE_BUART_sRX_RxSts__STATUS_REG )
    #define EMITE_RXSTATUS_PTR           (  (reg8 *) EMITE_BUART_sRX_RxSts__STATUS_REG )
    #define EMITE_RXSTATUS_MASK_REG      (* (reg8 *) EMITE_BUART_sRX_RxSts__MASK_REG )
    #define EMITE_RXSTATUS_MASK_PTR      (  (reg8 *) EMITE_BUART_sRX_RxSts__MASK_REG )
    #define EMITE_RXSTATUS_ACTL_REG      (* (reg8 *) EMITE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define EMITE_RXSTATUS_ACTL_PTR      (  (reg8 *) EMITE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) */

#if(EMITE_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define EMITE_INTCLOCK_CLKEN_REG     (* (reg8 *) EMITE_IntClock__PM_ACT_CFG)
    #define EMITE_INTCLOCK_CLKEN_PTR     (  (reg8 *) EMITE_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define EMITE_INTCLOCK_CLKEN_MASK    EMITE_IntClock__PM_ACT_MSK
#endif /* End EMITE_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(EMITE_TX_ENABLED)
    #define EMITE_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End EMITE_TX_ENABLED */

#if(EMITE_HD_ENABLED)
    #define EMITE_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End EMITE_HD_ENABLED */

#if( (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) )
    #define EMITE_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define EMITE_WAIT_1_MS      EMITE_BL_CHK_DELAY_MS   

#define EMITE_TXBUFFERSIZE   EMITE_TX_BUFFER_SIZE
#define EMITE_RXBUFFERSIZE   EMITE_RX_BUFFER_SIZE

#if (EMITE_RXHW_ADDRESS_ENABLED)
    #define EMITE_RXADDRESSMODE  EMITE_RX_ADDRESS_MODE
    #define EMITE_RXHWADDRESS1   EMITE_RX_HW_ADDRESS1
    #define EMITE_RXHWADDRESS2   EMITE_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define EMITE_RXAddressMode  EMITE_RXADDRESSMODE
#endif /* (EMITE_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define EMITE_initvar                    EMITE_initVar

#define EMITE_RX_Enabled                 EMITE_RX_ENABLED
#define EMITE_TX_Enabled                 EMITE_TX_ENABLED
#define EMITE_HD_Enabled                 EMITE_HD_ENABLED
#define EMITE_RX_IntInterruptEnabled     EMITE_RX_INTERRUPT_ENABLED
#define EMITE_TX_IntInterruptEnabled     EMITE_TX_INTERRUPT_ENABLED
#define EMITE_InternalClockUsed          EMITE_INTERNAL_CLOCK_USED
#define EMITE_RXHW_Address_Enabled       EMITE_RXHW_ADDRESS_ENABLED
#define EMITE_OverSampleCount            EMITE_OVER_SAMPLE_COUNT
#define EMITE_ParityType                 EMITE_PARITY_TYPE

#if( EMITE_TX_ENABLED && (EMITE_TXBUFFERSIZE > EMITE_FIFO_LENGTH))
    #define EMITE_TXBUFFER               EMITE_txBuffer
    #define EMITE_TXBUFFERREAD           EMITE_txBufferRead
    #define EMITE_TXBUFFERWRITE          EMITE_txBufferWrite
#endif /* End EMITE_TX_ENABLED */
#if( ( EMITE_RX_ENABLED || EMITE_HD_ENABLED ) && \
     (EMITE_RXBUFFERSIZE > EMITE_FIFO_LENGTH) )
    #define EMITE_RXBUFFER               EMITE_rxBuffer
    #define EMITE_RXBUFFERREAD           EMITE_rxBufferRead
    #define EMITE_RXBUFFERWRITE          EMITE_rxBufferWrite
    #define EMITE_RXBUFFERLOOPDETECT     EMITE_rxBufferLoopDetect
    #define EMITE_RXBUFFER_OVERFLOW      EMITE_rxBufferOverflow
#endif /* End EMITE_RX_ENABLED */

#ifdef EMITE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define EMITE_CONTROL                EMITE_CONTROL_REG
#endif /* End EMITE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(EMITE_TX_ENABLED)
    #define EMITE_TXDATA                 EMITE_TXDATA_REG
    #define EMITE_TXSTATUS               EMITE_TXSTATUS_REG
    #define EMITE_TXSTATUS_MASK          EMITE_TXSTATUS_MASK_REG
    #define EMITE_TXSTATUS_ACTL          EMITE_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(EMITE_TXCLKGEN_DP)
        #define EMITE_TXBITCLKGEN_CTR        EMITE_TXBITCLKGEN_CTR_REG
        #define EMITE_TXBITCLKTX_COMPLETE    EMITE_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define EMITE_TXBITCTR_PERIOD        EMITE_TXBITCTR_PERIOD_REG
        #define EMITE_TXBITCTR_CONTROL       EMITE_TXBITCTR_CONTROL_REG
        #define EMITE_TXBITCTR_COUNTER       EMITE_TXBITCTR_COUNTER_REG
    #endif /* EMITE_TXCLKGEN_DP */
#endif /* End EMITE_TX_ENABLED */

#if(EMITE_HD_ENABLED)
    #define EMITE_TXDATA                 EMITE_TXDATA_REG
    #define EMITE_TXSTATUS               EMITE_TXSTATUS_REG
    #define EMITE_TXSTATUS_MASK          EMITE_TXSTATUS_MASK_REG
    #define EMITE_TXSTATUS_ACTL          EMITE_TXSTATUS_ACTL_REG
#endif /* End EMITE_HD_ENABLED */

#if( (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) )
    #define EMITE_RXDATA                 EMITE_RXDATA_REG
    #define EMITE_RXADDRESS1             EMITE_RXADDRESS1_REG
    #define EMITE_RXADDRESS2             EMITE_RXADDRESS2_REG
    #define EMITE_RXBITCTR_PERIOD        EMITE_RXBITCTR_PERIOD_REG
    #define EMITE_RXBITCTR_CONTROL       EMITE_RXBITCTR_CONTROL_REG
    #define EMITE_RXBITCTR_COUNTER       EMITE_RXBITCTR_COUNTER_REG
    #define EMITE_RXSTATUS               EMITE_RXSTATUS_REG
    #define EMITE_RXSTATUS_MASK          EMITE_RXSTATUS_MASK_REG
    #define EMITE_RXSTATUS_ACTL          EMITE_RXSTATUS_ACTL_REG
#endif /* End  (EMITE_RX_ENABLED) || (EMITE_HD_ENABLED) */

#if(EMITE_INTERNAL_CLOCK_USED)
    #define EMITE_INTCLOCK_CLKEN         EMITE_INTCLOCK_CLKEN_REG
#endif /* End EMITE_INTERNAL_CLOCK_USED */

#define EMITE_WAIT_FOR_COMLETE_REINIT    EMITE_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_EMITE_H */


/* [] END OF FILE */
