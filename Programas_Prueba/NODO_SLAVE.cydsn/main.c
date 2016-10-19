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
int8 dato_TX,dato_RX;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    SPIS_1_Start();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("Datos");
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        dato_RX=SPIS_1_ReadRxData();
        SPIS_1_WriteTxData(dato_RX);
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintInt8(dato_RX);
        CyDelay(1000);
    }
}
