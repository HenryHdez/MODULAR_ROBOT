#include <project.h>
uint8 dato_enviado;
uint8 dato_recibido;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    CAN_Start();
    dato_enviado=40;
    for(;;)
    {
        CAN_ReceiveMsg0();
        if(dato_recibido==1){
            CAN_SendMsg0();
            CyDelay(500);
            dato_enviado++;
            LED_1_Write(1);
        }
        else{
            LED_1_Write(0);
        }
    }
}
