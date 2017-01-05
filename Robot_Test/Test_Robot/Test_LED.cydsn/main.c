/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        LED_1_Write(0);
        LED_2_Write(0);
        LED_3_Write(0);
        LED_4_Write(0);
        CyDelay(100);
        LED_1_Write(1);
        LED_2_Write(1);
        LED_3_Write(1);
        LED_4_Write(1);    
        CyDelay(200);
    }
}

/* [] END OF FILE */
