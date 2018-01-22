//Import Libraries
#include <project.h>
#include <stdio.h>
#include <math.h>
#include <protocolo can.h>

//Global Variables
uint8 dato_recibido;
char publicar[6];
char* formato=" %i";
uint8 mailbox0[8];
uint8 mailbox1[8];
uint8 mailbox2[8];
uint8 mailbox3[8];
uint8 dato_enviado[8];
uint8 dato_enviado_1[8];
int memoria=1;
float x1=0.0;


CY_ISR(interrutcan){
    LED_1_Write(1);
    CAN_GlobalIntDisable();
    CAN_MsgRXIsr();   
    CAN_GlobalIntEnable();
    LED_1_Write(0);
}


int main()
{
    //Init LED's
    LED_1_Write(0);
    ADC_Start();
    ADC_StartConvert();
    CAN_Start();                    //Start CAN
    //CAN_isr_StartEx(interrutcan);   //Interrupcion CAN 
    CAN_GlobalIntEnable();
    CyGlobalIntEnable;
    for(;;){  
        LED_1_Write(0);
        dato_enviado_1[0]=ADC_GetResult16(0);
        dato_enviado_1[1]=ADC_GetResult16(1);
        dato_enviado_1[2]=ADC_GetResult16(2);
        CAN_SendMsg4();
        CyDelay(1500);
        LED_1_Write(1);
        CyDelay(1500);
        }
}

