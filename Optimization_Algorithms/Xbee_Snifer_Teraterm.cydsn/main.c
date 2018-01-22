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
uint8 mailbox4[8];
uint8 dato_enviado[8];
uint8 dato_enviado_1[8];
int memoria=1;
int conta=0;
float Amplitud_2=0.0;
float Fase_2=0.0;
float Offset_2=0.0;
int Orientacion_2=V;

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

void Oyente_Mail_4(){
    sprintf(publicar, formato, mailbox3[0]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox3[1]);
    XBEE_PutString(publicar);
    XBEE_PutCRLF(0); 
}

void Oyente_Mail_5(){
    sprintf(publicar, formato, mailbox4[0]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox4[1]);
    XBEE_PutString(publicar);
    sprintf(publicar, formato, mailbox4[2]);
    XBEE_PutString(publicar);
    XBEE_PutCRLF(0); 
}

CY_ISR(interrutcan){
    LED_1_Write(1);
    CyGlobalIntDisable;
    CAN_GlobalIntDisable();
    CAN_MsgRXIsr();   
   // Oyente_Mail_1();
   // Oyente_Mail_2(); 
    Oyente_Mail_3();
   // Oyente_Mail_4();
    Oyente_Mail_5();
    CAN_GlobalIntEnable();
    CyGlobalIntEnable;
    LED_1_Write(0);
}

void Enviar_Parametros(float Amplitud,float Phase,float Offset,int Orientacion){
    //Convertir el Valor a entero para ser enviado
    int Amp_env=(int)fabs(Amplitud*10000);
    int Phase_env=(int)fabs(Phase*10000);
    int Offset_env=(int)fabs(Offset*10000);
    //Enviar datos
    dato_enviado_1[0]=VER_M1;
    dato_enviado_1[1]=Orientacion;
    //Convertir Amplitud
    uint8 datoL=Amp_env;
    uint8 datoH=Amp_env>>8;
    uint8 Aux=0;
    if(Amplitud<0.0){
        Aux=(1<<7);
        datoH=datoH+Aux;
    }
    else{
        Aux=(0<<7);
        datoH=datoH+Aux;    
    }
    dato_enviado_1[2]=datoL;
    dato_enviado_1[3]=datoH;
    //Convertir Fase
    datoL=Phase_env;
    datoH=Phase_env>>8;
    if(Phase<0.0){
        Aux=(1<<7);
        datoH=datoH+Aux;
    }
    else{
        Aux=(0<<7);
        datoH=datoH+Aux;    
    }
    dato_enviado_1[4]=datoL;
    dato_enviado_1[5]=datoH;
    //Convertir Offset
    datoL=Offset_env;
    datoH=Offset_env>>8;
    if(Offset<0.0){
        Aux=(1<<7);
        datoH=datoH+Aux;
    }
    else{
        Aux=(0<<7);
        datoH=datoH+Aux;    
    }
    dato_enviado_1[6]=datoL;
    dato_enviado_1[7]=datoH;    
    CAN_SendMsg1();
    CyDelay(25);
}

float Saturador(float numero_comprobar,float lim_inf, float lim_sup){
    if(numero_comprobar>lim_sup){return lim_sup;}
    else if (numero_comprobar<lim_inf){return lim_inf;}
    else{return numero_comprobar;}
}

CY_ISR(Interrupcion_F){
    CyGlobalIntDisable;
    if(conta==0){
        Amplitud_2=XBEE_GetByte()/255.0;
        conta++;}
    else if(conta==1){
        Offset_2=XBEE_GetByte()/255.0;
        conta++;}
    else if(conta==2){
        Fase_2=XBEE_GetByte()/255.0;
        conta++;}
    else{
        Orientacion_2=XBEE_GetByte();
        conta=0;
        if(Orientacion_2!=H||Orientacion_2!=V){Orientacion_2=V;}
    }
    Amplitud_2=Saturador(Amplitud_2,0.0,0.2); 
    Offset_2=Saturador(Offset_2,0.0,0.2); 
    Fase_2=Saturador(Fase_2,0.0,0.2); 
    Enviar_Parametros(Amplitud_2,Fase_2,Offset_2,Orientacion_2);
    CyGlobalIntEnable;
}

int main()
{
    //Init LED's
    LED_1_Write(0);
    XBEE_Start();    
    RESET_XBEE_Write(1);
    CAN_Start();                            //Start CAN
    CAN_isr_StartEx(interrutcan);           //Interrupcion CAN 
    //Interrupcion_StartEx(Interrupcion_F);   //Sino desea comunicar con el PC Comentaree esta linea
    CAN_GlobalIntEnable();
    CyGlobalIntEnable;
    for(;;){       
        }
}

