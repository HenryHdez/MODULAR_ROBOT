#include <project.h>
#include <stdio.h>
//Import Libraries
//Global Variables
int16 S1=0,S2=0,S3=0,S4=0;
uint8 dato_enviado;
uint8 dato_recibido;
int32 Conversion=0;
float32 Tension=0.0;
char publicar[6];
char* formato=" %i";
uint8 mailbox0[8];

CY_ISR(interrutcan){
    CAN_MsgRXIsr();
    LED_1_Write(1);
    sprintf(publicar, formato, mailbox0[0]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox0[1]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox0[2]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox0[3]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox0[4]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox0[5]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox0[6]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox0[7]);
    XBEE_PutString(publicar); 
    XBEE_PutCRLF(0);
    LED_1_Write(0);
}

int main()
{
    CyGlobalIntEnable;    
    //Init LED's
    LED_1_Write(0);
    //Serial Start
    XBEE_Start();
    RESET_XBEE_Write(1);
    ADC_Start();
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    CAN_Start(); ///  start CAN
    CAN_isr_StartEx(interrutcan);//interrucion can 
    CAN_GlobalIntEnable();
    for(;;)
    {
        Conversion=ADC_GetResult16(0);
        Tension=ADC_CountsTo_Volts(0,Conversion);
        }
}

