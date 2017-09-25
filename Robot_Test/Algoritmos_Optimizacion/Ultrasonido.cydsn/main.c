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
char dist=0;
int16 conta=0;
int cantidadM=0;
//Variables AE
int Max_Conta=5;
float Amplitud_H=0.0;
float Offset_H=0.833;
float Fase_H=0.0;
float Amplitud_V=0.0;
float Offset_V=0.833;
float Fase_V=0.0;
float ampli_aux=0.0;
float off_aux=0.0;
float pha_aux=0.0;
int flag=0;
float L_actual=0.0;
float L_anterior=0.0;
int Cantidad_Sensores=0;
int conta_H=0;

double randd() {
  return (double)rand()/((double)RAND_MAX + 1);
}

double random_amplitud(){
    return randd();
}

double random_Fase(){
    float r=0.0;
    r=randd();
    if(r>0.5){return randd();}
    else{return randd()*-1;}
}

double random_Offset(){
    float r=0.0;
    r=randd();
    r=r*0.5;
    r=r+0.3;
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
    int8 datoL=datas;
    int8 datoH=datas>>8;  
    dato_enviado[0]=VER2;//verificacion
    dato_enviado[1]=Conta3;//dir
    dato_enviado[2]=Conta3;//dir
    dato_enviado[3]=ANGLE;
    dato_enviado[4]=0;//lugar
    dato_enviado[5]=0;  //dato... en este caso direcion para asignar
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



double Medir_dist(){
    int i=0;
    float longitud=0.0;
    for(i=1;i<10;i++){
        longitud=dist+longitud;
        CyDelay(50);
    }
    longitud=longitud/10;
    return longitud;
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
                        //Enviar_Parametros(amplitud,fase,offset,V);
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
        if(mailbox2[1]==0&&mailbox2[2]==0&&mailbox2[3]==0)      {Cantidad_Sensores=0;}   //Normal
        else if(mailbox2[1]==0&&mailbox2[2]==0&&mailbox2[3]==1) {Cantidad_Sensores=1;}   //Izquierda
        else if(mailbox2[1]==0&&mailbox2[2]==1&&mailbox2[3]==0) {Cantidad_Sensores=2;}   //Frontal
        else if(mailbox2[1]==0&&mailbox2[2]==1&&mailbox2[3]==1) {Cantidad_Sensores=3;}   //Frontal e Izquierda
        else if(mailbox2[1]==1&&mailbox2[2]==0&&mailbox2[3]==0) {Cantidad_Sensores=4;}   //Derecha
        else if(mailbox2[1]==1&&mailbox2[2]==0&&mailbox2[3]==1) {Cantidad_Sensores=5;}   //Derecha e Izquierda
        else if(mailbox2[1]==1&&mailbox2[2]==1&&mailbox2[3]==0) {Cantidad_Sensores=6;}   //Frontal y Derecha
        else if(mailbox2[1]==1&&mailbox2[2]==1&&mailbox2[3]==1) {Cantidad_Sensores=7;}   //Todos
        else                                                    {Cantidad_Sensores=0;}   //Normal
    }
}

void Left_Move(){
    Send_Angle(0,200);
    int  i=0;
    int j=0;
    for(i=0;i<cantidadM;i++){ //Alineación
        movimiento(i,512);
        }
    CyDelay(2000);
    movimiento(cantidadM,675);//Ancla Trasera
    CyDelay(2000);
    //Giro
    for(j=0;j<1;j++){
        for(i=2;i<cantidadM;i=i+4){ 
            movimiento(i,650);
            movimiento(i+1,750);
            }
        movimiento(cantidadM,512);//Ancla Trasera
        CyDelay(2000);
        movimiento(0,350);//Ancla Delantera
        CyDelay(2000);
        for(i=2;i<cantidadM;i=i+4){ //Alineación
            movimiento(i,512);
            movimiento(i+1,512);
        }    
        CyDelay(2000);
    }         
    for(i=0;i<cantidadM;i++){ //Alineación
        movimiento(i,512);
        }
    CyDelay(2000); 
    Send_Angle(0,0);
}

void Right_Move(){
    Send_Angle(0,200);
    int  i=0;
    int j=0;
    for(i=0;i<cantidadM;i++){ //Alineación
        movimiento(i,512);
        }
    CyDelay(2000);
    movimiento(cantidadM,675);//Ancla Trasera
    CyDelay(2000);
    //Giro
    for(j=0;j<1;j++){
        for(i=2;i<cantidadM;i=i+4){ 
            movimiento(i,374);
            movimiento(i+1,274);
            }
        movimiento(cantidadM,512);//Ancla Trasera
        CyDelay(2000);
        movimiento(0,350);//Ancla Delantera
        CyDelay(2000);
        for(i=2;i<cantidadM;i=i+4){ //Alineación
            movimiento(i,512);
            movimiento(i+1,512);
        }    
        CyDelay(2000);
    }         
    for(i=0;i<cantidadM;i++){ //Alineación
        movimiento(i,512);
        }
    CyDelay(2000); 
    Send_Angle(0,0);
}

void Select_Move(){
    Send_Angle(0,200);
    int  i=0;
    for(i=0;i<cantidadM;i++){ //Alineación
        movimiento(i,512);
        }
    CyDelay(2000);
    movimiento(2,650);
    CyDelay(4000);
    double a=Medir_dist();
    movimiento(2,374);
    double b=Medir_dist();
    CyDelay(4000);
    if(a<b){
        Left_Move();    
    }
    else{
        Right_Move();
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
        if(Cantidad_Sensores==2&&dist<20){
            Select_Move();      
        }
        else if(Cantidad_Sensores==3){//Izquierda
            Left_Move();
        }
        else if(Cantidad_Sensores==6){//Derecha
            Right_Move();           
        }
        else if(Cantidad_Sensores==7){//Reversa
            Amplitud_V=0.8;Offset_V=0.833;Fase_V=0.5;
            Enviar_Parametros(Amplitud_V,Fase_V,Offset_V,V); 
            CyDelay(1000);
            Enviar_Parametros(0.0,Fase_V,Offset_V,H);
            CyDelay(15000);
        }
        else{   //Adelante
            Amplitud_V=0.8;Offset_V=0.833;Fase_V=-0.5;
            Enviar_Parametros(Amplitud_V,Fase_V,Offset_V,V);    
            CyDelay(1000);
            Enviar_Parametros(0.0,Fase_V,Offset_V,H);
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
