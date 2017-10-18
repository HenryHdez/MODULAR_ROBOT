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
uint8 dato_enviado[8];
uint8 dato_enviado_1[8];

void Oyente_Mail_1(){
    XBEE_PutString("MB_0= ");
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
}

void Oyente_Mail_2(){
    XBEE_PutString("MB_1= ");
    sprintf(publicar, formato, mailbox1[0]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox1[1]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox1[2]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox1[3]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox1[4]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox1[5]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox1[6]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox1[7]);
    XBEE_PutString(publicar); 
    XBEE_PutCRLF(0);     
}

void Oyente_Mail_3(){
    XBEE_PutString("MB_3= ");
    sprintf(publicar, formato, mailbox2[0]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox2[1]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox2[2]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox2[3]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox2[4]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox2[5]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox2[6]);
    XBEE_PutString(publicar);    
    sprintf(publicar, formato, mailbox2[7]);
    XBEE_PutString(publicar); 
    XBEE_PutCRLF(0);     
}

CY_ISR(interrutcan){
    LED_1_Write(1);
    CAN_GlobalIntDisable();
    CAN_MsgRXIsr();   
    Oyente_Mail_1();
    Oyente_Mail_2(); 
    Oyente_Mail_3();
    CAN_GlobalIntEnable();
    LED_1_Write(0);
}


int main()
{
    //Init LED's
    LED_1_Write(0);
    XBEE_Start();    
    RESET_XBEE_Write(1);
    CAN_Start();                    //Start CAN
    CAN_isr_StartEx(interrutcan);   //Interrupcion CAN 
    CAN_GlobalIntEnable();
    CyGlobalIntEnable;
    for(;;){
        }
}

