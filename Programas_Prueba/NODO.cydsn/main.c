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
    SPIM_1_Start();
    SPIS_1_Start();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("Datos");
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        SPIM_1_WriteTxData(dato_TX);
        SPIS_1_WriteTxData(SPIS_1_ReadRxData());
        dato_RX=SPIM_1_ReadRxData();
        dato_TX++;
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintInt8(dato_RX);
        LCD_Char_1_Position(1,8);
        LCD_Char_1_PrintInt8(dato_TX);
        CyDelay(1000);
    }
}

/* [] END OF FILE */
