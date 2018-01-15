//Import Libraries
#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <protocolo can.h>
//Global Variables
uint8 mailbox0[8];
uint8 mailbox1[8];
uint8 mailbox2[8];
uint8 dato_enviado[8];
uint8 dato_enviado_1[8];
char dist=0;
int16 conta=0;
int cantidadM=0;
//Variables AE
int16 Max_Conta=100;
float Amplitud_H=0.0;
float Amplitud_V=0.0;
float Offset=0.833;
float Fase=0.0;
float peso_Amp_H=0.0;
float peso_Fase_H=0.0;
float peso_Off_H=0.0;
float peso_Amp_V=0.0;
float peso_Fase_V=0.0;
float peso_Off_V=0.0;
int flag=0;
float L_actual=0.0;
float L_anterior=0.0;
int conta_H=0;
int punto_a=0;
int punto_b=0;
int move_s=0;

//Generating Function of Random Numbers
double randd() {
  return (double)rand()/((double)RAND_MAX + 1);
}

//Amplitud Generator
double random_amplitud(){
    return randd();
}

//Phase Generator
double random_Fase(){
    float r=0.0;
    r=randd();
    if(r>0.5){return randd();}
    else{return randd()*-1;}
}

//Offset Genrator
double random_Offset(){
    float r=0.0;
    r=randd();
    r=r*0.5;
    r=r+0.3;
    return r;
}

//Send Theta using CAN
void Send_Angle(int datas,int Conta3){ 
    int8 datoL=datas;
    int8 datoH=datas>>8;  
    dato_enviado[0]=VER2;       //Check Bit
    dato_enviado[1]=Conta3;     //Address Previous Module
    dato_enviado[2]=Conta3;     //Address Module Next
    dato_enviado[3]=ANGLE;
    dato_enviado[4]=0;          
    dato_enviado[5]=0;          //who send
    dato_enviado[6]=0;
    dato_enviado[7]=Conta3;
    CAN_SendMsg0(); 
    CyDelay(50);
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

//Interrupt's
void Recibe_mail_0(){
if(mailbox0[0]==VER){
            switch(mailbox0[2]){
            ///////////////////////////////////////////////////////
            case ESCRITURA:{ 
                switch(mailbox0[3]){//////////+6+++++++++++++
                    case CANTIDAD_MOD: 
                            cantidadM=mailbox0[4];                           
                    break;
                    case START:
                        
                    break;
                }
            }                                            
        }    
}
if(mailbox0[0]==VER2){
                switch(mailbox0[3]){
                ///////////////////////////////////////////////////////
                case ANGLE:{
                    int Contador=mailbox0[7];
                    if(Contador>=cantidadM){
                    } 
                }
                break;}
            } 
}

void Recibe_Mail_2(){
    if(mailbox2[0]==SENSORES){
        //Mailbox2[1] Derecha
        //Mailbox2[2] Frente
        //Mailbox2[3] Izquierda
        if(mailbox2[1]==0&&mailbox2[2]==0&&mailbox2[3]==0)      {peso_Amp_V=1;peso_Fase_V=-0.5;peso_Off_V=0.0;peso_Amp_H=0.0;peso_Fase_H=-0.5;peso_Off_H=0.0;}       //Normal
        else if(mailbox2[1]==0&&mailbox2[2]==0&&mailbox2[3]==1) {peso_Amp_V=1;peso_Fase_V=-0.5;peso_Off_V=0.0;peso_Amp_H=0.0;peso_Fase_H=-0.5;peso_Off_H=0.0;}       //Izquierda
        else if(mailbox2[1]==0&&mailbox2[2]==1&&mailbox2[3]==0) {peso_Amp_V=0.3;peso_Fase_V=0.5;peso_Off_V=0.0;peso_Amp_H=0.8;peso_Fase_H=-0.5;peso_Off_H=0.2;}      //Frontal
        else if(mailbox2[1]==0&&mailbox2[2]==1&&mailbox2[3]==1) {peso_Amp_V=0.3;peso_Fase_V=-0.5;peso_Off_V=0.0;peso_Amp_H=0.8;peso_Fase_H=-0.5;peso_Off_H=0.2;}     //Frontal e Izquierda
        else if(mailbox2[1]==1&&mailbox2[2]==0&&mailbox2[3]==0) {peso_Amp_V=1;peso_Fase_V=-0.5;peso_Off_V=0.0;peso_Amp_H=0.0;peso_Fase_H=-0.5;peso_Off_H=0.0;}       //Derecha
        else if(mailbox2[1]==1&&mailbox2[2]==0&&mailbox2[3]==1) {peso_Amp_V=1;peso_Fase_V=-0.5;peso_Off_V=0.0;peso_Amp_H=0.0;peso_Fase_H=-0.5;peso_Off_H=0.0;}       //Derecha e Izquierda
        else if(mailbox2[1]==1&&mailbox2[2]==1&&mailbox2[3]==0) {peso_Amp_V=0.3;peso_Fase_V=-0.5;peso_Off_V=0.0;peso_Amp_H=0.8;peso_Fase_H=-0.5;peso_Off_H=-0.2;}    //Frontal y Derecha
        else if(mailbox2[1]==1&&mailbox2[2]==1&&mailbox2[3]==1) {peso_Amp_V=1;peso_Fase_V=0.5;peso_Off_V=0.0;peso_Amp_H=0.0;peso_Fase_H=-0.5;peso_Off_H=0.0;}        //Todos
        else                                                    {peso_Amp_V=1;peso_Fase_V=-0.5;peso_Off_V=0.0;peso_Amp_H=0.0;peso_Fase_H=-0.5;peso_Off_H=0.0;}       //Normal
    }
}

CY_ISR(Serial_Interrupt){
   dist=SENSOR_U_GetChar();
}

CY_ISR(interrutcan){
    CAN_GlobalIntDisable();
    CAN_MsgRXIsr();  
    Recibe_mail_0();
    Recibe_Mail_2();
    CAN_GlobalIntEnable();
}

CY_ISR(Temporizador){
    conta++;
    if(conta==Max_Conta){    
        LED_1_Write(1);
        if(flag==0){//Verificar Sensores
            for(int i=0;i<10;i++){
                Enviar_Parametros(0.0,Fase,Offset,H);
                Enviar_Parametros(0.0,Fase,Offset,V);
                CyDelay(20);
            }
            Max_Conta=10;
            flag=1;
        }
        else{
            for(int i=0;i<10;i++){
                Enviar_Parametros(Amplitud_H+peso_Amp_H,Fase+peso_Fase_H,Offset+peso_Off_H,H);
                Enviar_Parametros(Amplitud_V+peso_Amp_V,Fase+peso_Fase_V,Offset+peso_Off_V,V);
                CyDelay(20);
            }
            Max_Conta=50;
            flag=0;
        }
        conta=0;
    }
    else{
        LED_1_Write(0);
    }  
}

int main()
{
    //Init LED's
    CAN_Start(); ///  start CAN
    Timer_1_Start();
    Timer_Start();
    SENSOR_U_Start();
    CAN_isr_StartEx(interrutcan);
    CAN_GlobalIntEnable();
    Int_Serial_StartEx(Serial_Interrupt);
    Timer_StartEx(Temporizador);
    LED_1_Write(0);
    CyGlobalIntEnable;
    for(;;){     
    }
}
