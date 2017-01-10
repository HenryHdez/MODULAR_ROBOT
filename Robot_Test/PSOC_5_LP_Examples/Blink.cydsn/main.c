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

int32 RESULT[MUX_CHANNELS];

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    SENSORES_Start();
    MUX_Start();
    MUX_Next();
    SENSORES_StartConvert();
    SENSORES_IsEndConversion(SENSORES_WAIT_FOR_RESULT);
    for(;;)
    {
        SENSORES_StopConvert();
        RESULT[MUX_GetChannel()]=SENSORES_GetResult16(0);
        if(RESULT[0]<2000){LED_1_Write(1);}
        else{LED_1_Write(0);}
        if(RESULT[1]<680){LED_2_Write(1);}
        else{LED_2_Write(0);}
        if(RESULT[2]<3800){LED_3_Write(1);}
        else{LED_3_Write(0);}
        if(RESULT[3]<1500){LED_4_Write(1);}
        else{LED_4_Write(0);}  
        MUX_Next();
        SENSORES_StartConvert();
    }
}

/* [] END OF FILE */
