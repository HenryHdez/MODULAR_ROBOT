/*******************************************************************************
* File Name: SENSOR_PWM_INT.c
* Version 2.40
*
*  Description:
*    This file contains the code that operates during the ADC_SAR interrupt
*    service routine.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SENSOR_PWM.h"
#include "cyapicallbacks.h"


/******************************************************************************
* Custom Declarations and Variables
* - add user inlcude files, prototypes and variables between the following
*   #START and #END tags
******************************************************************************/
/* `#START ADC_SYS_VAR`  */

/* `#END`  */

#if(SENSOR_PWM_IRQ_REMOVE == 0u)


    /******************************************************************************
    * Function Name: SENSOR_PWM_ISR
    *******************************************************************************
    *
    * Summary:
    *  Handle Interrupt Service Routine.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    ******************************************************************************/
    CY_ISR( SENSOR_PWM_ISR )
    {
        uint32 intr_status;

        /* Read interrupt status register */
        intr_status = SENSOR_PWM_SAR_INTR_REG;

        #ifdef SENSOR_PWM_ISR_INTERRUPT_CALLBACK
            SENSOR_PWM_ISR_InterruptCallback();
        #endif /* SENSOR_PWM_ISR_INTERRUPT_CALLBACK */


        /************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        *************************************************************************/
        /* `#START MAIN_ADC_ISR`  */

        /* `#END`  */
        
        /* Clear handled interrupt */
        SENSOR_PWM_SAR_INTR_REG = intr_status;
    }

#endif   /* End SENSOR_PWM_IRQ_REMOVE */


/* [] END OF FILE */
