/*******************************************************************************
* File Name: XBEE.h
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


#if !defined(CY_UART_XBEE_H)
#define CY_UART_XBEE_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define XBEE_RX_ENABLED                     (1u)
#define XBEE_TX_ENABLED                     (1u)
#define XBEE_HD_ENABLED                     (0u)
#define XBEE_RX_INTERRUPT_ENABLED           (0u)
#define XBEE_TX_INTERRUPT_ENABLED           (0u)
#define XBEE_INTERNAL_CLOCK_USED            (1u)
#define XBEE_RXHW_ADDRESS_ENABLED           (0u)
#define XBEE_OVER_SAMPLE_COUNT              (8u)
#define XBEE_PARITY_TYPE                    (0u)
#define XBEE_PARITY_TYPE_SW                 (0u)
#define XBEE_BREAK_DETECT                   (0u)
#define XBEE_BREAK_BITS_TX                  (13u)
#define XBEE_BREAK_BITS_RX                  (13u)
#define XBEE_TXCLKGEN_DP                    (1u)
#define XBEE_USE23POLLING                   (1u)
#define XBEE_FLOW_CONTROL                   (0u)
#define XBEE_CLK_FREQ                       (0u)
#define XBEE_TX_BUFFER_SIZE                 (4u)
#define XBEE_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(XBEE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define XBEE_CONTROL_REG_REMOVED            (0u)
#else
    #define XBEE_CONTROL_REG_REMOVED            (1u)
#endif /* End XBEE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct XBEE_backupStruct_
{
    uint8 enableState;

    #if(XBEE_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End XBEE_CONTROL_REG_REMOVED */

} XBEE_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void XBEE_Start(void) ;
void XBEE_Stop(void) ;
uint8 XBEE_ReadControlRegister(void) ;
void XBEE_WriteControlRegister(uint8 control) ;

void XBEE_Init(void) ;
void XBEE_Enable(void) ;
void XBEE_SaveConfig(void) ;
void XBEE_RestoreConfig(void) ;
void XBEE_Sleep(void) ;
void XBEE_Wakeup(void) ;

/* Only if RX is enabled */
#if( (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) )

    #if (XBEE_RX_INTERRUPT_ENABLED)
        #define XBEE_EnableRxInt()  CyIntEnable (XBEE_RX_VECT_NUM)
        #define XBEE_DisableRxInt() CyIntDisable(XBEE_RX_VECT_NUM)
        CY_ISR_PROTO(XBEE_RXISR);
    #endif /* XBEE_RX_INTERRUPT_ENABLED */

    void XBEE_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void XBEE_SetRxAddress1(uint8 address) ;
    void XBEE_SetRxAddress2(uint8 address) ;

    void  XBEE_SetRxInterruptMode(uint8 intSrc) ;
    uint8 XBEE_ReadRxData(void) ;
    uint8 XBEE_ReadRxStatus(void) ;
    uint8 XBEE_GetChar(void) ;
    uint16 XBEE_GetByte(void) ;
    uint8 XBEE_GetRxBufferSize(void)
                                                            ;
    void XBEE_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define XBEE_GetRxInterruptSource   XBEE_ReadRxStatus

#endif /* End (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) */

/* Only if TX is enabled */
#if(XBEE_TX_ENABLED || XBEE_HD_ENABLED)

    #if(XBEE_TX_INTERRUPT_ENABLED)
        #define XBEE_EnableTxInt()  CyIntEnable (XBEE_TX_VECT_NUM)
        #define XBEE_DisableTxInt() CyIntDisable(XBEE_TX_VECT_NUM)
        #define XBEE_SetPendingTxInt() CyIntSetPending(XBEE_TX_VECT_NUM)
        #define XBEE_ClearPendingTxInt() CyIntClearPending(XBEE_TX_VECT_NUM)
        CY_ISR_PROTO(XBEE_TXISR);
    #endif /* XBEE_TX_INTERRUPT_ENABLED */

    void XBEE_SetTxInterruptMode(uint8 intSrc) ;
    void XBEE_WriteTxData(uint8 txDataByte) ;
    uint8 XBEE_ReadTxStatus(void) ;
    void XBEE_PutChar(uint8 txDataByte) ;
    void XBEE_PutString(const char8 string[]) ;
    void XBEE_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void XBEE_PutCRLF(uint8 txDataByte) ;
    void XBEE_ClearTxBuffer(void) ;
    void XBEE_SetTxAddressMode(uint8 addressMode) ;
    void XBEE_SendBreak(uint8 retMode) ;
    uint8 XBEE_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define XBEE_PutStringConst         XBEE_PutString
    #define XBEE_PutArrayConst          XBEE_PutArray
    #define XBEE_GetTxInterruptSource   XBEE_ReadTxStatus

#endif /* End XBEE_TX_ENABLED || XBEE_HD_ENABLED */

#if(XBEE_HD_ENABLED)
    void XBEE_LoadRxConfig(void) ;
    void XBEE_LoadTxConfig(void) ;
#endif /* End XBEE_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_XBEE) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    XBEE_CyBtldrCommStart(void) CYSMALL ;
    void    XBEE_CyBtldrCommStop(void) CYSMALL ;
    void    XBEE_CyBtldrCommReset(void) CYSMALL ;
    cystatus XBEE_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus XBEE_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_XBEE)
        #define CyBtldrCommStart    XBEE_CyBtldrCommStart
        #define CyBtldrCommStop     XBEE_CyBtldrCommStop
        #define CyBtldrCommReset    XBEE_CyBtldrCommReset
        #define CyBtldrCommWrite    XBEE_CyBtldrCommWrite
        #define CyBtldrCommRead     XBEE_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_XBEE) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define XBEE_BYTE2BYTE_TIME_OUT (25u)
    #define XBEE_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define XBEE_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define XBEE_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define XBEE_SET_SPACE      (0x00u)
#define XBEE_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (XBEE_TX_ENABLED) || (XBEE_HD_ENABLED) )
    #if(XBEE_TX_INTERRUPT_ENABLED)
        #define XBEE_TX_VECT_NUM            (uint8)XBEE_TXInternalInterrupt__INTC_NUMBER
        #define XBEE_TX_PRIOR_NUM           (uint8)XBEE_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* XBEE_TX_INTERRUPT_ENABLED */

    #define XBEE_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define XBEE_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define XBEE_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(XBEE_TX_ENABLED)
        #define XBEE_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (XBEE_HD_ENABLED) */
        #define XBEE_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (XBEE_TX_ENABLED) */

    #define XBEE_TX_STS_COMPLETE            (uint8)(0x01u << XBEE_TX_STS_COMPLETE_SHIFT)
    #define XBEE_TX_STS_FIFO_EMPTY          (uint8)(0x01u << XBEE_TX_STS_FIFO_EMPTY_SHIFT)
    #define XBEE_TX_STS_FIFO_FULL           (uint8)(0x01u << XBEE_TX_STS_FIFO_FULL_SHIFT)
    #define XBEE_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << XBEE_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (XBEE_TX_ENABLED) || (XBEE_HD_ENABLED)*/

#if( (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) )
    #if(XBEE_RX_INTERRUPT_ENABLED)
        #define XBEE_RX_VECT_NUM            (uint8)XBEE_RXInternalInterrupt__INTC_NUMBER
        #define XBEE_RX_PRIOR_NUM           (uint8)XBEE_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* XBEE_RX_INTERRUPT_ENABLED */
    #define XBEE_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define XBEE_RX_STS_BREAK_SHIFT             (0x01u)
    #define XBEE_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define XBEE_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define XBEE_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define XBEE_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define XBEE_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define XBEE_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define XBEE_RX_STS_MRKSPC           (uint8)(0x01u << XBEE_RX_STS_MRKSPC_SHIFT)
    #define XBEE_RX_STS_BREAK            (uint8)(0x01u << XBEE_RX_STS_BREAK_SHIFT)
    #define XBEE_RX_STS_PAR_ERROR        (uint8)(0x01u << XBEE_RX_STS_PAR_ERROR_SHIFT)
    #define XBEE_RX_STS_STOP_ERROR       (uint8)(0x01u << XBEE_RX_STS_STOP_ERROR_SHIFT)
    #define XBEE_RX_STS_OVERRUN          (uint8)(0x01u << XBEE_RX_STS_OVERRUN_SHIFT)
    #define XBEE_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << XBEE_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define XBEE_RX_STS_ADDR_MATCH       (uint8)(0x01u << XBEE_RX_STS_ADDR_MATCH_SHIFT)
    #define XBEE_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << XBEE_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define XBEE_RX_HW_MASK                     (0x7Fu)
#endif /* End (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) */

/* Control Register definitions */
#define XBEE_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define XBEE_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define XBEE_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define XBEE_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define XBEE_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define XBEE_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define XBEE_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define XBEE_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define XBEE_CTRL_HD_SEND               (uint8)(0x01u << XBEE_CTRL_HD_SEND_SHIFT)
#define XBEE_CTRL_HD_SEND_BREAK         (uint8)(0x01u << XBEE_CTRL_HD_SEND_BREAK_SHIFT)
#define XBEE_CTRL_MARK                  (uint8)(0x01u << XBEE_CTRL_MARK_SHIFT)
#define XBEE_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << XBEE_CTRL_PARITY_TYPE0_SHIFT)
#define XBEE_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << XBEE_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define XBEE_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define XBEE_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define XBEE_SEND_BREAK                         (0x00u)
#define XBEE_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define XBEE_REINIT                             (0x02u)
#define XBEE_SEND_WAIT_REINIT                   (0x03u)

#define XBEE_OVER_SAMPLE_8                      (8u)
#define XBEE_OVER_SAMPLE_16                     (16u)

#define XBEE_BIT_CENTER                         (XBEE_OVER_SAMPLE_COUNT - 2u)

#define XBEE_FIFO_LENGTH                        (4u)
#define XBEE_NUMBER_OF_START_BIT                (1u)
#define XBEE_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define XBEE_TXBITCTR_BREAKBITS8X   ((XBEE_BREAK_BITS_TX * XBEE_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define XBEE_TXBITCTR_BREAKBITS ((XBEE_BREAK_BITS_TX * XBEE_OVER_SAMPLE_COUNT) - 1u)

#define XBEE_HALF_BIT_COUNT   \
                            (((XBEE_OVER_SAMPLE_COUNT / 2u) + (XBEE_USE23POLLING * 1u)) - 2u)
#if (XBEE_OVER_SAMPLE_COUNT == XBEE_OVER_SAMPLE_8)
    #define XBEE_HD_TXBITCTR_INIT   (((XBEE_BREAK_BITS_TX + \
                            XBEE_NUMBER_OF_START_BIT) * XBEE_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define XBEE_RXBITCTR_INIT  ((((XBEE_BREAK_BITS_RX + XBEE_NUMBER_OF_START_BIT) \
                            * XBEE_OVER_SAMPLE_COUNT) + XBEE_HALF_BIT_COUNT) - 1u)

#else /* XBEE_OVER_SAMPLE_COUNT == XBEE_OVER_SAMPLE_16 */
    #define XBEE_HD_TXBITCTR_INIT   ((8u * XBEE_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define XBEE_RXBITCTR_INIT      (((7u * XBEE_OVER_SAMPLE_COUNT) - 1u) + \
                                                      XBEE_HALF_BIT_COUNT)
#endif /* End XBEE_OVER_SAMPLE_COUNT */

#define XBEE_HD_RXBITCTR_INIT                   XBEE_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 XBEE_initVar;
#if (XBEE_TX_INTERRUPT_ENABLED && XBEE_TX_ENABLED)
    extern volatile uint8 XBEE_txBuffer[XBEE_TX_BUFFER_SIZE];
    extern volatile uint8 XBEE_txBufferRead;
    extern uint8 XBEE_txBufferWrite;
#endif /* (XBEE_TX_INTERRUPT_ENABLED && XBEE_TX_ENABLED) */
#if (XBEE_RX_INTERRUPT_ENABLED && (XBEE_RX_ENABLED || XBEE_HD_ENABLED))
    extern uint8 XBEE_errorStatus;
    extern volatile uint8 XBEE_rxBuffer[XBEE_RX_BUFFER_SIZE];
    extern volatile uint8 XBEE_rxBufferRead;
    extern volatile uint8 XBEE_rxBufferWrite;
    extern volatile uint8 XBEE_rxBufferLoopDetect;
    extern volatile uint8 XBEE_rxBufferOverflow;
    #if (XBEE_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 XBEE_rxAddressMode;
        extern volatile uint8 XBEE_rxAddressDetected;
    #endif /* (XBEE_RXHW_ADDRESS_ENABLED) */
#endif /* (XBEE_RX_INTERRUPT_ENABLED && (XBEE_RX_ENABLED || XBEE_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define XBEE__B_UART__AM_SW_BYTE_BYTE 1
#define XBEE__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define XBEE__B_UART__AM_HW_BYTE_BY_BYTE 3
#define XBEE__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define XBEE__B_UART__AM_NONE 0

#define XBEE__B_UART__NONE_REVB 0
#define XBEE__B_UART__EVEN_REVB 1
#define XBEE__B_UART__ODD_REVB 2
#define XBEE__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define XBEE_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define XBEE_NUMBER_OF_STOP_BITS    (1u)

#if (XBEE_RXHW_ADDRESS_ENABLED)
    #define XBEE_RX_ADDRESS_MODE    (0u)
    #define XBEE_RX_HW_ADDRESS1     (0u)
    #define XBEE_RX_HW_ADDRESS2     (0u)
#endif /* (XBEE_RXHW_ADDRESS_ENABLED) */

#define XBEE_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << XBEE_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << XBEE_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << XBEE_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << XBEE_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << XBEE_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << XBEE_RX_STS_BREAK_SHIFT) \
                                        | (0 << XBEE_RX_STS_OVERRUN_SHIFT))

#define XBEE_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << XBEE_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << XBEE_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << XBEE_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << XBEE_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef XBEE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define XBEE_CONTROL_REG \
                            (* (reg8 *) XBEE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define XBEE_CONTROL_PTR \
                            (  (reg8 *) XBEE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End XBEE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(XBEE_TX_ENABLED)
    #define XBEE_TXDATA_REG          (* (reg8 *) XBEE_BUART_sTX_TxShifter_u0__F0_REG)
    #define XBEE_TXDATA_PTR          (  (reg8 *) XBEE_BUART_sTX_TxShifter_u0__F0_REG)
    #define XBEE_TXDATA_AUX_CTL_REG  (* (reg8 *) XBEE_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define XBEE_TXDATA_AUX_CTL_PTR  (  (reg8 *) XBEE_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define XBEE_TXSTATUS_REG        (* (reg8 *) XBEE_BUART_sTX_TxSts__STATUS_REG)
    #define XBEE_TXSTATUS_PTR        (  (reg8 *) XBEE_BUART_sTX_TxSts__STATUS_REG)
    #define XBEE_TXSTATUS_MASK_REG   (* (reg8 *) XBEE_BUART_sTX_TxSts__MASK_REG)
    #define XBEE_TXSTATUS_MASK_PTR   (  (reg8 *) XBEE_BUART_sTX_TxSts__MASK_REG)
    #define XBEE_TXSTATUS_ACTL_REG   (* (reg8 *) XBEE_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define XBEE_TXSTATUS_ACTL_PTR   (  (reg8 *) XBEE_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(XBEE_TXCLKGEN_DP)
        #define XBEE_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define XBEE_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define XBEE_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define XBEE_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define XBEE_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define XBEE_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define XBEE_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define XBEE_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define XBEE_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define XBEE_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) XBEE_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* XBEE_TXCLKGEN_DP */

#endif /* End XBEE_TX_ENABLED */

#if(XBEE_HD_ENABLED)

    #define XBEE_TXDATA_REG             (* (reg8 *) XBEE_BUART_sRX_RxShifter_u0__F1_REG )
    #define XBEE_TXDATA_PTR             (  (reg8 *) XBEE_BUART_sRX_RxShifter_u0__F1_REG )
    #define XBEE_TXDATA_AUX_CTL_REG     (* (reg8 *) XBEE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define XBEE_TXDATA_AUX_CTL_PTR     (  (reg8 *) XBEE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define XBEE_TXSTATUS_REG           (* (reg8 *) XBEE_BUART_sRX_RxSts__STATUS_REG )
    #define XBEE_TXSTATUS_PTR           (  (reg8 *) XBEE_BUART_sRX_RxSts__STATUS_REG )
    #define XBEE_TXSTATUS_MASK_REG      (* (reg8 *) XBEE_BUART_sRX_RxSts__MASK_REG )
    #define XBEE_TXSTATUS_MASK_PTR      (  (reg8 *) XBEE_BUART_sRX_RxSts__MASK_REG )
    #define XBEE_TXSTATUS_ACTL_REG      (* (reg8 *) XBEE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define XBEE_TXSTATUS_ACTL_PTR      (  (reg8 *) XBEE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End XBEE_HD_ENABLED */

#if( (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) )
    #define XBEE_RXDATA_REG             (* (reg8 *) XBEE_BUART_sRX_RxShifter_u0__F0_REG )
    #define XBEE_RXDATA_PTR             (  (reg8 *) XBEE_BUART_sRX_RxShifter_u0__F0_REG )
    #define XBEE_RXADDRESS1_REG         (* (reg8 *) XBEE_BUART_sRX_RxShifter_u0__D0_REG )
    #define XBEE_RXADDRESS1_PTR         (  (reg8 *) XBEE_BUART_sRX_RxShifter_u0__D0_REG )
    #define XBEE_RXADDRESS2_REG         (* (reg8 *) XBEE_BUART_sRX_RxShifter_u0__D1_REG )
    #define XBEE_RXADDRESS2_PTR         (  (reg8 *) XBEE_BUART_sRX_RxShifter_u0__D1_REG )
    #define XBEE_RXDATA_AUX_CTL_REG     (* (reg8 *) XBEE_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define XBEE_RXBITCTR_PERIOD_REG    (* (reg8 *) XBEE_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define XBEE_RXBITCTR_PERIOD_PTR    (  (reg8 *) XBEE_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define XBEE_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) XBEE_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define XBEE_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) XBEE_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define XBEE_RXBITCTR_COUNTER_REG   (* (reg8 *) XBEE_BUART_sRX_RxBitCounter__COUNT_REG )
    #define XBEE_RXBITCTR_COUNTER_PTR   (  (reg8 *) XBEE_BUART_sRX_RxBitCounter__COUNT_REG )

    #define XBEE_RXSTATUS_REG           (* (reg8 *) XBEE_BUART_sRX_RxSts__STATUS_REG )
    #define XBEE_RXSTATUS_PTR           (  (reg8 *) XBEE_BUART_sRX_RxSts__STATUS_REG )
    #define XBEE_RXSTATUS_MASK_REG      (* (reg8 *) XBEE_BUART_sRX_RxSts__MASK_REG )
    #define XBEE_RXSTATUS_MASK_PTR      (  (reg8 *) XBEE_BUART_sRX_RxSts__MASK_REG )
    #define XBEE_RXSTATUS_ACTL_REG      (* (reg8 *) XBEE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define XBEE_RXSTATUS_ACTL_PTR      (  (reg8 *) XBEE_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) */

#if(XBEE_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define XBEE_INTCLOCK_CLKEN_REG     (* (reg8 *) XBEE_IntClock__PM_ACT_CFG)
    #define XBEE_INTCLOCK_CLKEN_PTR     (  (reg8 *) XBEE_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define XBEE_INTCLOCK_CLKEN_MASK    XBEE_IntClock__PM_ACT_MSK
#endif /* End XBEE_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(XBEE_TX_ENABLED)
    #define XBEE_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End XBEE_TX_ENABLED */

#if(XBEE_HD_ENABLED)
    #define XBEE_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End XBEE_HD_ENABLED */

#if( (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) )
    #define XBEE_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define XBEE_WAIT_1_MS      XBEE_BL_CHK_DELAY_MS   

#define XBEE_TXBUFFERSIZE   XBEE_TX_BUFFER_SIZE
#define XBEE_RXBUFFERSIZE   XBEE_RX_BUFFER_SIZE

#if (XBEE_RXHW_ADDRESS_ENABLED)
    #define XBEE_RXADDRESSMODE  XBEE_RX_ADDRESS_MODE
    #define XBEE_RXHWADDRESS1   XBEE_RX_HW_ADDRESS1
    #define XBEE_RXHWADDRESS2   XBEE_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define XBEE_RXAddressMode  XBEE_RXADDRESSMODE
#endif /* (XBEE_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define XBEE_initvar                    XBEE_initVar

#define XBEE_RX_Enabled                 XBEE_RX_ENABLED
#define XBEE_TX_Enabled                 XBEE_TX_ENABLED
#define XBEE_HD_Enabled                 XBEE_HD_ENABLED
#define XBEE_RX_IntInterruptEnabled     XBEE_RX_INTERRUPT_ENABLED
#define XBEE_TX_IntInterruptEnabled     XBEE_TX_INTERRUPT_ENABLED
#define XBEE_InternalClockUsed          XBEE_INTERNAL_CLOCK_USED
#define XBEE_RXHW_Address_Enabled       XBEE_RXHW_ADDRESS_ENABLED
#define XBEE_OverSampleCount            XBEE_OVER_SAMPLE_COUNT
#define XBEE_ParityType                 XBEE_PARITY_TYPE

#if( XBEE_TX_ENABLED && (XBEE_TXBUFFERSIZE > XBEE_FIFO_LENGTH))
    #define XBEE_TXBUFFER               XBEE_txBuffer
    #define XBEE_TXBUFFERREAD           XBEE_txBufferRead
    #define XBEE_TXBUFFERWRITE          XBEE_txBufferWrite
#endif /* End XBEE_TX_ENABLED */
#if( ( XBEE_RX_ENABLED || XBEE_HD_ENABLED ) && \
     (XBEE_RXBUFFERSIZE > XBEE_FIFO_LENGTH) )
    #define XBEE_RXBUFFER               XBEE_rxBuffer
    #define XBEE_RXBUFFERREAD           XBEE_rxBufferRead
    #define XBEE_RXBUFFERWRITE          XBEE_rxBufferWrite
    #define XBEE_RXBUFFERLOOPDETECT     XBEE_rxBufferLoopDetect
    #define XBEE_RXBUFFER_OVERFLOW      XBEE_rxBufferOverflow
#endif /* End XBEE_RX_ENABLED */

#ifdef XBEE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define XBEE_CONTROL                XBEE_CONTROL_REG
#endif /* End XBEE_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(XBEE_TX_ENABLED)
    #define XBEE_TXDATA                 XBEE_TXDATA_REG
    #define XBEE_TXSTATUS               XBEE_TXSTATUS_REG
    #define XBEE_TXSTATUS_MASK          XBEE_TXSTATUS_MASK_REG
    #define XBEE_TXSTATUS_ACTL          XBEE_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(XBEE_TXCLKGEN_DP)
        #define XBEE_TXBITCLKGEN_CTR        XBEE_TXBITCLKGEN_CTR_REG
        #define XBEE_TXBITCLKTX_COMPLETE    XBEE_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define XBEE_TXBITCTR_PERIOD        XBEE_TXBITCTR_PERIOD_REG
        #define XBEE_TXBITCTR_CONTROL       XBEE_TXBITCTR_CONTROL_REG
        #define XBEE_TXBITCTR_COUNTER       XBEE_TXBITCTR_COUNTER_REG
    #endif /* XBEE_TXCLKGEN_DP */
#endif /* End XBEE_TX_ENABLED */

#if(XBEE_HD_ENABLED)
    #define XBEE_TXDATA                 XBEE_TXDATA_REG
    #define XBEE_TXSTATUS               XBEE_TXSTATUS_REG
    #define XBEE_TXSTATUS_MASK          XBEE_TXSTATUS_MASK_REG
    #define XBEE_TXSTATUS_ACTL          XBEE_TXSTATUS_ACTL_REG
#endif /* End XBEE_HD_ENABLED */

#if( (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) )
    #define XBEE_RXDATA                 XBEE_RXDATA_REG
    #define XBEE_RXADDRESS1             XBEE_RXADDRESS1_REG
    #define XBEE_RXADDRESS2             XBEE_RXADDRESS2_REG
    #define XBEE_RXBITCTR_PERIOD        XBEE_RXBITCTR_PERIOD_REG
    #define XBEE_RXBITCTR_CONTROL       XBEE_RXBITCTR_CONTROL_REG
    #define XBEE_RXBITCTR_COUNTER       XBEE_RXBITCTR_COUNTER_REG
    #define XBEE_RXSTATUS               XBEE_RXSTATUS_REG
    #define XBEE_RXSTATUS_MASK          XBEE_RXSTATUS_MASK_REG
    #define XBEE_RXSTATUS_ACTL          XBEE_RXSTATUS_ACTL_REG
#endif /* End  (XBEE_RX_ENABLED) || (XBEE_HD_ENABLED) */

#if(XBEE_INTERNAL_CLOCK_USED)
    #define XBEE_INTCLOCK_CLKEN         XBEE_INTCLOCK_CLKEN_REG
#endif /* End XBEE_INTERNAL_CLOCK_USED */

#define XBEE_WAIT_FOR_COMLETE_REINIT    XBEE_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_XBEE_H */


/* [] END OF FILE */
