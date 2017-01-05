/*******************************************************************************
* File Name: SENSORES_INT.c
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

#include "SENSORES.h"
#include "cyapicallbacks.h"


/******************************************************************************
* Custom Declarations and Variables
* - add user inlcude files, prototypes and variables between the following
*   #START and #END tags
******************************************************************************/
/* `#START ADC_SYS_VAR`  */

/* `#END`  */

#if(SENSORES_IRQ_REMOVE == 0u)


    /******************************************************************************
    * Function Name: SENSORES_ISR
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
    CY_ISR( SENSORES_ISR )
    {
        uint32 intr_status;

        /* Read interrupt status register */
        intr_status = SENSORES_SAR_INTR_REG;

        #ifdef SENSORES_ISR_INTERRUPT_CALLBACK
            SENSORES_ISR_InterruptCallback();
        #endif /* SENSORES_ISR_INTERRUPT_CALLBACK */


        /************************************************************************
        *  Custom Code
        *  - add user ISR code between the following #START and #END tags
        *************************************************************************/
        /* `#START MAIN_ADC_ISR`  */

        /* `#END`  */
        
        /* Clear handled interrupt */
        SENSORES_SAR_INTR_REG = intr_status;
    }

#endif   /* End SENSORES_IRQ_REMOVE */


/* [] END OF FILE */
