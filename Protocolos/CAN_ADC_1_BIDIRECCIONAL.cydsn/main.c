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
#include "cyapicallbacks.h"

uint8 dato_enviado;
uint8 dato_recibido;
int8 ADC_Vec[AMUX_CHANNELS];
int8 flag;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    TX_CAN_Start();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("Enviando...");
    ADC_Start();
    ADC_IRQ_Start();
    AMUX_Start();
    ADC_StartConvert();
    AMUX_Next();
    for(;;)
    {
        TX_CAN_SendMsg0();
        CyDelay(10);
        if(flag==1){
            ADC_StopConvert();
            ADC_Vec[AMUX_GetChannel()]=ADC_GetResult8();
            dato_enviado=ADC_GetResult8();
            LCD_Char_1_Position(1,0);
            LCD_Char_1_PrintHexUint8(ADC_Vec[0]);             
            LCD_Char_1_Position(1,4);
            LCD_Char_1_PrintHexUint8(ADC_Vec[1]);
            LCD_Char_1_Position(1,8);
            LCD_Char_1_PrintHexUint8(ADC_Vec[2]);
            LCD_Char_1_Position(1,12);
            LCD_Char_1_PrintHexUint8(ADC_Vec[3]); 
            AMUX_Next();
            flag=0;
            ADC_StartConvert();
            }
        CyDelay(1000);
    }
}

void ADC_IRQ_Interrupt_InterruptCallback(void){
    flag=1;
    return;
}
/* [] END OF FILE */
