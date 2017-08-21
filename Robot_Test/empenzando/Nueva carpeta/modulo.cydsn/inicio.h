#include <VCNL4010_1.h>
#include <VCNL4010_2.h>
#include <VCNL4010_3.h>
//#include <VCNL4010_4.h>
#include <AX12.h>
#include <math.h>
#include <protocolo can.h>

int  ant,t,Pd,Pt,cstatus,M,Pta,errc,PositionM,pos; 
unsigned char dir=1; //////dir motor
unsigned int Pda;
uint8 Mydir=250;
uint8 dirasig ;
uint8 sumadir;
uint8 dato_enviado[8];
uint8 mailbox0[8];
float srf;
unsigned char dir,tem,tes,error,Error_Byte,Incoming_Byte2;
int amb,posicion,posicionh,posicionl;

/////////////////////////////////////codigo para identificar//////////////////////////



int asignacion(){


  dato_enviado[0]=VER ;//verificacion
  dato_enviado[1]=DIRCS ;//dir
  dato_enviado[2]=ESCRITURA ;//operacion
  dato_enviado[3]=MEC;//lugar
  dato_enviado[4]= dirasig ;  //dato... en este caso direcion para asignar

      CAN_SendMsg0(); 
CyDelay(25);

return(0);
}
int ledoff(int datas,int direccion,int led){
    
    
  dato_enviado[0]=VER ;//verificacion
  dato_enviado[1]=direccion ;//dir
  dato_enviado[2]=ESCRITURA ;//operacion
  dato_enviado[3]=led;//lugar
  dato_enviado[4]= datas ;  //dato... en este caso direcion para asignar
    
      CAN_SendMsg0(); 
    
    CyDelay(25);
    return 0;
}
int inicio(){
    ini:
        Pd=readProximity_2();
       // Pt=readProximity_4();
       
    
      
    if(Mydir!=DIRCS){
       goto end;
    }else{
    
    
    errc=CAN_GetErrorState();  ///////////status can
        if(errc==0){
      
            if(Pd>=3000){Pda=readAmbient_2(); 
                LED_3_Write(1); 
     Pda=readAmbient_2(); 
      CyDelay(4);  
    LED_3_Write(0);
    Pda=readAmbient_2(); 
     CyDelay(4); 
              Pda=readAmbient_2(); 
                goto ini;
                          
            
        }else{
            
            
            
            
            Mydir=DIRM;
            
           
            
         
      
//        /////esperar respuesta de vericacion de asignacion de dir
//        
         
            
            
            LED_2_Write(1);  ///identificador
           LED_4_Write(1);///led trasero 
          
            
            asig:
            CyDelay(2000);///t para lectura amb
         ///
            dirasig=sumadir+1;
            sumadir=0;
            asignacion();/////envio1
            while(sumadir!=dirasig){//// espera de verificacion de llegada
                
                CyDelay(75);
                
            
            if(t==10){ LED_4_Write(0);
            dirasig= dirasig-1;
            
            ledoff(0,dirasig,LEDF);
            ant=dirasig; ///valor real de numero de modulos
            
            ant=dirasig-1;
            CyDelay(5);
             ledoff(0,ant,LEDF);
            
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
       
       
     end:   
     return (0);
              
}

int recibmail(){

 
   if( (mailbox0[0]==VER)&(mailbox0[1]==Mydir)){
    
    switch(mailbox0[2]){
        ///////////////////////////////////////////////////////
         case LECM:{ 
        switch(mailbox0[3]){//////////+6+++++++++++++
        case MOTOR:{
        char mh,ml;
             pos = readPosition(1);
            mh=pos>>8;
            ml=pos;
                dato_enviado[0]=VER;
                dato_enviado[1]=DIRM;
                dato_enviado[2]=MOTOR;
                dato_enviado[3]=mh;
                dato_enviado[4]=ml;
               
                CAN_SendMsg0();
        
        }
            
            
            
            break;
        case SENSOR1:
            switch(mailbox0[4]){
            case SAMB:///code
                
                break;
                case SPROX: ///code
                
                break;
            
            }
            
            break;
        case SENSOR2:  
            break;
        case SENSOR3:  
            break;
        case SENSOR4: 
            break;
  

        }break;///////+++++++++++++++++
        
        }break;
        
        
        /////////////////////////////////////////////////////
        
        case ESCRITURA:{
        switch(mailbox0[3]){////+++++++++++++++++++++++++++++++++++
        case MEC:{

           
         
              CyDelay(100);
           LED_1_Write(1);
           CyDelay(100);
           LED_1_Write(0);
            if(Pda>=5000){
                Mydir=mailbox0[4];
                dato_enviado[0]=VER;
                dato_enviado[1]=DIRM;
                dato_enviado[2]=OK;
                dato_enviado[3]=Mydir;
                dato_enviado[4]=0;
                dato_enviado[5]=0;
                CAN_SendMsg0();
               LED_1_Write(1); 
            }
            else;}
        break;
         case MOTOR:
            
                PositionM = mailbox0[4] << 8;           
                PositionM = PositionM+mailbox0[5];
                MoveSpeed(dir,PositionM,100);
        break;
             case LED1:
            switch(mailbox0[4]){
                case ONM:  LED_1_Write(1);  break;
                case OFFM: LED_1_Write(0);  break;
                 }
        break;
             case LED2:
             switch(mailbox0[4]){
                case ONM:  LED_2_Write(1);  break;
                case OFFM: LED_2_Write(0);  break;
                 }
        break;
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
        
        }//////++++++++++++++++++++++++++++++++
        }break;
        case OK:{sumadir=mailbox0[3]; }break;
        /////////////////////////////////////////////////////
        }
        //////
        
        
            
            
        
   
        
    }else;
    
    
    
    
    

    
    
    

   return 0;
}
int movimiento(unsigned int dirr,int mov){
char byteh,bytel;
   byteh= mov >> 8;
   bytel=mov;
    
    
//    if(dirr!=2 && dirr!=3){
    
    
    if(dirr==0){
        MoveSpeed(1,mov,100);
    }else{
    
  dato_enviado[0]=VER ;//verificacion
  dato_enviado[1]=dirr ;//dir
  dato_enviado[2]=ESCRITURA ;//operacion
  dato_enviado[3]=MOTOR;//lugar
  dato_enviado[4]= byteh ;  //dato
  dato_enviado[5]= bytel ;  //dato
      CAN_SendMsg0(); 
    CyDelay(25);
    }
    
//    }else;
    
return 0;
}





int cal(int df){

    
    
if(Mydir==DIRM){  
int n,m,r,v;// n num de modulos, f fase ,df desfase
unsigned int f;    
m=dirasig;  
    m=m-2;//// quitar los que estan torcisdos
n=dirasig+1;
    
    n=n-2;/// quitar los que estan torcisdos
    
    
    float fx[m],theta[m],fase[m],x,dsf;//
    dsf=(M_PI*df)/180;//// de grados a rad
    
  srf=((2*M_PI)/n);//// recorrido de fase separamiento

for(r=0;r<=m;r++){
    fase[r]=r*srf+dsf;
    fx[r]=cosf(fase[r]); /////fx puntos tangentes al sen(theta)
         theta[r]=atanf(fx[r]);///desplasamiento inicial del motor para cada modulo
                              ////en radianes
      
        
    x=theta[r]+(0.83333*M_PI);///centrando 0 del angulo
    x=(x*1023)/(1.66666*M_PI); // covercion a bit
    
f= (int)x ;     ///funcion píso
//CyDelay(2000);

v=r;//////////asignacion de movimiento para los extremos

if(v==2){
  v=4;
}else{

if(v==3){
  v=5;
}else;

}



    movimiento(v,f);  
        
}   




    

    
    
    
}else;



return 0;
}


////////////////////////////////////////////////////
//
//int depuracion(){
//
//    
//    
//
//
//
//return 0;
//}
