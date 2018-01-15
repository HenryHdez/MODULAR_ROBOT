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
int Cantidad_Sensores=0;
//Variables AE
int Max_Conta=150;
float amplitud=0.0;
float offset=0.833;
float fase=0.0;
float ampli_aux=0.0;
float off_aux=0.833;
float pha_aux=0.0;
float ampli_aux_2=0.0;
float off_aux_2=0.833;
float pha_aux_2=0.0;
int flag=0;
float L_actual=0.0;
float L_anterior=0.0;
int punto_a=0;
int punto_b=0;
int move_s=0;

double randd() {
  return (double)rand()/((double)RAND_MAX + 1);
}

double random_amplitud(){
    return randd();
}

double random_Fase(){
    float r=0.0;
    r=randd();
    r=r*2;
    r=r-1;
    return r;
}

double random_Offset(){
    float r=0.0;
    r=randd();
    r=r*0.35;
    r=r+0.65;
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
                        Enviar_Parametros(amplitud,fase,offset,V);
                        CyDelay(10);
                        Enviar_Parametros(0.0,fase,0.833,H);  
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

CY_ISR(interrutcan){
    CAN_GlobalIntDisable();
    CAN_MsgRXIsr();  
    Recibe_mail_0();
    Recibe_Mail_2();
    CAN_GlobalIntEnable();
}

void Hill(){
    if (flag==1){
        ampli_aux=amplitud;
        off_aux=offset;
        pha_aux=fase;        
        amplitud=0.0;
        fase=0.0;
        offset=0.833;
        Max_Conta=20;
    }
    else if (flag==2){   
        L_anterior=dist;
        amplitud=random_amplitud();
        fase=random_Fase();
        offset=random_Offset();
        ampli_aux_2=amplitud;
        off_aux_2=offset;
        pha_aux_2=fase;         
        Max_Conta=100;
    }
    else if (flag==3){
        amplitud=0.0;
        fase=0.0;
        offset=0.833;
        Max_Conta=20;
    }    
    else if(flag==4){
        L_actual=dist;
        if(L_anterior<L_actual){
            amplitud=ampli_aux;
            fase=pha_aux; 
            offset=off_aux;
        }
        else{
            amplitud=ampli_aux_2;
            fase=pha_aux_2; 
            offset=off_aux_2;        
        }
        flag=0;
        Max_Conta=100;
    }
    else if(flag>4){flag=0;Max_Conta=5;}  
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
    for(i=0;i<6;i++){ //Alineación
        movimiento(i,512);
        }
    CyDelay(2000);
    movimiento(2,750);
    CyDelay(5000);
    move_s=1;
}

void Select_Move_2(){
    punto_a=dist;  
    move_s=2;
}

void Select_Move_3(){
    movimiento(2,274);
    CyDelay(5000);
    move_s=3;
}

void Select_Move_4(){
    punto_b=dist;
    move_s=0;       
    if(punto_a>=punto_b){
        Left_Move();    
    }
    else{
        Right_Move();
    } 
}

//Interrupt's
CY_ISR(Temporizador){
    conta++;
    if(conta==Max_Conta){     
        LED_1_Write(1);
        if(move_s==0){
            if(Cantidad_Sensores==2){
                Select_Move();   
                Max_Conta=10;
            }
            else if(Cantidad_Sensores==3){//Izquierda
                Left_Move();
                Max_Conta=10;
            }
            else if(Cantidad_Sensores==6){//Derecha
                Right_Move();    
                Max_Conta=10;
            }
            else if(Cantidad_Sensores==7){//Reversa
                amplitud=0.8;offset=0.833;fase=-0.5;
                Enviar_Parametros(amplitud,fase,offset,V); 
                CyDelay(1000);
                Enviar_Parametros(0.0,fase,offset,H);
                CyDelay(15000);
                Max_Conta=10;
            }
            else{   //Adelante
                flag++;      
                Hill();
                Enviar_Parametros(0.0,fase,0.833,H);
                CyDelay(1000);
                Enviar_Parametros(amplitud,fase,offset,V);             
            }
        }
        else if (move_s==1){Select_Move_2();}
        else if (move_s==2){Select_Move_3();}
        else{Select_Move_4();}
        conta=0;
    }
    else{
        LED_1_Write(0);
    }  
}

CY_ISR(Serial_Interrupt){
    dist=SENSOR_U_GetChar();
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
