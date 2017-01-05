#include <project.h>
uint8 dato_enviado;
uint8 dato_recibido;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    CAN_Start();
    LED_1_Write(0);
    for(;;)
    {
        CAN_SendMsg0();
        CyDelay(500);
        dato_enviado++;
        CAN_ReceiveMsg0();
        if(dato_recibido==0){LED_1_Write(0);}
        else{LED_1_Write(1);}
    }
}

