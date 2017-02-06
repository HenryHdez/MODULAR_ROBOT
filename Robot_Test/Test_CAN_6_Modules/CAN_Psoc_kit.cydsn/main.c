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
uint8 dato_recibido_1;
uint8 dato_recibido_2;
uint8 dato_recibido_3;
uint8 dato_recibido_4;
uint8 dato_recibido_5;
uint8 dato_recibido_6;
uint8 dato_recibido_7;
uint8 dato_enviado;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    RX_CAN_Start();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("R1_3: ");
    LCD_Char_1_Position(1,0);
    LCD_Char_1_PrintString("R4_6: ");    
    dato_enviado=0;
    for(;;)
    {
        RX_CAN_ReceiveMsg0();
        RX_CAN_ReceiveMsg1();
        RX_CAN_ReceiveMsg2();
        RX_CAN_ReceiveMsg3();
        RX_CAN_ReceiveMsg4();
        RX_CAN_ReceiveMsg5();
        CyDelay(10);
        RX_CAN_SendMsg0();
        RX_CAN_SendMsg1();
        RX_CAN_SendMsg2();
        RX_CAN_SendMsg3();
        RX_CAN_SendMsg4();
        RX_CAN_SendMsg5();
        if(dato_enviado==1){
            dato_enviado=0;
        }
        else{
            dato_enviado=1;
        }
        LCD_Char_1_Position(0,6);
        LCD_Char_1_PrintHexUint8(dato_recibido_1);    
        LCD_Char_1_Position(0,9);
        LCD_Char_1_PrintHexUint8(dato_recibido_2); 
        LCD_Char_1_Position(0,12);
        LCD_Char_1_PrintHexUint8(dato_recibido_3);
        LCD_Char_1_Position(1,6);
        LCD_Char_1_PrintHexUint8(dato_recibido_4);
        LCD_Char_1_Position(1,9);
        LCD_Char_1_PrintHexUint8(dato_recibido_5);
        LCD_Char_1_Position(1,12);
        LCD_Char_1_PrintHexUint8(dato_recibido_6);
        }
}

/* [] END OF FILE */
