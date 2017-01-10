/*******************************************************************************
* File Name: SENSORES.h
* Version 2.40
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Sequencing Successive Approximation ADC Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ADC_SAR_SEQ_SENSORES_H)
#define CY_ADC_SAR_SEQ_SENSORES_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*      Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} SENSORES_BACKUP_STRUCT;


/**************************************
*    Enumerated Types and Parameters
**************************************/

/*  Clock Source setting constants */
#define SENSORES__EXTERNAL 0
#define SENSORES__INTERNAL 1

/*  Sample Mode setting constants */
#define SENSORES__FREERUNNING 0
#define SENSORES__HARDWARESOC 1

/*  Reference type setting constants */
#define SENSORES__VDDA_2 0
#define SENSORES__VDDA 1
#define SENSORES__INTERNAL1024 2
#define SENSORES__INTERNAL1024BYPASSED 3
#define SENSORES__INTERNALVREF 4
#define SENSORES__INTERNALVREFBYPASSED 5
#define SENSORES__VDDA_2BYPASSED 6
#define SENSORES__EXTERNALVREF 7

/* Input buffer gain setting constants */
#define SENSORES__DISABLED 0
#define SENSORES__ONE 1
#define SENSORES__TWO 2
#define SENSORES__FOUR 3
#define SENSORES__EIGHT 4
#define SENSORES__SIXTEEN 5

/* Negative input setting sonstants in single ended mode */
#define SENSORES__VSS 0
#define SENSORES__VREF 1
#define SENSORES__OTHER 2

/* Compare mode setting constants:
*    Mode0 - Disable
*    Mode1 - Result < Low_Limit
*    Mode2 - Low_Limit <= Result < High_Limit
*    Mode3 - High_Limit <= Result
*    Mode4 - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define SENSORES__MODE0 0
#define SENSORES__MODE1 1
#define SENSORES__MODE2 2
#define SENSORES__MODE3 3

#define SENSORES__RES8 0
#define SENSORES__RES10 1

#define SENSORES__RIGHT 0
#define SENSORES__LEFT 1

#define SENSORES__FSIGNED 1
#define SENSORES__FUNSIGNED 0

#define SENSORES__ACCUMULATE 0
#define SENSORES__FIXEDRESOLUTION 1



/***************************************
*   Conditional Compilation Parameters
****************************************/ 

#define SENSORES_CY_SAR_IP_VER0               	(0u)
#define SENSORES_CY_SAR_IP_VER1               	(1u)

#if (CY_PSOC4_4100 || CY_PSOC4_4200)
    #define SENSORES_CY_SAR_IP_VER              (SENSORES_CY_SAR_IP_VER0)
#else /* Other devices */
    #define SENSORES_CY_SAR_IP_VER              (SENSORES_CY_SAR_IP_VER1)
#endif  /* (CY_PSOC4_4100 || CY_PSOC4_4200) */


/***************************************
*    Initial Parameter Constants
***************************************/
#define SENSORES_DEFAULT_SAMPLE_MODE_SEL        (0u)
#define SENSORES_DEFAULT_VREF_SEL               (1u)
#define SENSORES_DEFAULT_NEG_INPUT_SEL          (0u)
#define SENSORES_DEFAULT_ALT_RESOLUTION_SEL     (0u)
#define SENSORES_DEFAULT_JUSTIFICATION_SEL      (0u)
#define SENSORES_DEFAULT_DIFF_RESULT_FORMAT_SEL (1u)
#define SENSORES_DEFAULT_SE_RESULT_FORMAT_SEL   (1u)
#define SENSORES_DEFAULT_CLOCK_SOURCE           (1u)
#define SENSORES_DEFAULT_VREF_MV_VALUE          (5000)
#define SENSORES_DEFAULT_BUFFER_GAIN            (0u)
#define SENSORES_DEFAULT_AVG_SAMPLES_NUM        (0u)
#define SENSORES_DEFAULT_AVG_SAMPLES_DIV        (int16)(0x100u >> (7u - 0u))
#define SENSORES_DEFAULT_AVG_MODE               (1u)
#define SENSORES_MAX_RESOLUTION                 (12u)
#define SENSORES_DEFAULT_LOW_LIMIT              (0u)
#define SENSORES_DEFAULT_HIGH_LIMIT             (2047u)
#define SENSORES_DEFAULT_COMPARE_MODE           (0u)
#define SENSORES_DEFAULT_ACLKS_NUM              (2u)
#define SENSORES_DEFAULT_BCLKS_NUM              (2u)
#define SENSORES_DEFAULT_CCLKS_NUM              (2u)
#define SENSORES_DEFAULT_DCLKS_NUM              (2u)
#define SENSORES_TOTAL_CHANNELS_NUM             (1u)
#define SENSORES_SEQUENCED_CHANNELS_NUM         (1u)
#define SENSORES_DEFAULT_EN_CHANNELS            (1u)
#define SENSORES_NOMINAL_CLOCK_FREQ             (1000000)
#define SENSORES_INJ_CHANNEL_ENABLED            (0u)
#define SENSORES_IRQ_REMOVE                     (0u)

/* Determines whether the configuration contains external negative input. */
#define SENSORES_SINGLE_PRESENT                 (0u)
#define SENSORES_CHANNELS_MODE                  (0u)
#define SENSORES_MAX_CHANNELS_EN_MASK           (0xffffu >> (16u - SENSORES_SEQUENCED_CHANNELS_NUM))


/***************************************
*        Function Prototypes
***************************************/

void SENSORES_Start(void);
void SENSORES_Stop(void);
void SENSORES_Init(void);
void SENSORES_Enable(void);
void SENSORES_StartConvert(void);
void SENSORES_StopConvert(void);
uint32 SENSORES_IsEndConversion(uint32 retMode);
int16 SENSORES_GetResult16(uint32 chan);
void SENSORES_SetChanMask(uint32 mask);
void SENSORES_SetLowLimit(uint32 lowLimit);
void SENSORES_SetHighLimit(uint32 highLimit);
void SENSORES_SetLimitMask(uint32 mask);
void SENSORES_SetSatMask(uint32 mask);
void SENSORES_SetOffset(uint32 chan, int16 offset);
void SENSORES_SetGain(uint32 chan, int32 adcGain);
#if(SENSORES_INJ_CHANNEL_ENABLED)
    void SENSORES_EnableInjection(void);
#endif /* SENSORES_INJ_CHANNEL_ENABLED */
#if(SENSORES_DEFAULT_JUSTIFICATION_SEL == SENSORES__RIGHT)
    int16 SENSORES_CountsTo_mVolts(uint32 chan, int16 adcCounts);
    int32 SENSORES_CountsTo_uVolts(uint32 chan, int16 adcCounts);
    float32 SENSORES_CountsTo_Volts(uint32 chan, int16 adcCounts);
#endif /* End SENSORES_DEFAULT_JUSTIFICATION_SEL == SENSORES__RIGHT */
void SENSORES_Sleep(void);
void SENSORES_Wakeup(void);
void SENSORES_SaveConfig(void);
void SENSORES_RestoreConfig(void);

CY_ISR_PROTO( SENSORES_ISR );


/**************************************
*           API Constants
**************************************/
/* Constants for Sleep mode states */
#define SENSORES_DISABLED                   (0x00u)
#define SENSORES_ENABLED                    (0x01u)
#define SENSORES_STARTED                    (0x02u)
#define SENSORES_BOOSTPUMP_ENABLED          (0x04u)

/*   Constants for IsEndConversion() "retMode" parameter  */
#define SENSORES_RETURN_STATUS              (0x01u)
#define SENSORES_WAIT_FOR_RESULT            (0x02u)
#define SENSORES_RETURN_STATUS_INJ          (0x04u)
#define SENSORES_WAIT_FOR_RESULT_INJ        (0x08u)

#define SENSORES_MAX_FREQUENCY              (18000000)       /*18Mhz*/

#define SENSORES_RESOLUTION_12              (12u)
#define SENSORES_RESOLUTION_10              (10u)
#define SENSORES_RESOLUTION_8               (8u)

#define SENSORES_10US_DELAY                 (10u)

#define SENSORES_10V_COUNTS                 (10.0F)
#define SENSORES_10MV_COUNTS                (10000)
#define SENSORES_10UV_COUNTS                (10000000L)


/***************************************
* Global variables external identifier
***************************************/

extern uint8 SENSORES_initVar;
extern volatile int16 SENSORES_offset[SENSORES_TOTAL_CHANNELS_NUM];
extern volatile int32 SENSORES_countsPer10Volt[SENSORES_TOTAL_CHANNELS_NUM];


/***************************************
*              Registers
***************************************/

#define SENSORES_SAR_CTRL_REG                (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CTRL )
#define SENSORES_SAR_CTRL_PTR                ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CTRL )

#define SENSORES_SAR_SAMPLE_CTRL_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_SAMPLE_CTRL )
#define SENSORES_SAR_SAMPLE_CTRL_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_SAMPLE_CTRL )

#define SENSORES_SAR_SAMPLE_TIME01_REG       (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_SAMPLE_TIME01 )
#define SENSORES_SAR_SAMPLE_TIME01_PTR       ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_SAMPLE_TIME01 )

#define SENSORES_SAR_SAMPLE_TIME23_REG       (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_SAMPLE_TIME23 )
#define SENSORES_SAR_SAMPLE_TIME23_PTR       ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_SAMPLE_TIME23 )

#define SENSORES_SAR_RANGE_THRES_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_THRES )
#define SENSORES_SAR_RANGE_THRES_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_THRES )

#define SENSORES_SAR_RANGE_COND_REG          (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_COND )
#define SENSORES_SAR_RANGE_COND_PTR          ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_COND )

#define SENSORES_SAR_CHAN_EN_REG             (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_EN )
#define SENSORES_SAR_CHAN_EN_PTR             ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_EN )

#define SENSORES_SAR_START_CTRL_REG          (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_START_CTRL )
#define SENSORES_SAR_START_CTRL_PTR          ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_START_CTRL )

#define SENSORES_SAR_DFT_CTRL_REG            (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_DFT_CTRL )
#define SENSORES_SAR_DFT_CTRL_PTR            ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_DFT_CTRL )

#define SENSORES_SAR_CHAN_CONFIG_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define SENSORES_SAR_CHAN_CONFIG_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define SENSORES_SAR_CHAN_CONFIG_IND         SENSORES_cy_psoc4_sar__SAR_CHAN_CONFIG00

#define SENSORES_SAR_CHAN_WORK_REG           (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_WORK00 )
#define SENSORES_SAR_CHAN_WORK_PTR           ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_WORK00 )

#define SENSORES_SAR_CHAN_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define SENSORES_SAR_CHAN_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define SENSORES_SAR_CHAN_RESULT_IND         SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT00

#define SENSORES_SAR_CHAN0_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define SENSORES_SAR_CHAN0_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT00 )

#define SENSORES_SAR_CHAN1_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT01 )
#define SENSORES_SAR_CHAN1_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT01 )

#define SENSORES_SAR_CHAN2_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT02 )
#define SENSORES_SAR_CHAN2_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT02 )

#define SENSORES_SAR_CHAN3_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT03 )
#define SENSORES_SAR_CHAN3_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT03 )

#define SENSORES_SAR_CHAN4_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT04 )
#define SENSORES_SAR_CHAN4_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT04 )

#define SENSORES_SAR_CHAN5_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT05 )
#define SENSORES_SAR_CHAN5_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT05 )

#define SENSORES_SAR_CHAN6_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT06 )
#define SENSORES_SAR_CHAN6_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT06 )

#define SENSORES_SAR_CHAN7_RESULT_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT07 )
#define SENSORES_SAR_CHAN7_RESULT_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT07 )

#if(SENSORES_CY_SAR_IP_VER != SENSORES_CY_SAR_IP_VER0)
    #define SENSORES_SAR_CHAN8_RESULT_REG     (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT08 )
    #define SENSORES_SAR_CHAN8_RESULT_PTR     ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT08 )

    #define SENSORES_SAR_CHAN9_RESULT_REG     (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT09 )
    #define SENSORES_SAR_CHAN9_RESULT_PTR     ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT09 )

    #define SENSORES_SAR_CHAN10_RESULT_REG    (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT10 )
    #define SENSORES_SAR_CHAN10_RESULT_PTR    ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT10 )

    #define SENSORES_SAR_CHAN11_RESULT_REG    (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT11 )
    #define SENSORES_SAR_CHAN11_RESULT_PTR    ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT11 )

    #define SENSORES_SAR_CHAN12_RESULT_REG    (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT12 )
    #define SENSORES_SAR_CHAN12_RESULT_PTR    ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT12 )

    #define SENSORES_SAR_CHAN13_RESULT_REG    (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT13 )
    #define SENSORES_SAR_CHAN13_RESULT_PTR    ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT13 )

    #define SENSORES_SAR_CHAN14_RESULT_REG    (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT14 )
    #define SENSORES_SAR_CHAN14_RESULT_PTR    ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT14 )

    #define SENSORES_SAR_CHAN15_RESULT_REG    (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT15 )
    #define SENSORES_SAR_CHAN15_RESULT_PTR    ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT15 )
#endif /* (SENSORES_CY_SAR_IP_VER != SENSORES_CY_SAR_IP_VER0) */

#define SENSORES_SAR_CHAN_WORK_VALID_REG     (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_WORK_VALID)
#define SENSORES_SAR_CHAN_WORK_VALID_PTR     ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_WORK_VALID)

#define SENSORES_SAR_CHAN_RESULT_VALID_REG  ( *(reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )
#define SENSORES_SAR_CHAN_RESULT_VALID_PTR  ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )

#define SENSORES_SAR_STATUS_REG              (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_STATUS )
#define SENSORES_SAR_STATUS_PTR              ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_STATUS )

#define SENSORES_SAR_AVG_START_REG           (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_AVG_STAT )
#define SENSORES_SAR_AVG_START_PTR           ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_AVG_STAT )

#define SENSORES_SAR_INTR_REG                (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR )
#define SENSORES_SAR_INTR_PTR                ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR )

#define SENSORES_SAR_INTR_SET_REG            (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR_SET )
#define SENSORES_SAR_INTR_SET_PTR            ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR_SET )

#define SENSORES_SAR_INTR_MASK_REG           (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR_MASK )
#define SENSORES_SAR_INTR_MASK_PTR           ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR_MASK )

#define SENSORES_SAR_INTR_MASKED_REG         (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR_MASKED )
#define SENSORES_SAR_INTR_MASKED_PTR         ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR_MASKED )

#define SENSORES_SAR_SATURATE_INTR_REG       (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_SATURATE_INTR )
#define SENSORES_SAR_SATURATE_INTR_PTR       ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_SATURATE_INTR )

#define SENSORES_SAR_SATURATE_INTR_SET_REG   (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_SATURATE_INTR_SET )
#define SENSORES_SAR_SATURATE_INTR_SET_PTR   ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_SATURATE_INTR_SET )

#define SENSORES_SAR_SATURATE_INTR_MASK_REG (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )
#define SENSORES_SAR_SATURATE_INTR_MASK_PTR ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )

#define SENSORES_SAR_SATURATE_INTR_MASKED_REG \
                                                 (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )
#define SENSORES_SAR_SATURATE_INTR_MASKED_PTR \
                                                 ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )

#define SENSORES_SAR_RANGE_INTR_REG          (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_INTR )
#define SENSORES_SAR_RANGE_INTR_PTR          ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_INTR )

#define SENSORES_SAR_RANGE_INTR_SET_REG      (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_INTR_SET )
#define SENSORES_SAR_RANGE_INTR_SET_PTR      ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_INTR_SET )

#define SENSORES_SAR_RANGE_INTR_MASK_REG     (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_INTR_MASK )
#define SENSORES_SAR_RANGE_INTR_MASK_PTR     ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_INTR_MASK )

#define SENSORES_SAR_RANGE_INTR_MASKED_REG   (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )
#define SENSORES_SAR_RANGE_INTR_MASKED_PTR   ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )

#define SENSORES_SAR_INTR_CAUSE_REG          (*(reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR_CAUSE )
#define SENSORES_SAR_INTR_CAUSE_PTR          ( (reg32 *) SENSORES_cy_psoc4_sar__SAR_INTR_CAUSE )

#if(SENSORES_INJ_CHANNEL_ENABLED)
    #define SENSORES_SAR_INJ_CHAN_CONFIG_REG \
                                                 (*(reg32 *) SENSORES_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )
    #define SENSORES_SAR_INJ_CHAN_CONFIG_PTR    \
                                                 ( (reg32 *) SENSORES_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )

    #define SENSORES_SAR_INJ_RESULT_REG    (*(reg32 *) SENSORES_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
    #define SENSORES_SAR_INJ_RESULT_PTR    ( (reg32 *) SENSORES_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
#endif /* SENSORES_INJ_CHANNEL_ENABLED */
    
#define SENSORES_MUX_SWITCH0_REG           (*(reg32 *)  SENSORES_cy_psoc4_sar__SAR_MUX_SWITCH0 )
#define SENSORES_MUX_SWITCH0_PTR           ( (reg32 *)  SENSORES_cy_psoc4_sar__SAR_MUX_SWITCH0 )

#define SENSORES_MUX_SWITCH_HW_CTRL_REG    (*(reg32 *)  SENSORES_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )
#define SENSORES_MUX_SWITCH_HW_CTRL_PTR    ( (reg32 *)  SENSORES_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )

#define SENSORES_PUMP_CTRL_REG             (*(reg32 *)  SENSORES_cy_psoc4_sar__SAR_PUMP_CTRL )
#define SENSORES_PUMP_CTRL_PTR             ( (reg32 *)  SENSORES_cy_psoc4_sar__SAR_PUMP_CTRL )

#define SENSORES_ANA_TRIM_REG              (*(reg32 *)  SENSORES_cy_psoc4_sar__SAR_ANA_TRIM )
#define SENSORES_ANA_TRIM_PTR              ( (reg32 *)  SENSORES_cy_psoc4_sar__SAR_ANA_TRIM )

#define SENSORES_WOUNDING_REG              (*(reg32 *)  SENSORES_cy_psoc4_sar__SAR_WOUNDING )
#define SENSORES_WOUNDING_PTR              ( (reg32 *)  SENSORES_cy_psoc4_sar__SAR_WOUNDING )


/**************************************
*       Register Constants
**************************************/
#define SENSORES_INTC_NUMBER                (SENSORES_IRQ__INTC_NUMBER)
#define SENSORES_INTC_PRIOR_NUMBER          (SENSORES_IRQ__INTC_PRIOR_NUM)

/* defines for CTRL register */
#define SENSORES_VREF_INTERNAL1024          (0x00000040Lu)
#define SENSORES_VREF_EXTERNAL              (0x00000050Lu)
#define SENSORES_VREF_VDDA_2                (0x00000060Lu)
#define SENSORES_VREF_VDDA                  (0x00000070Lu)
#define SENSORES_VREF_INTERNAL1024BYPASSED  (0x000000C0Lu)
#define SENSORES_VREF_VDDA_2BYPASSED        (0x000000E0Lu)
#define SENSORES_VREF_INTERNALVREF          (0x00000040Lu)
#define SENSORES_VREF_INTERNALVREFBYPASSED  (0x000000C0Lu)

#define SENSORES_NEG_VSSA_KELVIN            (0x00000000Lu)
#define SENSORES_NEG_VSSA                   (0x00000200Lu)
#define SENSORES_NEG_VREF                   (0x00000E00Lu)
#if(SENSORES_TOTAL_CHANNELS_NUM > 1u)
    #define SENSORES_NEG_OTHER              (uint16)((uint16)SENSORES_cy_psoc4_sarmux_8__VNEG << 9u)
#else
    #define SENSORES_NEG_OTHER              (0)
#endif /* SENSORES_TOTAL_CHANNELS_NUM > 1u */

#define SENSORES_SAR_HW_CTRL_NEGVREF        (0x00002000Lu)

#define SENSORES_BOOSTPUMP_EN               (0x00100000Lu)

#define SENSORES_NORMAL_PWR                 (0x00000000Lu)
#define SENSORES_HALF_PWR                   (0x01000000Lu)
#define SENSORES_MORE_PWR                   (0x02000000Lu)
#define SENSORES_QUARTER_PWR                (0x03000000Lu)
#define SENSORES_DEEPSLEEP_ON               (0x08000000Lu)

#define SENSORES_DSI_SYNC_CONFIG            (0x10000000Lu)
#define SENSORES_DSI_MODE                   (0x20000000Lu)
#define SENSORES_SWITCH_DISABLE             (0x40000000Lu)
#define SENSORES_ENABLE                     (0x80000000Lu)

/* defines for STATUS register */
#define SENSORES_STATUS_BUSY                (0x80000000Lu)

/* defines for SAMPLE_CTRL register */
#define SENSORES_ALT_RESOLUTION_10BIT       (0x00000001Lu)
#define SENSORES_ALT_RESOLUTION_8BIT        (0x00000000Lu)

#define SENSORES_DATA_ALIGN_LEFT            (0x00000002Lu)
#define SENSORES_DATA_ALIGN_RIGHT           (0x00000000Lu)

#define SENSORES_SE_SIGNED_RESULT           (0x00000004Lu)
#define SENSORES_SE_UNSIGNED_RESULT         (0x00000000Lu)

#define SENSORES_DIFF_SIGNED_RESULT         (0x00000008Lu)
#define SENSORES_DIFF_UNSIGNED_RESULT       (0x00000000Lu)

#define SENSORES_AVG_CNT_OFFSET             (4u)
#define SENSORES_AVG_CNT_MASK               (0x00000070Lu)
#define SENSORES_AVG_SHIFT                  (0x00000080Lu)

#define SENSORES_CONTINUOUS_EN              (0x00010000Lu)
#define SENSORES_DSI_TRIGGER_EN             (0x00020000Lu)
#define SENSORES_DSI_TRIGGER_LEVEL          (0x00040000Lu)
#define SENSORES_DSI_SYNC_TRIGGER           (0x00080000Lu)
#define SENSORES_EOS_DSI_OUT_EN             (0x80000000Lu)

/* defines for SAMPLE_TIME01 / SAMPLE_TIME23 registers */
#define SENSORES_SAMPLE_TIME13_OFFSET       (16u)
#define SENSORES_SAMPLE_TIME02_MASK         (0x000003FFLu)
#define SENSORES_SAMPLE_TIME13_MASK         (0x03FF0000Lu)

/* defines for RANGE_THRES registers */
#define SENSORES_RANGE_HIGH_OFFSET          (16u)
#define SENSORES_RANGE_HIGH_MASK            (0xFFFF0000Lu)
#define SENSORES_RANGE_LOW_MASK             (0x0000FFFFLu)

/* defines for RANGE_COND register */
/* Compare mode setting constants:
*    BELOW   - Result < Low_Limit
*    INSIDE  - Low_Limit <= Result < High_Limit
*    ABOVE   - High_Limit <= Result
*    OUTSIDE - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define SENSORES_CMP_MODE_BELOW             (0x00000000Lu)
#define SENSORES_CMP_MODE_INSIDE            (0x40000000Lu)
#define SENSORES_CMP_MODE_ABOVE             (0x80000000Lu)
#define SENSORES_CMP_MODE_OUTSIDE           (0xC0000000Lu)
#define SENSORES_CMP_OFFSET                 (30u)

/* defines for _START_CTRL register */
#define SENSORES_FW_TRIGGER                 (0x00000001Lu)

/* defines for DFT_CTRL register */
#define SENSORES_DLY_INC                    (0x00000001Lu)
#define SENSORES_HIZ                        (0x00000002Lu)
#define SENSORES_DFT_INC_MASK               (0x000F0000Lu)
#define SENSORES_DFT_OUTC_MASK              (0x00700000Lu)
#define SENSORES_SEL_CSEL_DFT_MASK          (0x0F000000Lu)

/* configuration for clock speed > 9 Mhz based on
* characterization results
*/
#define SENSORES_SEL_CSEL_DFT_CHAR          (0x03000000Lu)
#define SENSORES_EN_CSEL_DFT                (0x10000000Lu)
#define SENSORES_DCEN                       (0x20000000Lu)
#define SENSORES_ADFT_OVERRIDE              (0x80000000Lu)

/* defines for CHAN_CONFIG / DIE_CHAN_CONFIG register
*  and channelsConfig parameter
*/
#define SENSORES_SARMUX_VIRT_SELECT         (0x00000070Lu)
#define SENSORES_DIFFERENTIAL_EN            (0x00000100Lu)
#define SENSORES_ALT_RESOLUTION_ON          (0x00000200Lu)
#define SENSORES_AVERAGING_EN               (0x00000400Lu)

#define SENSORES_SAMPLE_TIME_SEL_SHIFT      (12u)
#define SENSORES_SAMPLE_TIME_SEL_MASK       (0x00003000Lu)

#define SENSORES_CHANNEL_CONFIG_MASK        (0x00003700Lu)

/* for CHAN_CONFIG only */
#define SENSORES_DSI_OUT_EN                 (0x80000000Lu)

/* for INJ_CHAN_CONFIG only */
#define SENSORES_INJ_TAILGATING             (0x40000000Lu)
#define SENSORES_INJ_CHAN_EN                (0x80000000Lu)

/* defines for CHAN_WORK register */
#define SENSORES_SAR_WRK_MAX_12BIT          (0x00001000Lu)
#define SENSORES_SAR_WRK_MAX_10BIT          (0x00000400Lu)
#define SENSORES_SAR_WRK_MAX_8BIT           (0x00000100Lu)

/* defines for CHAN_RESULT register */
#define SENSORES_RESULT_MASK                (0x0000FFFFLu)
#define SENSORES_SATURATE_INTR_MIR          (0x20000000Lu)
#define SENSORES_RANGE_INTR_MIR             (0x40000000Lu)
#define SENSORES_CHAN_RESULT_VALID_MIR      (0x80000000Lu)

/* defines for INTR_MASK register */
#define SENSORES_EOS_MASK                   (0x00000001Lu)
#define SENSORES_OVERFLOW_MASK              (0x00000002Lu)
#define SENSORES_FW_COLLISION_MASK          (0x00000004Lu)
#define SENSORES_DSI_COLLISION_MASK         (0x00000008Lu)
#define SENSORES_INJ_EOC_MASK               (0x00000010Lu)
#define SENSORES_INJ_SATURATE_MASK          (0x00000020Lu)
#define SENSORES_INJ_RANGE_MASK             (0x00000040Lu)
#define SENSORES_INJ_COLLISION_MASK         (0x00000080Lu)

/* defines for INJ_RESULT register */
#define SENSORES_INJ_COLLISION_INTR_MIR     (0x10000000Lu)
#define SENSORES_INJ_SATURATE_INTR_MIR      (0x20000000Lu)
#define SENSORES_INJ_RANGE_INTR_MIR         (0x40000000Lu)
#define SENSORES_INJ_EOC_INTR_MIR           (0x80000000Lu)

/* defines for MUX_SWITCH0 register */
#define SENSORES_MUX_FW_VSSA_VMINUS         (0x00010000Lu)

/* defines for PUMP_CTRL register */
#define SENSORES_PUMP_CTRL_ENABLED          (0x80000000Lu)

/* additional defines for channelsConfig parameter */
#define SENSORES_IS_SATURATE_EN_MASK        (0x00000001Lu)
#define SENSORES_IS_RANGE_CTRL_EN_MASK      (0x00000002Lu)

/* defines for WOUNDING register */
#define SENSORES_WOUNDING_12BIT             (0x00000000Lu)
#define SENSORES_WOUNDING_10BIT             (0x00000001Lu)
#define SENSORES_WOUNDING_8BIT              (0x00000002Lu)

/* Trim value based on characterization */
#define SENSORES_TRIM_COEF                  (2u)

#if(SENSORES_MAX_RESOLUTION == SENSORES_RESOLUTION_10)
    #define SENSORES_ALT_WOUNDING           SENSORES_WOUNDING_10BIT
#else
    #define SENSORES_ALT_WOUNDING           SENSORES_WOUNDING_8BIT
#endif /* SENSORES_MAX_RESOLUTION == SENSORES_RESOLUTION_10 */

#if(SENSORES_DEFAULT_VREF_SEL == SENSORES__VDDA_2)
    #define SENSORES_DEFAULT_VREF_SOURCE    SENSORES_VREF_VDDA_2
#elif(SENSORES_DEFAULT_VREF_SEL == SENSORES__VDDA)
    #define SENSORES_DEFAULT_VREF_SOURCE    SENSORES_VREF_VDDA
#elif(SENSORES_DEFAULT_VREF_SEL == SENSORES__INTERNAL1024)
    #define SENSORES_DEFAULT_VREF_SOURCE    SENSORES_VREF_INTERNAL1024
#elif(SENSORES_DEFAULT_VREF_SEL == SENSORES__INTERNAL1024BYPASSED)
    #define SENSORES_DEFAULT_VREF_SOURCE    SENSORES_VREF_INTERNAL1024BYPASSED
#elif(SENSORES_DEFAULT_VREF_SEL == SENSORES__INTERNALVREF)
    #define SENSORES_DEFAULT_VREF_SOURCE    SENSORES_VREF_INTERNALVREF
#elif(SENSORES_DEFAULT_VREF_SEL == SENSORES__INTERNALVREFBYPASSED)
    #define SENSORES_DEFAULT_VREF_SOURCE    SENSORES_VREF_INTERNALVREFBYPASSED
#elif(SENSORES_DEFAULT_VREF_SEL == SENSORES__VDDA_2BYPASSED)
    #define SENSORES_DEFAULT_VREF_SOURCE    SENSORES_VREF_VDDA_2BYPASSED
#else
    #define SENSORES_DEFAULT_VREF_SOURCE    SENSORES_VREF_EXTERNAL
#endif /* SENSORES_DEFAULT_VREF_SEL == SENSORES__VDDA_2 */

#if(SENSORES_DEFAULT_NEG_INPUT_SEL == SENSORES__VSS)
    /* Connect NEG input of SARADC to VSSA close to the SARADC for single channel mode */
    #if(SENSORES_TOTAL_CHANNELS_NUM == 1u)
        #define SENSORES_DEFAULT_SE_NEG_INPUT    SENSORES_NEG_VSSA
    #else
        #define SENSORES_DEFAULT_SE_NEG_INPUT    SENSORES_NEG_VSSA_KELVIN
    #endif /* (SENSORES_TOTAL_CHANNELS_NUM == 1u) */
    /* Do not connect VSSA to VMINUS when one channel in differential mode used */
    #if((SENSORES_TOTAL_CHANNELS_NUM == 1u) && (SENSORES_CHANNELS_MODE != 0u))
        #define SENSORES_DEFAULT_MUX_SWITCH0     0u
    #else    /* miltiple channels or one single channel */
        #define SENSORES_DEFAULT_MUX_SWITCH0     SENSORES_MUX_FW_VSSA_VMINUS
    #endif /* (SENSORES_TOTAL_CHANNELS_NUM == 1u) */
#elif(SENSORES_DEFAULT_NEG_INPUT_SEL == SENSORES__VREF)
    /* Do not connect VNEG to VREF when one channel in differential mode used */
    #if((SENSORES_TOTAL_CHANNELS_NUM == 1u) && (SENSORES_CHANNELS_MODE != 0u))
        #define SENSORES_DEFAULT_SE_NEG_INPUT    0u
    #else    /* miltiple channels or one single channel */
        #define SENSORES_DEFAULT_SE_NEG_INPUT    SENSORES_NEG_VREF
    #endif /* (SENSORES_TOTAL_CHANNELS_NUM == 1u) */
    #define SENSORES_DEFAULT_MUX_SWITCH0     0u
#elif (SENSORES_SINGLE_PRESENT != 0u)
    #define SENSORES_DEFAULT_SE_NEG_INPUT    SENSORES_NEG_OTHER
    #define SENSORES_DEFAULT_MUX_SWITCH0     0u
#else
    #define SENSORES_DEFAULT_SE_NEG_INPUT    0u
    #define SENSORES_DEFAULT_MUX_SWITCH0     0u
#endif /* SENSORES_DEFAULT_NEG_INPUT_SEL == SENSORES__VREF */

/* If the SAR is configured for multiple channels, always set SAR_HW_CTRL_NEGVREF to 1 */
#if(SENSORES_TOTAL_CHANNELS_NUM == 1u)
    #define SENSORES_DEFAULT_HW_CTRL_NEGVREF 0u
#else
    #define SENSORES_DEFAULT_HW_CTRL_NEGVREF SENSORES_SAR_HW_CTRL_NEGVREF
#endif /* (SENSORES_TOTAL_CHANNELS_NUM == 1u) */


#if(SENSORES_DEFAULT_ALT_RESOLUTION_SEL == SENSORES__RES8)
    #define SENSORES_DEFAULT_ALT_RESOLUTION     (SENSORES_ALT_RESOLUTION_8BIT)
    #define SENSORES_DEFAULT_MAX_WRK_ALT        (SENSORES_SAR_WRK_MAX_8BIT)
#else
    #define SENSORES_DEFAULT_ALT_RESOLUTION     (SENSORES_ALT_RESOLUTION_10BIT)
    #define SENSORES_DEFAULT_MAX_WRK_ALT        (SENSORES_SAR_WRK_MAX_10BIT)
#endif /* End SENSORES_DEFAULT_ALT_RESOLUTION_SEL == SENSORES__RES8 */

#if(SENSORES_DEFAULT_JUSTIFICATION_SEL == SENSORES__RIGHT)
    #define SENSORES_DEFAULT_JUSTIFICATION  SENSORES_DATA_ALIGN_RIGHT
#else
    #define SENSORES_DEFAULT_JUSTIFICATION  SENSORES_DATA_ALIGN_LEFT
#endif /* SENSORES_DEFAULT_JUSTIFICATION_SEL == SENSORES__RIGHT */

#if(SENSORES_DEFAULT_DIFF_RESULT_FORMAT_SEL == SENSORES__FSIGNED)
    #define SENSORES_DEFAULT_DIFF_RESULT_FORMAT SENSORES_DIFF_SIGNED_RESULT
#else
    #define SENSORES_DEFAULT_DIFF_RESULT_FORMAT SENSORES_DIFF_UNSIGNED_RESULT
#endif /* SENSORES_DEFAULT_DIFF_RESULT_FORMAT_SEL == SENSORES__FSIGNED */

#if(SENSORES_DEFAULT_SE_RESULT_FORMAT_SEL == SENSORES__FSIGNED)
    #define SENSORES_DEFAULT_SE_RESULT_FORMAT SENSORES_SE_SIGNED_RESULT
#else
    #define SENSORES_DEFAULT_SE_RESULT_FORMAT SENSORES_SE_UNSIGNED_RESULT
#endif /* SENSORES_DEFAULT_SE_RESULT_FORMAT_SEL == SENSORES__FSIGNED */

#if(SENSORES_DEFAULT_SAMPLE_MODE_SEL == SENSORES__FREERUNNING)
    #define SENSORES_DSI_TRIGGER        0u
#else /* Firmware trigger */
    #define SENSORES_DSI_TRIGGER        (SENSORES_DSI_TRIGGER_EN | SENSORES_DSI_SYNC_TRIGGER)
#endif /* End SENSORES_DEFAULT_SAMPLE_MODE == SENSORES__FREERUNNING */

#if(SENSORES_INJ_CHANNEL_ENABLED)
    #define SENSORES_SAR_INTR_MASK      (SENSORES_EOS_MASK | SENSORES_INJ_EOC_MASK)
#else
    #define SENSORES_SAR_INTR_MASK      (SENSORES_EOS_MASK)
#endif /* SENSORES_INJ_CHANNEL_ENABLED*/

#if(SENSORES_DEFAULT_AVG_MODE == SENSORES__FIXEDRESOLUTION)
    #define SENSORES_AVG_SHIFT_MODE     SENSORES_AVG_SHIFT
#else
    #define SENSORES_AVG_SHIFT_MODE     0u
#endif /* End SENSORES_DEFAULT_AVG_MODE */

#define SENSORES_COMPARE_MODE           (uint32)((uint32)(SENSORES_DEFAULT_COMPARE_MODE) \
                                                << SENSORES_CMP_OFFSET)

#if(SENSORES_TOTAL_CHANNELS_NUM > 1u)
    #define SENSORES_DEFAULT_SWITCH_CONF    0u
#else /* Disable SAR sequencer from enabling routing switches in single channel mode */
    #define SENSORES_DEFAULT_SWITCH_CONF    SENSORES_SWITCH_DISABLE
#endif /* End SENSORES_TOTAL_CHANNELS_NUM > 1 */

#define SENSORES_DEFAULT_POWER \
       ((SENSORES_NOMINAL_CLOCK_FREQ > (SENSORES_MAX_FREQUENCY / 4)) ? SENSORES_NORMAL_PWR : \
       ((SENSORES_NOMINAL_CLOCK_FREQ > (SENSORES_MAX_FREQUENCY / 8)) ? SENSORES_HALF_PWR : \
                                                                                       SENSORES_QUARTER_PWR))

#define SENSORES_DEFAULT_CTRL_REG_CFG       (SENSORES_DEFAULT_VREF_SOURCE \
                                                   | SENSORES_DEFAULT_SE_NEG_INPUT \
                                                   | SENSORES_DEFAULT_HW_CTRL_NEGVREF \
                                                   | SENSORES_DEFAULT_POWER \
                                                   | SENSORES_DSI_SYNC_CONFIG \
                                                   | SENSORES_DEFAULT_SWITCH_CONF)

#define SENSORES_DEFAULT_SAMPLE_CTRL_REG_CFG (SENSORES_DEFAULT_DIFF_RESULT_FORMAT \
                                                    | SENSORES_DEFAULT_SE_RESULT_FORMAT \
                                                    | SENSORES_DEFAULT_JUSTIFICATION \
                                                    | SENSORES_DEFAULT_ALT_RESOLUTION \
                                           | (uint8)(SENSORES_DEFAULT_AVG_SAMPLES_NUM \
                                                   << SENSORES_AVG_CNT_OFFSET) \
                                                    | SENSORES_AVG_SHIFT_MODE \
                                                    | SENSORES_DSI_TRIGGER \
                                                    | SENSORES_EOS_DSI_OUT_EN)

#define SENSORES_DEFAULT_RANGE_THRES_REG_CFG (SENSORES_DEFAULT_LOW_LIMIT \
            | (uint32)((uint32)SENSORES_DEFAULT_HIGH_LIMIT << SENSORES_RANGE_HIGH_OFFSET))

#define SENSORES_DEFAULT_SAMPLE_TIME01_REG_CFG (SENSORES_DEFAULT_ACLKS_NUM \
            | (uint32)((uint32)SENSORES_DEFAULT_BCLKS_NUM << SENSORES_SAMPLE_TIME13_OFFSET))

#define SENSORES_DEFAULT_SAMPLE_TIME23_REG_CFG (SENSORES_DEFAULT_CCLKS_NUM \
            | (uint32)((uint32)SENSORES_DEFAULT_DCLKS_NUM << SENSORES_SAMPLE_TIME13_OFFSET))


#endif /* End CY_ADC_SAR_SEQ_SENSORES_H */


/* [] END OF FILE */
