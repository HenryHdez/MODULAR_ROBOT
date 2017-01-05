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
uint8 dato_enviado;

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
        CyDelay(10);
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintHexUint8(dato_recibido);
    }
}

/* [] END OF FILE */
