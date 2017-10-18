#include <VCNL4010_1.h>
#include <VCNL4010_2.h>
#include <VCNL4010_3.h>
#include <VCNL4010_4.h>
#include <AX12.h>
#include <math.h>
#include <stdlib.h>
#include <protocolo can.h>

float pi=M_PI;
float teta = 0.0;
float teta2 = 0.0;
float teta3 = 0.0;
float phase =0.0;
float ampli = 0.0;
float amp2 = 0.0;
float dampli = 0;
float of2 = (0.8333)*M_PI;
float offset = (0.8333)*M_PI;
float doffset = 0.0;
float dt = 0.3;
float sal = 0.0;
float vel = 0.0;
float theta,theta_velocidad,x,y;
int f,v,almacena_salida;
int Seleccion=0;
int Condicion_Start=0;
int  ant,t,Pd,Pt,cstatus,M,Pta,errc,PositionM,Teta_Can_l,Teta_Can_r,cantidadM,Contador,Orientacion_H,Orientacion_V; 
unsigned char dir=1; //////dir motor
unsigned int Pda;
uint8 Mydir=250;
uint8 dirasig ;
uint8 sumadir;
uint8 dato_enviado[8];
uint8 dato_enviado_2[8];
uint8 mailbox0[8];
uint8 mailbox1[8];
uint8 mailbox2[8];
float srf;
unsigned char dir,tem,tes,error,Error_Byte,Incoming_Byte2;
int amb,posicion,posicionh,posicionl;
int S1=0;

double randd() {
  return (double)rand()/((double)RAND_MAX + 1);
}

float dteta(float teta, float teta2, float teta3, float phase){
    float wij = 0.8; //0.8 Se mueve
    float v = 0.4;//0.4 Se mueve
    switch (Seleccion){
        case 1:
            return (2*pi*v)+(wij*sin(teta2-teta-phase));
        break;
        case 2:
            return (2*pi*v)+(wij*sin(teta3-teta+phase));
        break;
        case 3:
            return (2*pi*v)+(wij*sin(teta2-teta-phase))+(wij*sin(teta3-teta+phase));
        break;    
        default:
            return 2*pi*v;
        break;
            }
}

float ddampli(float dampli, float ampli, float amp){
  float ar = 30;
  return ar*((ar/4)*(amp-ampli)-dampli);
}

float ddoffset(float doffset, float offset, float of){
 float ax = 30;
return ax*((ax/4)*(of-offset)-doffset); 
}

void CPG(){
    //Calculate CPG 
    teta2=(Teta_Can_l*0.001);
    teta3=(Teta_Can_r*0.001);
    teta = teta + dteta(teta,teta2,teta3,phase)*dt;
    ampli = ampli + dampli*dt;
    dampli = dampli + ddampli(dampli,ampli,amp2)*dt;
    offset = offset + doffset*dt;
    doffset = doffset + ddoffset(doffset,offset,of2)*dt;    
    sal = offset + ampli*cos(teta);
    x=(sal*1023)/(1.66666*M_PI); // covercion a bit
    f=(int)x;                    //funcion píso
    MoveSpeed(1,f,100);
}

/////////////////////////////////////codigo para identificar//////////////////////////
void  cantidad_m(){
    dato_enviado[0]=VER ;//verificacion
    dato_enviado[1]=DIRM;//dir
    dato_enviado[2]=ESCRITURA ;//operacion
    dato_enviado[3]=CANTIDAD_MOD;//lugar
    dato_enviado[4]=dirasig;  //dato... en este caso direcion para asignar
    CAN_SendMsg0(); 
    CyDelay(25);
}

void asignacion(){
    dato_enviado[0]=VER ;//verificacion
    dato_enviado[1]=DIRCS ;//dir
    dato_enviado[2]=ESCRITURA ;//operacion
    dato_enviado[3]=MEC;//lugar
    dato_enviado[4]=dirasig ;  //dato... en este caso direcion para asignar
    CAN_SendMsg0(); 
    CyDelay(25);
}

void ledoff(int datas,int direccion,int led){
    dato_enviado[0]=VER ;//verificacion
    dato_enviado[1]=direccion ;//dir
    dato_enviado[2]=ESCRITURA ;//operacion
    dato_enviado[3]=led;//lugar
    dato_enviado[4]= datas ;  //dato... en este caso direcion para asignar
    CAN_SendMsg0(); 
    CyDelay(25);
}

void Send_Angle(int datas,int Conta){ 
    int8 datoL=datas;
    int8 datoH=datas>>8;  
    dato_enviado[0]=VER2;//verificacion
    dato_enviado[1]=Mydir+1;//dir
    if(Mydir==cantidadM){dato_enviado[1]=0;}
    dato_enviado[2]=Mydir-1;//dir
    dato_enviado[3]=ANGLE;
    dato_enviado[4]=datoL;//dato
    dato_enviado[5]=datoH;  //dato... en este caso direcion para asignar
    dato_enviado[6]=Mydir;
    dato_enviado[7]=Conta;
    CAN_SendMsg0();
    int retardo=0;
    retardo=(int)randd()*100;
    CyDelay(retardo);
}

void inicio(){
    ini:
    Pd=readProximity_2();
    if(Mydir!=DIRCS){
       goto end;
    }else{
    errc=CAN_GetErrorState();  ///////////status can
        if(errc==0){  
            if(Pd>=3000){
                Pda=readAmbient_2(); 
                LED_3_Write(1); 
                Pda=readAmbient_2(); 
                CyDelay(4);  
                LED_3_Write(0);
                Pda=readAmbient_2(); 
                CyDelay(4); 
                Pda=readAmbient_2(); 
                goto ini;     
        }else{
            Mydir=0;
            LED_2_Write(1);  ///identificador
            LED_4_Write(1);///led trasero 
            asig:
            CyDelay(2000);///t para lectura amb         ///
            dirasig=sumadir+1;
            sumadir=0;
            asignacion();/////envio1
            while(sumadir!=dirasig){
                //// espera de verificacion de llegada
                CyDelay(75);       
                if(t==20){ 
                    LED_4_Write(0);
                    dirasig= dirasig-1;
                    ledoff(0,dirasig,LEDF);
                    ant=dirasig; ///valor real de numero de modulos
                    ant=dirasig-1;
                    CyDelay(5);
                    ledoff(0,ant,LEDF);
                    cantidad_m();
                    cantidadM=dirasig;
                    Condicion_Start=1;
                    goto end;
                }else t++;
            }t=0;  
            ledoff(1,sumadir,LEDF);
            if(sumadir>1){
                ant=dirasig-1;
                ledoff(0,ant,LEDF);
            }else LED_4_Write(0);      
            goto asig;
            }      
        } else goto ini;
    }        
    end:;           
}

void recibmail(){
    if((mailbox0[0]==VER)&(mailbox0[1]==Mydir)){
        switch(mailbox0[2]){
            ///////////////////////////////////////////////////////
            case LECM:{ 
                switch(mailbox0[3]){//////////+6+++++++++++++
                    case MOTOR:
                    break;
                    case SENSOR1:
                    switch(mailbox0[4]){
                        case SAMB:
                        //code
                        break;
                        case SPROX: 
                        //code 
                        break; 
                    }break;
                    case SENSOR2: 
                    break;
                    case SENSOR3:  
                    break;
                    case SENSOR4: 
                    break; 
                }break;  
                }break;   
            case ESCRITURA:{
                switch(mailbox0[3]){////+++++++++++++++++++++++++++++++++++
                    case MEC:{
                        CyDelay(100);
                        LED_1_Write(1);
                        CyDelay(100);
                        LED_1_Write(0);
                        if(Pda>=1700){
                            Mydir=mailbox0[4];
                            dato_enviado[0]=VER;
                            dato_enviado[1]=0;//Direccion del Maestro
                            dato_enviado[2]=OK;
                            dato_enviado[3]=Mydir;
                            dato_enviado[4]=0;
                            dato_enviado[5]=0;
                            CAN_SendMsg0();
                            LED_1_Write(1); 
                            }
                        else;}break;
                    case MOTOR:   
                    PositionM = mailbox0[4] << 8;  
                    PositionM = PositionM+mailbox0[5];
                    MoveSpeed(dir,PositionM,100);
                    break;
                    case LED1:
                    switch(mailbox0[4]){
                        case ONM:  LED_1_Write(1);  break;
                        case OFFM: LED_1_Write(0);  break;
                        }break;
                    case LED2:
                    switch(mailbox0[4]){
                        case ONM:  LED_2_Write(1);  break;
                        case OFFM: LED_2_Write(0);  break;
                        }break;
                    case LED3:
                    switch(mailbox0[4]){
                        case ONM:  LED_3_Write(1);  break;
                        case OFFM: LED_3_Write(0);  break;
                        }
                    break;
                    case LED4:
                    switch(mailbox0[4]){
                        case ONM:  LED_4_Write(1);  break;
                        case OFFM: LED_4_Write(0);  break;
                        }
                    break;
                    case ANGLE:{
                        if(mailbox0[4]<Mydir){
                            Teta_Can_r = mailbox0[6]<<8;    
                            Teta_Can_r = Teta_Can_r+mailbox0[5]; 
                        }
                        else{
                            Teta_Can_l = mailbox0[6]<<8;    
                            Teta_Can_l = Teta_Can_l+mailbox0[5];                                                     
                        }
                        }break;
                }//////++++++++++++++++++++++++++++++++
                }break;
            case OK:{sumadir=mailbox0[3]; }break;
            /////////////////////////////////////////////////////
            }
        //////
        }
    else{          
        if((mailbox0[0]==VER)&(mailbox0[1]==DIRM)){
            switch(mailbox0[2]){
            ///////////////////////////////////////////////////////
            case ESCRITURA:{ 
                switch(mailbox0[3]){//////////+6+++++++++++++
                    case CANTIDAD_MOD: 
                            cantidadM=mailbox0[4];
                            Condicion_Start=1;
                    break;
                    case START:
                        
                    break;
                }
            }                                            
        }
    }
        else{
            if((mailbox0[0]==VER2)&&((mailbox0[1]==Mydir)||(mailbox0[2]==Mydir))){
                switch(mailbox0[3]){
                ///////////////////////////////////////////////////////
                case ANGLE:{
                    if(S1==1){goto salto2;}
                    else{
                    CyDelay(100+Mydir);                       
                    Contador=mailbox0[7];
                        if(mailbox0[6]<Mydir){
                            Teta_Can_r = mailbox0[5]<<8;    
                            Teta_Can_r = Teta_Can_r+mailbox0[4]; 
                        }
                        else{
                            Teta_Can_l = mailbox0[5]<<8;    
                            Teta_Can_l = Teta_Can_l+mailbox0[4];                                                     
                        }      
                        if(Contador==Mydir){ 
                            if(Contador>=cantidadM){Contador=0;}
                            else{Contador=Mydir+1;}
                            Send_Angle((int)sal*1000,Contador);
                            CPG();
                        }}
                }salto2:
                break;
                
            }            
        } 
        }
    }
}

void Condicion (){
    while(Condicion_Start==0){
        LED_1_Write(0);
        CyDelay(50);
        LED_1_Write(1);
        CyDelay(50);}
    LED_1_Write(0);
    if(Mydir==0&&cantidadM!=0){Seleccion=1;}
    else if(Mydir==cantidadM&&cantidadM!=0){Seleccion=2;}
    else if(Mydir==0&&cantidadM==0){Seleccion=4;}
    else{Seleccion=3;}
}

void Orientacion_Modulo(){
    int pos=readProximity_3();
    if(pos<10000){
        Orientacion_H=0;
    }
    else{
        Orientacion_H=1;   
    }
}

void Adquirir_Parametros(){
                        int dato_L = mailbox1[2];
                        int dato_H = mailbox1[3];
                        int signo_can=0;
                        signo_can=dato_H>>7;
                        if(signo_can==1){
                            dato_H=dato_H-128;
                            dato_H=dato_H<<8;
                            dato_H=dato_H+dato_L;
                            dato_H=dato_H*-1;
                        }
                        else{
                            dato_H=dato_H<<8;
                            dato_H=dato_H+dato_L;                       
                        }
                        ampli=(float)((dato_H)*0.0001);
                        amp2=ampli;
                        dato_L = mailbox1[4];
                        dato_H = mailbox1[5];
                        signo_can=0;
                        signo_can=dato_H>>7;
                        if(signo_can==1){
                            dato_H=dato_H-128;
                            dato_H=dato_H<<8;
                            dato_H=dato_H+dato_L;
                            dato_H=dato_H*-1;
                        }
                        else{
                            dato_H=dato_H<<8;
                            dato_H=dato_H+dato_L;                       
                        }
                        phase=(float)((dato_H)*0.0001); 
                        phase=phase*M_PI;
                        dato_L = mailbox1[6];
                        dato_H = mailbox1[7];
                        signo_can=0;
                        signo_can=dato_H>>7;
                        if(signo_can==1){
                            dato_H=dato_H-128;
                            dato_H=dato_H<<8;
                            dato_H=dato_H+dato_L;
                            dato_H=dato_H*-1;
                        }
                        else{
                            dato_H=dato_H<<8;
                            dato_H=dato_H+dato_L;                       
                        }
                        offset=(float)((dato_H)*0.0001);
                        offset=M_PI*offset;
                        of2=offset;
}

void recibmail_1(){
    if(mailbox1[0]==VER_M1){
        switch(mailbox1[1]){
            case H:{
                if(Orientacion_H==1){
                    Adquirir_Parametros();
                }
            }break;        
            case V:{
                if(Orientacion_H==0){
                    Adquirir_Parametros();
                }
            }break;
            }
        mailbox1[0]=0;
    }    
}

void movimiento(unsigned int dirr,int mov){
    char byteh,bytel;
    byteh= mov >> 8;
    bytel=mov; 
    if(dirr==0){
        MoveSpeed(1,mov,100);
    }else{   
        dato_enviado[0]=VER ;       //verificacion
        dato_enviado[1]=dirr ;      //dir
        dato_enviado[2]=ESCRITURA ; //operacion
        dato_enviado[3]=MOTOR;      //lugar
        dato_enviado[4]= byteh ;    //dato
        dato_enviado[5]= bytel ;    //dato
        CAN_SendMsg0(); 
        CyDelay(25);
        }
}