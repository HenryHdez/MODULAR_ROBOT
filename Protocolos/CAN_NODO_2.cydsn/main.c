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
uint8 dato_recibido;
uint8 dato_recibido1;
uint8 dato_recibido2;
uint8 dato_recibido3;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    RX_CAN_Start();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("Recibiendo...");
    for(;;)
    {
        RX_CAN_ReceiveMsg0();
        RX_CAN_ReceiveMsg3();
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintHexUint8(dato_recibido);
        LCD_Char_1_Position(1,3);
        LCD_Char_1_PrintHexUint8(dato_recibido1);
        LCD_Char_1_Position(1,6);
        LCD_Char_1_PrintHexUint8(dato_recibido2);
        LCD_Char_1_Position(1,9);
        LCD_Char_1_PrintHexUint8(dato_recibido3);        
    }
}

/* [] END OF FILE */
