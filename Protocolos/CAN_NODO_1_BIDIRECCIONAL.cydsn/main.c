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
uint8 dato_enviado;
uint8 dato_recibido;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    TX_CAN_Start();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("Enviando...");
    for(;;)
    {
        TX_CAN_SendMsg0();
        CyDelay(10);
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintHexUint8(dato_enviado);     
        LCD_Char_1_Position(1,5);
        LCD_Char_1_PrintHexUint8(dato_recibido);  
        CyDelay(1000);
        dato_enviado++;
    }
}

/* [] END OF FILE */
