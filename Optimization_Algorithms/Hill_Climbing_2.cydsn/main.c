#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <protocolo can.h>
//Import Libraries
//Global Variables
uint8 mailbox0[8];
uint8 mailbox1[8];
uint8 mailbox2[8];
uint8 dato_enviado[8];
uint8 dato_enviado_1[8];
uint8 dato_enviado_2[8];
int16 conta=0;
int cantidadM=0;
int Cantidad_Sensores=0;
//Variables AE
int Max_Conta=150;
float Amplitud_H=0.0;
float Amplitud_V=0.0;
float Offset_H=0.833;
float Offset_V=0.833;
float Fase=0.0;
float ampli_aux_V=0.0;
float ampli_aux_H=0.0;
float off_aux_V=0.833;
float off_aux_H=0.833;
float pha_aux=0.0;
float ampli_aux_V_2=0.0;
float off_aux_V_2=0.833;
float pha_aux_2=0.0;
float ampli_aux_H_2=0.0;
float off_aux_H_2=0.833;
float Peso_Sensor=0.0;
int flag=0;
float dist=0.0;
float L_actual=0.0;
float L_anterior=0.0;
int punto_a=0;
int punto_b=0;
int move_s=0;
int individuo=0;


double randd() {
  return (double)rand()/((double)RAND_MAX + 1);
}

double random_amplitud_1(){
    float r=0.0;
    r=randd();
    return r;
}

double random_amplitud_2(){
    float r=0.0;
    r=randd();
    r=r*0.2;
    return r;
}

double random_Fase(){
    float r=0.0;
    r=-0.1*randd();
    return r;
}

double random_Offset(){
    float r=0.0;
    r=randd();
    r=r*-0.01;
    r=r+0.02;
    return r;
}

void movimiento(unsigned int dirr,int mov){
    char byteh,bytel;
    byteh= mov >> 8;
    bytel=mov;
    dato_enviado[0]=VER ;//verificacion
    dato_enviado[1]=dirr ;//dir
    dato_enviado[2]=ESCRITURA ;//operacion
    dato_enviado[3]=MOTOR;//lugar
    dato_enviado[4]= byteh ;  //dato
    dato_enviado[5]= bytel ;  //dato
    CAN_SendMsg0(); 
    CyDelay(25);    
}

void Send_Angle(int datas,int Conta3){ 
    dato_enviado[0]=VER2;//verificacion
    dato_enviado[1]=Conta3;//dir
    dato_enviado[2]=Conta3;//dir
    dato_enviado[3]=ANGLE;
    dato_enviado[4]=0;//lugar
    dato_enviado[5]=0;  //dato... en este caso direcion para asignar
    dato_enviado[6]=datas;
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
                            Enviar_Parametros(Amplitud_H,Fase,Offset_H,H);
                            CyDelay(50);
                            Enviar_Parametros(Amplitud_V,Fase,Offset_V,V);
                            CyDelay(50); 
                            Send_Angle(0,0);
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
        if(mailbox2[1]==0&&mailbox2[2]==0&&mailbox2[3]==0)      {Peso_Sensor=0.0;}   //Normal
        else if(mailbox2[1]==0&&mailbox2[2]==0&&mailbox2[3]==1) {Peso_Sensor=0.2;}   //Izquierda
        else if(mailbox2[1]==0&&mailbox2[2]==1&&mailbox2[3]==0) {Peso_Sensor=0.6;}   //Frontal
        else if(mailbox2[1]==0&&mailbox2[2]==1&&mailbox2[3]==1) {Peso_Sensor=0.8;}   //Frontal e Izquierda
        else if(mailbox2[1]==1&&mailbox2[2]==0&&mailbox2[3]==0) {Peso_Sensor=0.2;}   //Derecha
        else if(mailbox2[1]==1&&mailbox2[2]==0&&mailbox2[3]==1) {Peso_Sensor=0.4;}   //Derecha e Izquierda
        else if(mailbox2[1]==1&&mailbox2[2]==1&&mailbox2[3]==0) {Peso_Sensor=0.8;}   //Frontal y Derecha
        else if(mailbox2[1]==1&&mailbox2[2]==1&&mailbox2[3]==1) {Peso_Sensor=1;}     //Todos
        else                                                    {Peso_Sensor=0.0;}   //Normal
    }
}

CY_ISR(interrutcan){
    CAN_GlobalIntDisable();
    CAN_MsgRXIsr();  
    Recibe_mail_0();
    Recibe_Mail_2();
    CAN_GlobalIntEnable();
}

void Hill(){
    if (flag==1){
        ampli_aux_V=Amplitud_V;
        ampli_aux_H=Amplitud_H;
        pha_aux=Fase;
        off_aux_V=Offset_V;
        off_aux_H=Offset_H;      
        Amplitud_V=0.0;
        Amplitud_H=0.0;
        Fase=0.0;
        Offset_V=0.833;
        Offset_H=0.833;
        Max_Conta=15;
    }
    else if (flag==2){   
        L_anterior=dist;
        if(randd()<0.5){
            Amplitud_H=random_amplitud_1();
            Amplitud_V=random_amplitud_2();
            Fase=random_Fase();
            Offset_H=0.833+random_Offset();
            Offset_V=Offset_H; 
        }
        else{
            Amplitud_V=random_amplitud_1();
            Amplitud_H=0.0;
            Fase=random_Fase();
            Offset_H=0.833;
            Offset_V=0.833+random_Offset();
        }  
        ampli_aux_H_2=Amplitud_H;
        off_aux_H_2=Offset_H;      
        ampli_aux_V_2=Amplitud_V;
        off_aux_V_2=Offset_V;
        pha_aux_2=Fase; 
        Max_Conta=50;
    }
    else if (flag==3){
        Amplitud_V=0.0;
        Amplitud_H=0.0;
        Fase=0.0;
        Offset_V=0.833;
        Offset_H=0.833;
        Max_Conta=15;
    }    
    else if(flag==4){
        L_actual=dist;
        dato_enviado_2[0]=individuo;
        dato_enviado_2[1]=(int)dist;
        CAN_SendMsg3();
        if(L_anterior<L_actual){
            Amplitud_V=ampli_aux_V;
            Amplitud_H=ampli_aux_H;
            Fase=pha_aux;
            Offset_V=off_aux_V;
            Offset_H=off_aux_H;              
        }
        else{
            Amplitud_V=ampli_aux_V_2;
            Amplitud_H=ampli_aux_H_2;
            Fase=pha_aux_2;
            Offset_V=off_aux_V_2;
            Offset_H=off_aux_H_2;       
        }
        if(individuo==0){individuo=1;}
        else{individuo=0;}
        flag=0;
        Max_Conta=70;
    }
    else if(flag>4){flag=0;Max_Conta=4;}  
}

//Interrupt's
CY_ISR(Temporizador){
    conta++;  
    if(conta==Max_Conta){     
        LED_1_Write(1);
        flag++;      
        Hill(); 
        for(int i=0;i<5;i++){
            Enviar_Parametros(Amplitud_H,Fase,Offset_H,H);
            CyDelay(50);
            Enviar_Parametros(Amplitud_V,Fase,Offset_V,V);
            CyDelay(50);
            Send_Angle(0,0);
        }        
        conta=0;
    }
    else{
        LED_1_Write(0);
    }  
}

CY_ISR(Serial_Interrupt){
    dist=SENSOR_U_GetChar()/255.0;
    dist=dist+Peso_Sensor;
    dist=dist*100;
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
    CyGlobalIntEnable;
    for(;;){     
    }
}
