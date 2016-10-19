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
int8 dato_enviado;
int8 dato_recibido;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    RECIBE_Start();
    RECIBE_LoadRxConfig();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("S...");
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        dato_recibido=RECIBE_ReadRxData();
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintInt8(dato_recibido); 
    }
}

/* [] END OF FILE */
