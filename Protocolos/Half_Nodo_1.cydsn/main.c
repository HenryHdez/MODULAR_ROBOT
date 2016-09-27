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
    EMITE_Start();
    RECIBE_Start();
    //EMITE_LoadTxConfig();
    //RECIBE_LoadRxConfig();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("Datos...");
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        EMITE_PutChar(dato_enviado);
        CyDelay(50);
        dato_recibido=RECIBE_GetChar();
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintInt8(dato_enviado);       
        LCD_Char_1_Position(1,5);
        LCD_Char_1_PrintInt8(dato_recibido); 
        dato_enviado++;
        CyDelay(500);
    }
}

/* [] END OF FILE */
