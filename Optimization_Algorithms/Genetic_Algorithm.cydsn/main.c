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

//Crear Array con tamaño de la población 
int Individuos=10;
float Poblacion[5][10];
float Poblacion_1[5][10];
float Fitness[10];
float Fitness_1[10];
float Fitness_Normalizado[10];
int Fila=0;
int Columna=0;
float Umbral_Cruce=0.7;
float Umbral_Mutacion=0.3;
int conmutador=0;
int puntero=0;
int indice[10];
float Acu=0.0;
float bolita=0.0;

double randd() {
  return (double)rand()/((double)RAND_MAX + 1);
}

double random_gauss(){
    return randd()*0.2;
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
    r=-2*randd();
    r=r+1;
    return r;
}

double random_Offset(){
    float r=0.0;
    r=randd();
    //r=r*-0.01;
    //r=r+0.02;
    r=r*0.4;
    r=r-0.2;
    r=0.833+r;
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
        else if(mailbox2[1]==0&&mailbox2[2]==0&&mailbox2[3]==1) {Peso_Sensor=20;}   //Izquierda
        else if(mailbox2[1]==0&&mailbox2[2]==1&&mailbox2[3]==0) {Peso_Sensor=60;}   //Frontal
        else if(mailbox2[1]==0&&mailbox2[2]==1&&mailbox2[3]==1) {Peso_Sensor=80;}   //Frontal e Izquierda
        else if(mailbox2[1]==1&&mailbox2[2]==0&&mailbox2[3]==0) {Peso_Sensor=20;}   //Derecha
        else if(mailbox2[1]==1&&mailbox2[2]==0&&mailbox2[3]==1) {Peso_Sensor=40;}   //Derecha e Izquierda
        else if(mailbox2[1]==1&&mailbox2[2]==1&&mailbox2[3]==0) {Peso_Sensor=80;}   //Frontal y Derecha
        else if(mailbox2[1]==1&&mailbox2[2]==1&&mailbox2[3]==1) {Peso_Sensor=100;}     //Todos
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

float maximo(float *valores, int num){ 
  int i; 
  float max; 
  max = valores[0]; 
  for (i = 1; i < num; i++) 
    if (valores[i] > max) 
	  max = valores[i]; 
  return (max); 
} 

void Normalizar(int I){
    float Valor_Maximo=maximo(Fitness,I);
    float Valor_Maximo_1=maximo(Fitness_1,I);
    int i=0;
    for (i=0;i<Individuos;i++){
        Fitness[i]=Fitness[i]/Valor_Maximo;
        Fitness[i]=1.0-Fitness[i];
        Fitness_1[i]=Fitness_1[i]/Valor_Maximo_1;
        Fitness_1[i]=1.0-Fitness_1[i];        
    }
}

void Evaluar_Fitness(int i){
    Amplitud_V=Poblacion_1[0][i];
    Amplitud_H=Poblacion_1[1][i];
    Fase=Poblacion_1[2][i];
    Offset_V=Poblacion_1[3][i];
    Offset_H=Poblacion_1[4][i];    
        for(int i=0;i<5;i++){
            Enviar_Parametros(Amplitud_H,Fase,Offset_H,H);
            CyDelay(50);
            Enviar_Parametros(Amplitud_V,Fase,Offset_V,V);
            CyDelay(50);
            Send_Angle(0,0);
        }   
}

void Espere(){
    Amplitud_V=0.0;
    Amplitud_H=0.0;
    Fase=0.0;
    Offset_V=0.833;
    Offset_H=0.833;
        for(int i=0;i<5;i++){
            Enviar_Parametros(Amplitud_H,Fase,Offset_H,H);
            CyDelay(50);
            Enviar_Parametros(Amplitud_V,Fase,Offset_V,V);
            CyDelay(50);
            Send_Angle(0,0);
        }   
}

void Seleccion(){
    int i=0,j=0;
    Acu=0.0;
    bolita=0.0;
    //Crear Vector de Seleccion
    for(i=0;i<Individuos;i++){
        Acu=Acu+Fitness[i];            
    }
    for(i=0;i<Individuos;i++){
        Fitness_Normalizado[i]=Fitness[i]/Acu;            
    }    
    //Ruleta
    for(i=0;i<Individuos;i++){
        bolita=randd();
        Acu=0.0;
        j=0;
        while (Acu<bolita){
            Acu=Fitness_Normalizado[j]+Acu;
            indice[i]=j;
            j++;
        }
    } 
    //Llenar Matriz de población_1
    for(i=0;i<Individuos;i++){
        Poblacion_1[0][i]=Poblacion[0][indice[i]];
        Poblacion_1[1][i]=Poblacion[1][indice[i]];
        Poblacion_1[2][i]=Poblacion[2][indice[i]];
        Poblacion_1[3][i]=Poblacion[3][indice[i]];
        Poblacion_1[4][i]=Poblacion[4][indice[i]];
        Fitness_1[i]=Fitness[indice[i]];
    }     
}

float Saturador(float numero_comprobar,float lim_inf, float lim_sup){
    if(numero_comprobar>lim_sup){return lim_sup;}
    else if (numero_comprobar<lim_inf){return lim_inf;}
    else{return numero_comprobar;}
}

void reparacion(int Aux_ind){
    if(randd()<0.5){
    Poblacion_1[0][Aux_ind] = Saturador(Poblacion_1[0][Aux_ind],0.0,0.2);      
    Poblacion_1[1][Aux_ind] = Saturador(Poblacion_1[1][Aux_ind],0.0,0.8);
    }
    else{
    Poblacion_1[0][Aux_ind] = Saturador(Poblacion_1[0][Aux_ind],0.0,0.8);      
    Poblacion_1[1][Aux_ind] = Saturador(Poblacion_1[1][Aux_ind],0.0,0.2);
    }    
    Poblacion_1[2][Aux_ind] = Saturador(Poblacion_1[2][Aux_ind],-1.0,1.0);   
    Poblacion_1[3][Aux_ind] = Saturador(Poblacion_1[3][Aux_ind],(0.833-0.3),(0.833+0.3));
    Poblacion_1[4][Aux_ind] = Saturador(Poblacion_1[4][Aux_ind],(0.833-0.3),(0.833+0.3));
}

void Cruce(){
    int papa=(int)Individuos*randd();
    int mama=(int)Individuos*randd();
    for (int i=0; i<5; i++) {
        float alpha = randd();
        float alpha_1 = randd();
        float neg_alpha = 1.0 - alpha;
        float neg_alpha_1 = 1.0 - alpha_1;
        float tx;
        float ty;
        tx = Poblacion_1[i][papa];
        ty = Poblacion_1[i][mama];
        Poblacion_1[i][papa] = alpha * tx + neg_alpha * ty;
        Poblacion_1[i][mama] = alpha_1 * tx + neg_alpha_1 * ty;
    } 
    reparacion(papa);
    reparacion(mama);
}

void Mutacion(){
    int Selector=(int)Individuos*randd();
    int componente=(int)randd()*4;
    if(componente==0){Poblacion_1[componente][Selector]=Poblacion_1[componente][Selector]+random_gauss();}
    else if(componente==1){Poblacion_1[componente][Selector]=Poblacion_1[componente][Selector]+random_gauss();}
    else if(componente==2){Poblacion_1[componente][Selector]=Poblacion_1[componente][Selector]+random_gauss();}
    else if(componente==3){Poblacion_1[componente][Selector]=Poblacion_1[componente][Selector]+random_gauss();}
    else if(componente==4){Poblacion_1[componente][Selector]=Poblacion_1[componente][Selector]+random_gauss();} 
    reparacion(Selector);
}

void Reemplazo_Generacional(){
    int i=0;
    for(i=0;i<Individuos;i++){
        if(Fitness_1[i]<Fitness[i]){
            Poblacion[0][i]=Poblacion_1[0][i];
            Poblacion[1][i]=Poblacion_1[1][i];
            Poblacion[2][i]=Poblacion_1[2][i];
            Poblacion[3][i]=Poblacion_1[3][i];
            Poblacion[4][i]=Poblacion_1[4][i];
            }
    }
}

void A_G(){
    float Prob_c=0.5;
    float Prob_m=0.2;
    //Ingrese su Algoritmo Aqui
    switch (flag){
        case 0:
            Normalizar(Individuos);
            Reemplazo_Generacional();
            Max_Conta=5;
            puntero=0;
            flag=1;        
            break;
        case 1:
            Espere();
            Seleccion();
            Max_Conta=5;
            flag=2;
            break;
        case 2:
            Prob_c=randd();
            if(Prob_c<Umbral_Cruce){
                Cruce();
            }
            Prob_m=randd();
            if(Prob_m<Umbral_Mutacion){
                Mutacion();
            }
            flag=3;
            break;
        case 3:
            switch (conmutador){
                case 0:
                    conmutador=1;
                    Evaluar_Fitness(puntero);
                    Max_Conta=50;                    
                break;
                case 1:
                    conmutador=2;
                    Espere();
                    Max_Conta=20;
                break;
                case 2:
                    conmutador=0;
                    Fitness_1[puntero]=dist;
                    //dist=dist*100;
                    dato_enviado_2[0]=individuo;
                    dato_enviado_2[1]=(int)dist;
                    CAN_SendMsg3();                    
                    puntero++;
                    Max_Conta=10;
                    if(puntero==Individuos){flag=0;}
                break;
            }
            break;
    }
}

void Ejecutar(){
    conta++;
    if(conta==Max_Conta){     
        LED_1_Write(1);    
        A_G();         
        conta=0;
    }
    else{
        LED_1_Write(0);
    }
}
//Interrupt's
CY_ISR(Temporizador){
    Ejecutar();  
}

CY_ISR(Serial_Interrupt){
    dist=SENSOR_U_GetChar()/2.0;
    dist=dist+Peso_Sensor;
}

int main()
{
    int i=0;
    for(i=0;i<Individuos;i++){
        Fitness[i]=randd();
        if(randd()<0.5){
            Poblacion[0][i]=random_amplitud_1();
            Poblacion[1][i]=random_amplitud_2();
            Poblacion[2][i]=random_Fase();
            Poblacion[3][i]=random_Offset();
            Poblacion[4][i]=random_Offset(); 
        }
        else{
            Poblacion[0][i]=random_amplitud_1();
            Poblacion[1][i]=0.0;
            Poblacion[2][i]=random_Fase();
            Poblacion[3][i]=0.833;
            Poblacion[4][i]=random_Offset(); 
        } 
    }
    for(i=0;i<Individuos;i++){
        Fitness_1[i]=randd();
        if(randd()<0.5){
            Poblacion_1[0][i]=random_amplitud_1();
            Poblacion_1[1][i]=random_amplitud_2();
            Poblacion_1[2][i]=random_Fase();
            Poblacion_1[3][i]=random_Offset();
            Poblacion_1[4][i]=random_Offset(); 
        }
        else{
            Poblacion_1[0][i]=random_amplitud_1();
            Poblacion_1[1][i]=0.0;
            Poblacion_1[2][i]=random_Fase();
            Poblacion_1[3][i]=0.833;
            Poblacion_1[4][i]=random_Offset(); 
        }
    }
    Max_Conta=150;    
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
