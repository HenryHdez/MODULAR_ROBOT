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

uint8 dato_recibido[8];
uint8 dato_enviado;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    RX_CAN_Start();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    //LCD_Char_1_PrintString("Recibiendo...");
    dato_enviado=0;
    for(;;)
    {
        LCD_Char_1_Position(0,0);
        LCD_Char_1_PrintHexUint8(dato_recibido[0]);
        LCD_Char_1_Position(0,3);
        LCD_Char_1_PrintHexUint8(dato_recibido[1]);   
        LCD_Char_1_Position(0,6);
        LCD_Char_1_PrintHexUint8(dato_recibido[2]);   
        LCD_Char_1_Position(0,9);
        LCD_Char_1_PrintHexUint8(dato_recibido[3]);   
        LCD_Char_1_Position(0,12);
        LCD_Char_1_PrintHexUint8(dato_recibido[4]);         
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintHexUint8(dato_recibido[5]);
        LCD_Char_1_Position(1,3);
        LCD_Char_1_PrintHexUint8(dato_recibido[6]);   
        LCD_Char_1_Position(1,6);
        LCD_Char_1_PrintHexUint8(dato_recibido[7]); 
        CyDelay(500);
    }
}

/* [] END OF FILE */
