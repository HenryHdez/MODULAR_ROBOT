/*******************************************************************************
* File Name: SENSOR.h
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

#if !defined(CY_ADC_SAR_SEQ_SENSOR_H)
#define CY_ADC_SAR_SEQ_SENSOR_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*      Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} SENSOR_BACKUP_STRUCT;


/**************************************
*    Enumerated Types and Parameters
**************************************/

/*  Clock Source setting constants */
#define SENSOR__EXTERNAL 0
#define SENSOR__INTERNAL 1

/*  Sample Mode setting constants */
#define SENSOR__FREERUNNING 0
#define SENSOR__HARDWARESOC 1

/*  Reference type setting constants */
#define SENSOR__VDDA_2 0
#define SENSOR__VDDA 1
#define SENSOR__INTERNAL1024 2
#define SENSOR__INTERNAL1024BYPASSED 3
#define SENSOR__INTERNALVREF 4
#define SENSOR__INTERNALVREFBYPASSED 5
#define SENSOR__VDDA_2BYPASSED 6
#define SENSOR__EXTERNALVREF 7

/* Input buffer gain setting constants */
#define SENSOR__DISABLED 0
#define SENSOR__ONE 1
#define SENSOR__TWO 2
#define SENSOR__FOUR 3
#define SENSOR__EIGHT 4
#define SENSOR__SIXTEEN 5

/* Negative input setting sonstants in single ended mode */
#define SENSOR__VSS 0
#define SENSOR__VREF 1
#define SENSOR__OTHER 2

/* Compare mode setting constants:
*    Mode0 - Disable
*    Mode1 - Result < Low_Limit
*    Mode2 - Low_Limit <= Result < High_Limit
*    Mode3 - High_Limit <= Result
*    Mode4 - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define SENSOR__MODE0 0
#define SENSOR__MODE1 1
#define SENSOR__MODE2 2
#define SENSOR__MODE3 3

#define SENSOR__RES8 0
#define SENSOR__RES10 1

#define SENSOR__RIGHT 0
#define SENSOR__LEFT 1

#define SENSOR__FSIGNED 1
#define SENSOR__FUNSIGNED 0

#define SENSOR__ACCUMULATE 0
#define SENSOR__FIXEDRESOLUTION 1



/***************************************
*   Conditional Compilation Parameters
****************************************/ 

#define SENSOR_CY_SAR_IP_VER0               	(0u)
#define SENSOR_CY_SAR_IP_VER1               	(1u)

#if (CY_PSOC4_4100 || CY_PSOC4_4200)
    #define SENSOR_CY_SAR_IP_VER              (SENSOR_CY_SAR_IP_VER0)
#else /* Other devices */
    #define SENSOR_CY_SAR_IP_VER              (SENSOR_CY_SAR_IP_VER1)
#endif  /* (CY_PSOC4_4100 || CY_PSOC4_4200) */


/***************************************
*    Initial Parameter Constants
***************************************/
#define SENSOR_DEFAULT_SAMPLE_MODE_SEL        (0u)
#define SENSOR_DEFAULT_VREF_SEL               (3u)
#define SENSOR_DEFAULT_NEG_INPUT_SEL          (0u)
#define SENSOR_DEFAULT_ALT_RESOLUTION_SEL     (1u)
#define SENSOR_DEFAULT_JUSTIFICATION_SEL      (0u)
#define SENSOR_DEFAULT_DIFF_RESULT_FORMAT_SEL (1u)
#define SENSOR_DEFAULT_SE_RESULT_FORMAT_SEL   (1u)
#define SENSOR_DEFAULT_CLOCK_SOURCE           (1u)
#define SENSOR_DEFAULT_VREF_MV_VALUE          (1024)
#define SENSOR_DEFAULT_BUFFER_GAIN            (0u)
#define SENSOR_DEFAULT_AVG_SAMPLES_NUM        (0u)
#define SENSOR_DEFAULT_AVG_SAMPLES_DIV        (int16)(0x100u >> (7u - 0u))
#define SENSOR_DEFAULT_AVG_MODE               (1u)
#define SENSOR_MAX_RESOLUTION                 (12u)
#define SENSOR_DEFAULT_LOW_LIMIT              (0u)
#define SENSOR_DEFAULT_HIGH_LIMIT             (2047u)
#define SENSOR_DEFAULT_COMPARE_MODE           (0u)
#define SENSOR_DEFAULT_ACLKS_NUM              (4u)
#define SENSOR_DEFAULT_BCLKS_NUM              (4u)
#define SENSOR_DEFAULT_CCLKS_NUM              (4u)
#define SENSOR_DEFAULT_DCLKS_NUM              (4u)
#define SENSOR_TOTAL_CHANNELS_NUM             (1u)
#define SENSOR_SEQUENCED_CHANNELS_NUM         (1u)
#define SENSOR_DEFAULT_EN_CHANNELS            (1u)
#define SENSOR_NOMINAL_CLOCK_FREQ             (3000000)
#define SENSOR_INJ_CHANNEL_ENABLED            (0u)
#define SENSOR_IRQ_REMOVE                     (0u)

/* Determines whether the configuration contains external negative input. */
#define SENSOR_SINGLE_PRESENT                 (0u)
#define SENSOR_CHANNELS_MODE                  (0u)
#define SENSOR_MAX_CHANNELS_EN_MASK           (0xffffu >> (16u - SENSOR_SEQUENCED_CHANNELS_NUM))


/***************************************
*        Function Prototypes
***************************************/

void SENSOR_Start(void);
void SENSOR_Stop(void);
void SENSOR_Init(void);
void SENSOR_Enable(void);
void SENSOR_StartConvert(void);
void SENSOR_StopConvert(void);
uint32 SENSOR_IsEndConversion(uint32 retMode);
int16 SENSOR_GetResult16(uint32 chan);
void SENSOR_SetChanMask(uint32 mask);
void SENSOR_SetLowLimit(uint32 lowLimit);
void SENSOR_SetHighLimit(uint32 highLimit);
void SENSOR_SetLimitMask(uint32 mask);
void SENSOR_SetSatMask(uint32 mask);
void SENSOR_SetOffset(uint32 chan, int16 offset);
void SENSOR_SetGain(uint32 chan, int32 adcGain);
#if(SENSOR_INJ_CHANNEL_ENABLED)
    void SENSOR_EnableInjection(void);
#endif /* SENSOR_INJ_CHANNEL_ENABLED */
#if(SENSOR_DEFAULT_JUSTIFICATION_SEL == SENSOR__RIGHT)
    int16 SENSOR_CountsTo_mVolts(uint32 chan, int16 adcCounts);
    int32 SENSOR_CountsTo_uVolts(uint32 chan, int16 adcCounts);
    float32 SENSOR_CountsTo_Volts(uint32 chan, int16 adcCounts);
#endif /* End SENSOR_DEFAULT_JUSTIFICATION_SEL == SENSOR__RIGHT */
void SENSOR_Sleep(void);
void SENSOR_Wakeup(void);
void SENSOR_SaveConfig(void);
void SENSOR_RestoreConfig(void);

CY_ISR_PROTO( SENSOR_ISR );


/**************************************
*           API Constants
**************************************/
/* Constants for Sleep mode states */
#define SENSOR_DISABLED                   (0x00u)
#define SENSOR_ENABLED                    (0x01u)
#define SENSOR_STARTED                    (0x02u)
#define SENSOR_BOOSTPUMP_ENABLED          (0x04u)

/*   Constants for IsEndConversion() "retMode" parameter  */
#define SENSOR_RETURN_STATUS              (0x01u)
#define SENSOR_WAIT_FOR_RESULT            (0x02u)
#define SENSOR_RETURN_STATUS_INJ          (0x04u)
#define SENSOR_WAIT_FOR_RESULT_INJ        (0x08u)

#define SENSOR_MAX_FREQUENCY              (18000000)       /*18Mhz*/

#define SENSOR_RESOLUTION_12              (12u)
#define SENSOR_RESOLUTION_10              (10u)
#define SENSOR_RESOLUTION_8               (8u)

#define SENSOR_10US_DELAY                 (10u)

#define SENSOR_10V_COUNTS                 (10.0F)
#define SENSOR_10MV_COUNTS                (10000)
#define SENSOR_10UV_COUNTS                (10000000L)


/***************************************
* Global variables external identifier
***************************************/

extern uint8 SENSOR_initVar;
extern volatile int16 SENSOR_offset[SENSOR_TOTAL_CHANNELS_NUM];
extern volatile int32 SENSOR_countsPer10Volt[SENSOR_TOTAL_CHANNELS_NUM];


/***************************************
*              Registers
***************************************/

#define SENSOR_SAR_CTRL_REG                (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CTRL )
#define SENSOR_SAR_CTRL_PTR                ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CTRL )

#define SENSOR_SAR_SAMPLE_CTRL_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_SAMPLE_CTRL )
#define SENSOR_SAR_SAMPLE_CTRL_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_SAMPLE_CTRL )

#define SENSOR_SAR_SAMPLE_TIME01_REG       (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_SAMPLE_TIME01 )
#define SENSOR_SAR_SAMPLE_TIME01_PTR       ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_SAMPLE_TIME01 )

#define SENSOR_SAR_SAMPLE_TIME23_REG       (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_SAMPLE_TIME23 )
#define SENSOR_SAR_SAMPLE_TIME23_PTR       ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_SAMPLE_TIME23 )

#define SENSOR_SAR_RANGE_THRES_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_THRES )
#define SENSOR_SAR_RANGE_THRES_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_THRES )

#define SENSOR_SAR_RANGE_COND_REG          (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_COND )
#define SENSOR_SAR_RANGE_COND_PTR          ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_COND )

#define SENSOR_SAR_CHAN_EN_REG             (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_EN )
#define SENSOR_SAR_CHAN_EN_PTR             ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_EN )

#define SENSOR_SAR_START_CTRL_REG          (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_START_CTRL )
#define SENSOR_SAR_START_CTRL_PTR          ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_START_CTRL )

#define SENSOR_SAR_DFT_CTRL_REG            (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_DFT_CTRL )
#define SENSOR_SAR_DFT_CTRL_PTR            ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_DFT_CTRL )

#define SENSOR_SAR_CHAN_CONFIG_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define SENSOR_SAR_CHAN_CONFIG_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define SENSOR_SAR_CHAN_CONFIG_IND         SENSOR_cy_psoc4_sar__SAR_CHAN_CONFIG00

#define SENSOR_SAR_CHAN_WORK_REG           (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_WORK00 )
#define SENSOR_SAR_CHAN_WORK_PTR           ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_WORK00 )

#define SENSOR_SAR_CHAN_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define SENSOR_SAR_CHAN_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define SENSOR_SAR_CHAN_RESULT_IND         SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT00

#define SENSOR_SAR_CHAN0_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define SENSOR_SAR_CHAN0_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT00 )

#define SENSOR_SAR_CHAN1_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT01 )
#define SENSOR_SAR_CHAN1_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT01 )

#define SENSOR_SAR_CHAN2_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT02 )
#define SENSOR_SAR_CHAN2_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT02 )

#define SENSOR_SAR_CHAN3_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT03 )
#define SENSOR_SAR_CHAN3_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT03 )

#define SENSOR_SAR_CHAN4_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT04 )
#define SENSOR_SAR_CHAN4_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT04 )

#define SENSOR_SAR_CHAN5_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT05 )
#define SENSOR_SAR_CHAN5_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT05 )

#define SENSOR_SAR_CHAN6_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT06 )
#define SENSOR_SAR_CHAN6_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT06 )

#define SENSOR_SAR_CHAN7_RESULT_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT07 )
#define SENSOR_SAR_CHAN7_RESULT_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT07 )

#if(SENSOR_CY_SAR_IP_VER != SENSOR_CY_SAR_IP_VER0)
    #define SENSOR_SAR_CHAN8_RESULT_REG     (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT08 )
    #define SENSOR_SAR_CHAN8_RESULT_PTR     ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT08 )

    #define SENSOR_SAR_CHAN9_RESULT_REG     (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT09 )
    #define SENSOR_SAR_CHAN9_RESULT_PTR     ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT09 )

    #define SENSOR_SAR_CHAN10_RESULT_REG    (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT10 )
    #define SENSOR_SAR_CHAN10_RESULT_PTR    ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT10 )

    #define SENSOR_SAR_CHAN11_RESULT_REG    (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT11 )
    #define SENSOR_SAR_CHAN11_RESULT_PTR    ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT11 )

    #define SENSOR_SAR_CHAN12_RESULT_REG    (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT12 )
    #define SENSOR_SAR_CHAN12_RESULT_PTR    ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT12 )

    #define SENSOR_SAR_CHAN13_RESULT_REG    (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT13 )
    #define SENSOR_SAR_CHAN13_RESULT_PTR    ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT13 )

    #define SENSOR_SAR_CHAN14_RESULT_REG    (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT14 )
    #define SENSOR_SAR_CHAN14_RESULT_PTR    ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT14 )

    #define SENSOR_SAR_CHAN15_RESULT_REG    (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT15 )
    #define SENSOR_SAR_CHAN15_RESULT_PTR    ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT15 )
#endif /* (SENSOR_CY_SAR_IP_VER != SENSOR_CY_SAR_IP_VER0) */

#define SENSOR_SAR_CHAN_WORK_VALID_REG     (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_WORK_VALID)
#define SENSOR_SAR_CHAN_WORK_VALID_PTR     ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_WORK_VALID)

#define SENSOR_SAR_CHAN_RESULT_VALID_REG  ( *(reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )
#define SENSOR_SAR_CHAN_RESULT_VALID_PTR  ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )

#define SENSOR_SAR_STATUS_REG              (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_STATUS )
#define SENSOR_SAR_STATUS_PTR              ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_STATUS )

#define SENSOR_SAR_AVG_START_REG           (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_AVG_STAT )
#define SENSOR_SAR_AVG_START_PTR           ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_AVG_STAT )

#define SENSOR_SAR_INTR_REG                (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR )
#define SENSOR_SAR_INTR_PTR                ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR )

#define SENSOR_SAR_INTR_SET_REG            (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR_SET )
#define SENSOR_SAR_INTR_SET_PTR            ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR_SET )

#define SENSOR_SAR_INTR_MASK_REG           (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR_MASK )
#define SENSOR_SAR_INTR_MASK_PTR           ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR_MASK )

#define SENSOR_SAR_INTR_MASKED_REG         (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR_MASKED )
#define SENSOR_SAR_INTR_MASKED_PTR         ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR_MASKED )

#define SENSOR_SAR_SATURATE_INTR_REG       (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_SATURATE_INTR )
#define SENSOR_SAR_SATURATE_INTR_PTR       ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_SATURATE_INTR )

#define SENSOR_SAR_SATURATE_INTR_SET_REG   (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_SATURATE_INTR_SET )
#define SENSOR_SAR_SATURATE_INTR_SET_PTR   ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_SATURATE_INTR_SET )

#define SENSOR_SAR_SATURATE_INTR_MASK_REG (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )
#define SENSOR_SAR_SATURATE_INTR_MASK_PTR ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )

#define SENSOR_SAR_SATURATE_INTR_MASKED_REG \
                                                 (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )
#define SENSOR_SAR_SATURATE_INTR_MASKED_PTR \
                                                 ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )

#define SENSOR_SAR_RANGE_INTR_REG          (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_INTR )
#define SENSOR_SAR_RANGE_INTR_PTR          ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_INTR )

#define SENSOR_SAR_RANGE_INTR_SET_REG      (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_INTR_SET )
#define SENSOR_SAR_RANGE_INTR_SET_PTR      ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_INTR_SET )

#define SENSOR_SAR_RANGE_INTR_MASK_REG     (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_INTR_MASK )
#define SENSOR_SAR_RANGE_INTR_MASK_PTR     ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_INTR_MASK )

#define SENSOR_SAR_RANGE_INTR_MASKED_REG   (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )
#define SENSOR_SAR_RANGE_INTR_MASKED_PTR   ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )

#define SENSOR_SAR_INTR_CAUSE_REG          (*(reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR_CAUSE )
#define SENSOR_SAR_INTR_CAUSE_PTR          ( (reg32 *) SENSOR_cy_psoc4_sar__SAR_INTR_CAUSE )

#if(SENSOR_INJ_CHANNEL_ENABLED)
    #define SENSOR_SAR_INJ_CHAN_CONFIG_REG \
                                                 (*(reg32 *) SENSOR_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )
    #define SENSOR_SAR_INJ_CHAN_CONFIG_PTR    \
                                                 ( (reg32 *) SENSOR_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )

    #define SENSOR_SAR_INJ_RESULT_REG    (*(reg32 *) SENSOR_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
    #define SENSOR_SAR_INJ_RESULT_PTR    ( (reg32 *) SENSOR_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
#endif /* SENSOR_INJ_CHANNEL_ENABLED */
    
#define SENSOR_MUX_SWITCH0_REG           (*(reg32 *)  SENSOR_cy_psoc4_sar__SAR_MUX_SWITCH0 )
#define SENSOR_MUX_SWITCH0_PTR           ( (reg32 *)  SENSOR_cy_psoc4_sar__SAR_MUX_SWITCH0 )

#define SENSOR_MUX_SWITCH_HW_CTRL_REG    (*(reg32 *)  SENSOR_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )
#define SENSOR_MUX_SWITCH_HW_CTRL_PTR    ( (reg32 *)  SENSOR_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )

#define SENSOR_PUMP_CTRL_REG             (*(reg32 *)  SENSOR_cy_psoc4_sar__SAR_PUMP_CTRL )
#define SENSOR_PUMP_CTRL_PTR             ( (reg32 *)  SENSOR_cy_psoc4_sar__SAR_PUMP_CTRL )

#define SENSOR_ANA_TRIM_REG              (*(reg32 *)  SENSOR_cy_psoc4_sar__SAR_ANA_TRIM )
#define SENSOR_ANA_TRIM_PTR              ( (reg32 *)  SENSOR_cy_psoc4_sar__SAR_ANA_TRIM )

#define SENSOR_WOUNDING_REG              (*(reg32 *)  SENSOR_cy_psoc4_sar__SAR_WOUNDING )
#define SENSOR_WOUNDING_PTR              ( (reg32 *)  SENSOR_cy_psoc4_sar__SAR_WOUNDING )


/**************************************
*       Register Constants
**************************************/
#define SENSOR_INTC_NUMBER                (SENSOR_IRQ__INTC_NUMBER)
#define SENSOR_INTC_PRIOR_NUMBER          (SENSOR_IRQ__INTC_PRIOR_NUM)

/* defines for CTRL register */
#define SENSOR_VREF_INTERNAL1024          (0x00000040Lu)
#define SENSOR_VREF_EXTERNAL              (0x00000050Lu)
#define SENSOR_VREF_VDDA_2                (0x00000060Lu)
#define SENSOR_VREF_VDDA                  (0x00000070Lu)
#define SENSOR_VREF_INTERNAL1024BYPASSED  (0x000000C0Lu)
#define SENSOR_VREF_VDDA_2BYPASSED        (0x000000E0Lu)
#define SENSOR_VREF_INTERNALVREF          (0x00000040Lu)
#define SENSOR_VREF_INTERNALVREFBYPASSED  (0x000000C0Lu)

#define SENSOR_NEG_VSSA_KELVIN            (0x00000000Lu)
#define SENSOR_NEG_VSSA                   (0x00000200Lu)
#define SENSOR_NEG_VREF                   (0x00000E00Lu)
#if(SENSOR_TOTAL_CHANNELS_NUM > 1u)
    #define SENSOR_NEG_OTHER              (uint16)((uint16)SENSOR_cy_psoc4_sarmux_8__VNEG << 9u)
#else
    #define SENSOR_NEG_OTHER              (0)
#endif /* SENSOR_TOTAL_CHANNELS_NUM > 1u */

#define SENSOR_SAR_HW_CTRL_NEGVREF        (0x00002000Lu)

#define SENSOR_BOOSTPUMP_EN               (0x00100000Lu)

#define SENSOR_NORMAL_PWR                 (0x00000000Lu)
#define SENSOR_HALF_PWR                   (0x01000000Lu)
#define SENSOR_MORE_PWR                   (0x02000000Lu)
#define SENSOR_QUARTER_PWR                (0x03000000Lu)
#define SENSOR_DEEPSLEEP_ON               (0x08000000Lu)

#define SENSOR_DSI_SYNC_CONFIG            (0x10000000Lu)
#define SENSOR_DSI_MODE                   (0x20000000Lu)
#define SENSOR_SWITCH_DISABLE             (0x40000000Lu)
#define SENSOR_ENABLE                     (0x80000000Lu)

/* defines for STATUS register */
#define SENSOR_STATUS_BUSY                (0x80000000Lu)

/* defines for SAMPLE_CTRL register */
#define SENSOR_ALT_RESOLUTION_10BIT       (0x00000001Lu)
#define SENSOR_ALT_RESOLUTION_8BIT        (0x00000000Lu)

#define SENSOR_DATA_ALIGN_LEFT            (0x00000002Lu)
#define SENSOR_DATA_ALIGN_RIGHT           (0x00000000Lu)

#define SENSOR_SE_SIGNED_RESULT           (0x00000004Lu)
#define SENSOR_SE_UNSIGNED_RESULT         (0x00000000Lu)

#define SENSOR_DIFF_SIGNED_RESULT         (0x00000008Lu)
#define SENSOR_DIFF_UNSIGNED_RESULT       (0x00000000Lu)

#define SENSOR_AVG_CNT_OFFSET             (4u)
#define SENSOR_AVG_CNT_MASK               (0x00000070Lu)
#define SENSOR_AVG_SHIFT                  (0x00000080Lu)

#define SENSOR_CONTINUOUS_EN              (0x00010000Lu)
#define SENSOR_DSI_TRIGGER_EN             (0x00020000Lu)
#define SENSOR_DSI_TRIGGER_LEVEL          (0x00040000Lu)
#define SENSOR_DSI_SYNC_TRIGGER           (0x00080000Lu)
#define SENSOR_EOS_DSI_OUT_EN             (0x80000000Lu)

/* defines for SAMPLE_TIME01 / SAMPLE_TIME23 registers */
#define SENSOR_SAMPLE_TIME13_OFFSET       (16u)
#define SENSOR_SAMPLE_TIME02_MASK         (0x000003FFLu)
#define SENSOR_SAMPLE_TIME13_MASK         (0x03FF0000Lu)

/* defines for RANGE_THRES registers */
#define SENSOR_RANGE_HIGH_OFFSET          (16u)
#define SENSOR_RANGE_HIGH_MASK            (0xFFFF0000Lu)
#define SENSOR_RANGE_LOW_MASK             (0x0000FFFFLu)

/* defines for RANGE_COND register */
/* Compare mode setting constants:
*    BELOW   - Result < Low_Limit
*    INSIDE  - Low_Limit <= Result < High_Limit
*    ABOVE   - High_Limit <= Result
*    OUTSIDE - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define SENSOR_CMP_MODE_BELOW             (0x00000000Lu)
#define SENSOR_CMP_MODE_INSIDE            (0x40000000Lu)
#define SENSOR_CMP_MODE_ABOVE             (0x80000000Lu)
#define SENSOR_CMP_MODE_OUTSIDE           (0xC0000000Lu)
#define SENSOR_CMP_OFFSET                 (30u)

/* defines for _START_CTRL register */
#define SENSOR_FW_TRIGGER                 (0x00000001Lu)

/* defines for DFT_CTRL register */
#define SENSOR_DLY_INC                    (0x00000001Lu)
#define SENSOR_HIZ                        (0x00000002Lu)
#define SENSOR_DFT_INC_MASK               (0x000F0000Lu)
#define SENSOR_DFT_OUTC_MASK              (0x00700000Lu)
#define SENSOR_SEL_CSEL_DFT_MASK          (0x0F000000Lu)

/* configuration for clock speed > 9 Mhz based on
* characterization results
*/
#define SENSOR_SEL_CSEL_DFT_CHAR          (0x03000000Lu)
#define SENSOR_EN_CSEL_DFT                (0x10000000Lu)
#define SENSOR_DCEN                       (0x20000000Lu)
#define SENSOR_ADFT_OVERRIDE              (0x80000000Lu)

/* defines for CHAN_CONFIG / DIE_CHAN_CONFIG register
*  and channelsConfig parameter
*/
#define SENSOR_SARMUX_VIRT_SELECT         (0x00000070Lu)
#define SENSOR_DIFFERENTIAL_EN            (0x00000100Lu)
#define SENSOR_ALT_RESOLUTION_ON          (0x00000200Lu)
#define SENSOR_AVERAGING_EN               (0x00000400Lu)

#define SENSOR_SAMPLE_TIME_SEL_SHIFT      (12u)
#define SENSOR_SAMPLE_TIME_SEL_MASK       (0x00003000Lu)

#define SENSOR_CHANNEL_CONFIG_MASK        (0x00003700Lu)

/* for CHAN_CONFIG only */
#define SENSOR_DSI_OUT_EN                 (0x80000000Lu)

/* for INJ_CHAN_CONFIG only */
#define SENSOR_INJ_TAILGATING             (0x40000000Lu)
#define SENSOR_INJ_CHAN_EN                (0x80000000Lu)

/* defines for CHAN_WORK register */
#define SENSOR_SAR_WRK_MAX_12BIT          (0x00001000Lu)
#define SENSOR_SAR_WRK_MAX_10BIT          (0x00000400Lu)
#define SENSOR_SAR_WRK_MAX_8BIT           (0x00000100Lu)

/* defines for CHAN_RESULT register */
#define SENSOR_RESULT_MASK                (0x0000FFFFLu)
#define SENSOR_SATURATE_INTR_MIR          (0x20000000Lu)
#define SENSOR_RANGE_INTR_MIR             (0x40000000Lu)
#define SENSOR_CHAN_RESULT_VALID_MIR      (0x80000000Lu)

/* defines for INTR_MASK register */
#define SENSOR_EOS_MASK                   (0x00000001Lu)
#define SENSOR_OVERFLOW_MASK              (0x00000002Lu)
#define SENSOR_FW_COLLISION_MASK          (0x00000004Lu)
#define SENSOR_DSI_COLLISION_MASK         (0x00000008Lu)
#define SENSOR_INJ_EOC_MASK               (0x00000010Lu)
#define SENSOR_INJ_SATURATE_MASK          (0x00000020Lu)
#define SENSOR_INJ_RANGE_MASK             (0x00000040Lu)
#define SENSOR_INJ_COLLISION_MASK         (0x00000080Lu)

/* defines for INJ_RESULT register */
#define SENSOR_INJ_COLLISION_INTR_MIR     (0x10000000Lu)
#define SENSOR_INJ_SATURATE_INTR_MIR      (0x20000000Lu)
#define SENSOR_INJ_RANGE_INTR_MIR         (0x40000000Lu)
#define SENSOR_INJ_EOC_INTR_MIR           (0x80000000Lu)

/* defines for MUX_SWITCH0 register */
#define SENSOR_MUX_FW_VSSA_VMINUS         (0x00010000Lu)

/* defines for PUMP_CTRL register */
#define SENSOR_PUMP_CTRL_ENABLED          (0x80000000Lu)

/* additional defines for channelsConfig parameter */
#define SENSOR_IS_SATURATE_EN_MASK        (0x00000001Lu)
#define SENSOR_IS_RANGE_CTRL_EN_MASK      (0x00000002Lu)

/* defines for WOUNDING register */
#define SENSOR_WOUNDING_12BIT             (0x00000000Lu)
#define SENSOR_WOUNDING_10BIT             (0x00000001Lu)
#define SENSOR_WOUNDING_8BIT              (0x00000002Lu)

/* Trim value based on characterization */
#define SENSOR_TRIM_COEF                  (2u)

#if(SENSOR_MAX_RESOLUTION == SENSOR_RESOLUTION_10)
    #define SENSOR_ALT_WOUNDING           SENSOR_WOUNDING_10BIT
#else
    #define SENSOR_ALT_WOUNDING           SENSOR_WOUNDING_8BIT
#endif /* SENSOR_MAX_RESOLUTION == SENSOR_RESOLUTION_10 */

#if(SENSOR_DEFAULT_VREF_SEL == SENSOR__VDDA_2)
    #define SENSOR_DEFAULT_VREF_SOURCE    SENSOR_VREF_VDDA_2
#elif(SENSOR_DEFAULT_VREF_SEL == SENSOR__VDDA)
    #define SENSOR_DEFAULT_VREF_SOURCE    SENSOR_VREF_VDDA
#elif(SENSOR_DEFAULT_VREF_SEL == SENSOR__INTERNAL1024)
    #define SENSOR_DEFAULT_VREF_SOURCE    SENSOR_VREF_INTERNAL1024
#elif(SENSOR_DEFAULT_VREF_SEL == SENSOR__INTERNAL1024BYPASSED)
    #define SENSOR_DEFAULT_VREF_SOURCE    SENSOR_VREF_INTERNAL1024BYPASSED
#elif(SENSOR_DEFAULT_VREF_SEL == SENSOR__INTERNALVREF)
    #define SENSOR_DEFAULT_VREF_SOURCE    SENSOR_VREF_INTERNALVREF
#elif(SENSOR_DEFAULT_VREF_SEL == SENSOR__INTERNALVREFBYPASSED)
    #define SENSOR_DEFAULT_VREF_SOURCE    SENSOR_VREF_INTERNALVREFBYPASSED
#elif(SENSOR_DEFAULT_VREF_SEL == SENSOR__VDDA_2BYPASSED)
    #define SENSOR_DEFAULT_VREF_SOURCE    SENSOR_VREF_VDDA_2BYPASSED
#else
    #define SENSOR_DEFAULT_VREF_SOURCE    SENSOR_VREF_EXTERNAL
#endif /* SENSOR_DEFAULT_VREF_SEL == SENSOR__VDDA_2 */

#if(SENSOR_DEFAULT_NEG_INPUT_SEL == SENSOR__VSS)
    /* Connect NEG input of SARADC to VSSA close to the SARADC for single channel mode */
    #if(SENSOR_TOTAL_CHANNELS_NUM == 1u)
        #define SENSOR_DEFAULT_SE_NEG_INPUT    SENSOR_NEG_VSSA
    #else
        #define SENSOR_DEFAULT_SE_NEG_INPUT    SENSOR_NEG_VSSA_KELVIN
    #endif /* (SENSOR_TOTAL_CHANNELS_NUM == 1u) */
    /* Do not connect VSSA to VMINUS when one channel in differential mode used */
    #if((SENSOR_TOTAL_CHANNELS_NUM == 1u) && (SENSOR_CHANNELS_MODE != 0u))
        #define SENSOR_DEFAULT_MUX_SWITCH0     0u
    #else    /* miltiple channels or one single channel */
        #define SENSOR_DEFAULT_MUX_SWITCH0     SENSOR_MUX_FW_VSSA_VMINUS
    #endif /* (SENSOR_TOTAL_CHANNELS_NUM == 1u) */
#elif(SENSOR_DEFAULT_NEG_INPUT_SEL == SENSOR__VREF)
    /* Do not connect VNEG to VREF when one channel in differential mode used */
    #if((SENSOR_TOTAL_CHANNELS_NUM == 1u) && (SENSOR_CHANNELS_MODE != 0u))
        #define SENSOR_DEFAULT_SE_NEG_INPUT    0u
    #else    /* miltiple channels or one single channel */
        #define SENSOR_DEFAULT_SE_NEG_INPUT    SENSOR_NEG_VREF
    #endif /* (SENSOR_TOTAL_CHANNELS_NUM == 1u) */
    #define SENSOR_DEFAULT_MUX_SWITCH0     0u
#elif (SENSOR_SINGLE_PRESENT != 0u)
    #define SENSOR_DEFAULT_SE_NEG_INPUT    SENSOR_NEG_OTHER
    #define SENSOR_DEFAULT_MUX_SWITCH0     0u
#else
    #define SENSOR_DEFAULT_SE_NEG_INPUT    0u
    #define SENSOR_DEFAULT_MUX_SWITCH0     0u
#endif /* SENSOR_DEFAULT_NEG_INPUT_SEL == SENSOR__VREF */

/* If the SAR is configured for multiple channels, always set SAR_HW_CTRL_NEGVREF to 1 */
#if(SENSOR_TOTAL_CHANNELS_NUM == 1u)
    #define SENSOR_DEFAULT_HW_CTRL_NEGVREF 0u
#else
    #define SENSOR_DEFAULT_HW_CTRL_NEGVREF SENSOR_SAR_HW_CTRL_NEGVREF
#endif /* (SENSOR_TOTAL_CHANNELS_NUM == 1u) */


#if(SENSOR_DEFAULT_ALT_RESOLUTION_SEL == SENSOR__RES8)
    #define SENSOR_DEFAULT_ALT_RESOLUTION     (SENSOR_ALT_RESOLUTION_8BIT)
    #define SENSOR_DEFAULT_MAX_WRK_ALT        (SENSOR_SAR_WRK_MAX_8BIT)
#else
    #define SENSOR_DEFAULT_ALT_RESOLUTION     (SENSOR_ALT_RESOLUTION_10BIT)
    #define SENSOR_DEFAULT_MAX_WRK_ALT        (SENSOR_SAR_WRK_MAX_10BIT)
#endif /* End SENSOR_DEFAULT_ALT_RESOLUTION_SEL == SENSOR__RES8 */

#if(SENSOR_DEFAULT_JUSTIFICATION_SEL == SENSOR__RIGHT)
    #define SENSOR_DEFAULT_JUSTIFICATION  SENSOR_DATA_ALIGN_RIGHT
#else
    #define SENSOR_DEFAULT_JUSTIFICATION  SENSOR_DATA_ALIGN_LEFT
#endif /* SENSOR_DEFAULT_JUSTIFICATION_SEL == SENSOR__RIGHT */

#if(SENSOR_DEFAULT_DIFF_RESULT_FORMAT_SEL == SENSOR__FSIGNED)
    #define SENSOR_DEFAULT_DIFF_RESULT_FORMAT SENSOR_DIFF_SIGNED_RESULT
#else
    #define SENSOR_DEFAULT_DIFF_RESULT_FORMAT SENSOR_DIFF_UNSIGNED_RESULT
#endif /* SENSOR_DEFAULT_DIFF_RESULT_FORMAT_SEL == SENSOR__FSIGNED */

#if(SENSOR_DEFAULT_SE_RESULT_FORMAT_SEL == SENSOR__FSIGNED)
    #define SENSOR_DEFAULT_SE_RESULT_FORMAT SENSOR_SE_SIGNED_RESULT
#else
    #define SENSOR_DEFAULT_SE_RESULT_FORMAT SENSOR_SE_UNSIGNED_RESULT
#endif /* SENSOR_DEFAULT_SE_RESULT_FORMAT_SEL == SENSOR__FSIGNED */

#if(SENSOR_DEFAULT_SAMPLE_MODE_SEL == SENSOR__FREERUNNING)
    #define SENSOR_DSI_TRIGGER        0u
#else /* Firmware trigger */
    #define SENSOR_DSI_TRIGGER        (SENSOR_DSI_TRIGGER_EN | SENSOR_DSI_SYNC_TRIGGER)
#endif /* End SENSOR_DEFAULT_SAMPLE_MODE == SENSOR__FREERUNNING */

#if(SENSOR_INJ_CHANNEL_ENABLED)
    #define SENSOR_SAR_INTR_MASK      (SENSOR_EOS_MASK | SENSOR_INJ_EOC_MASK)
#else
    #define SENSOR_SAR_INTR_MASK      (SENSOR_EOS_MASK)
#endif /* SENSOR_INJ_CHANNEL_ENABLED*/

#if(SENSOR_DEFAULT_AVG_MODE == SENSOR__FIXEDRESOLUTION)
    #define SENSOR_AVG_SHIFT_MODE     SENSOR_AVG_SHIFT
#else
    #define SENSOR_AVG_SHIFT_MODE     0u
#endif /* End SENSOR_DEFAULT_AVG_MODE */

#define SENSOR_COMPARE_MODE           (uint32)((uint32)(SENSOR_DEFAULT_COMPARE_MODE) \
                                                << SENSOR_CMP_OFFSET)

#if(SENSOR_TOTAL_CHANNELS_NUM > 1u)
    #define SENSOR_DEFAULT_SWITCH_CONF    0u
#else /* Disable SAR sequencer from enabling routing switches in single channel mode */
    #define SENSOR_DEFAULT_SWITCH_CONF    SENSOR_SWITCH_DISABLE
#endif /* End SENSOR_TOTAL_CHANNELS_NUM > 1 */

#define SENSOR_DEFAULT_POWER \
       ((SENSOR_NOMINAL_CLOCK_FREQ > (SENSOR_MAX_FREQUENCY / 4)) ? SENSOR_NORMAL_PWR : \
       ((SENSOR_NOMINAL_CLOCK_FREQ > (SENSOR_MAX_FREQUENCY / 8)) ? SENSOR_HALF_PWR : \
                                                                                       SENSOR_QUARTER_PWR))

#define SENSOR_DEFAULT_CTRL_REG_CFG       (SENSOR_DEFAULT_VREF_SOURCE \
                                                   | SENSOR_DEFAULT_SE_NEG_INPUT \
                                                   | SENSOR_DEFAULT_HW_CTRL_NEGVREF \
                                                   | SENSOR_DEFAULT_POWER \
                                                   | SENSOR_DSI_SYNC_CONFIG \
                                                   | SENSOR_DEFAULT_SWITCH_CONF)

#define SENSOR_DEFAULT_SAMPLE_CTRL_REG_CFG (SENSOR_DEFAULT_DIFF_RESULT_FORMAT \
                                                    | SENSOR_DEFAULT_SE_RESULT_FORMAT \
                                                    | SENSOR_DEFAULT_JUSTIFICATION \
                                                    | SENSOR_DEFAULT_ALT_RESOLUTION \
                                           | (uint8)(SENSOR_DEFAULT_AVG_SAMPLES_NUM \
                                                   << SENSOR_AVG_CNT_OFFSET) \
                                                    | SENSOR_AVG_SHIFT_MODE \
                                                    | SENSOR_DSI_TRIGGER \
                                                    | SENSOR_EOS_DSI_OUT_EN)

#define SENSOR_DEFAULT_RANGE_THRES_REG_CFG (SENSOR_DEFAULT_LOW_LIMIT \
            | (uint32)((uint32)SENSOR_DEFAULT_HIGH_LIMIT << SENSOR_RANGE_HIGH_OFFSET))

#define SENSOR_DEFAULT_SAMPLE_TIME01_REG_CFG (SENSOR_DEFAULT_ACLKS_NUM \
            | (uint32)((uint32)SENSOR_DEFAULT_BCLKS_NUM << SENSOR_SAMPLE_TIME13_OFFSET))

#define SENSOR_DEFAULT_SAMPLE_TIME23_REG_CFG (SENSOR_DEFAULT_CCLKS_NUM \
            | (uint32)((uint32)SENSOR_DEFAULT_DCLKS_NUM << SENSOR_SAMPLE_TIME13_OFFSET))


#endif /* End CY_ADC_SAR_SEQ_SENSOR_H */


/* [] END OF FILE */
