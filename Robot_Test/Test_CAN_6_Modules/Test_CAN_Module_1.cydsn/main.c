#include <project.h>
uint8 dato_enviado;
uint8 dato_recibido;

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    CAN_Start();
    for(;;)
    {
            CAN_SendMsg0();
            CyDelay(500);
            dato_enviado++;

        
    }
}

