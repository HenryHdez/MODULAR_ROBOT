#include <project.h>
#include <inicio.h>




CY_ISR(interrutcan){

CAN_MsgRXIsr();///para marcar la interrucion como leida y resive msm
//CAN_INT_SR_REG = CAN_RX_MESSAGE_MASK;
//CAN_ReceiveMsg0();
//Pda=readAmbient_2(); 
 recibmail();
//  LED_3_Write(1); 
//            CyDelay(20);
//               LED_3_Write(0); 
//             CyDelay(20);
}



int main()
{  
    CyGlobalIntEnable; /* Enable global interrupts. */

    //Init LED's
    LED_1_Write(0);
    LED_2_Write(0);
    LED_3_Write(0);
    LED_4_Write(0);
    //Init Sensor's
    SENSOR_1_Start();
    SENSOR_1_Enable();
    SENSOR_2_Start();
    SENSOR_2_Enable();
    SENSOR_3_Start();
    SENSOR_3_Enable();
    SENSOR_4_Start();
    SENSOR_4_Enable();    
    CyDelay(100);
    initVCNL_1();
    initVCNL_2();
    initVCNL_3();
   // initVCNL_4();
     int desfase,pxd,pxi,defr,dato,pos;
    CAN_Start(); ///  start CAN
    CAN_isr_StartEx(interrutcan);//interrucion can
    dir=1;  ///direcion del motor
    RX_Start();///rx motor
    MOTOR_Start();//tx motor
    TXC_Write(3);// rx y tx desabilitados
    CAN_GlobalIntEnable();
         MoveSpeed(1,512,100);
    inicio();///asignacion de posicion
    MoveSpeed(1,512,100);
    movimiento(1,512);/////// aliniacion
    movimiento(2,512);
    movimiento(3,512);
    movimiento(4,512);
    movimiento(5,512);
    pxd=readProximity_1();
         pxi=readProximity_3();
         defr=readProximity_2();
   LED_2_Write(0);
    
  
    
    for(;;)//////////////////////////////////////////////////////////////////////////////////////////////////////////
    { if(Mydir==DIRM){
 

        
     pos  = readPosition(1);
    
        if(pos<=500){
             LED_2_Write(1);
         pxd=readProximity_1();
         pxi=readProximity_3();
         defr=readProximity_2();
        LED_2_Write(0);
        }else;
        
        
        if(pxd>=2500&&pxi>=2950&&defr>=2900){
           dato=5;
            goto salto;
        }else ;
        
        if(defr<=2900){
            dato=1;
            goto salto;
            //// elige derecha o izquierda
            
        } else{
        if(pxi<=pxd){
            dato=2;
        }else dato=3;
        
        
        
        }
        
        
        
        
        
        
        
        
       salto: 
       switch(dato){
        
        
        case 1:{
        cal(desfase); /// desplasamiento 
        if(desfase==360){
               desfase=0;
        }else desfase=desfase+30;
        
        
        }
        break;
        case 2:{
        /////izquierda
    MoveSpeed(1,512,100);
     movimiento(1,512);/////// aliniacion
    movimiento(2,512);
    movimiento(3,512);
    movimiento(4,512);
    movimiento(5,674);//// ancha t
    CyDelay(2000);
    movimiento(2,700);/// mover parte de adelante
    movimiento(3,600);///mover parte trasera
    CyDelay(2000);
    movimiento(5,512);//// ancha t
    MoveSpeed(1,350,100);///ancla delantera
    CyDelay(2000);
    movimiento(2,512);
    movimiento(3,512);///mover parte trasera
    CyDelay(2000);
       
        }
        break;
    case 3:{    
    ////derecha
    
    MoveSpeed(1,512,100);    
    movimiento(1,512);/////// aliniacion
    movimiento(2,512);
    movimiento(3,512);
    movimiento(4,512);
    movimiento(5,674);//// ancha t
    CyDelay(2000);
    movimiento(2,324);/// mover parte de adelante
    movimiento(3,424);///mover parte trasera
    CyDelay(2000);
    movimiento(5,512);//// ancha t
    MoveSpeed(1,350,100);///ancla delantera
    CyDelay(2000);
    movimiento(2,512);
    movimiento(3,512);///mover parte trasera
    CyDelay(2000);
    }
    break;
    
    case 5:{
    
       
    while(pxd>=2500&&pxi>=2950){
    cal(desfase); /// desplasamiento 
    if(desfase==0){
               desfase=360;
        }else desfase=desfase-30;
      CyDelay(100);
         pxd=readProximity_1();
         pxi=readProximity_3();
    }
        
          MoveSpeed(1,512,100);    
    movimiento(1,512);/////// aliniacion
    movimiento(2,512);
    movimiento(3,512);
    movimiento(4,512);
    movimiento(5,674);//// ancha t
    CyDelay(2000);
    movimiento(2,324);/// mover parte de adelante
    movimiento(3,424);///mover parte trasera
    CyDelay(2000);
    movimiento(5,512);//// ancha t
    MoveSpeed(1,350,100);///ancla delantera
    CyDelay(2000);
    movimiento(2,512);
    movimiento(3,512);///mover parte trasera
    CyDelay(2000);
    
      MoveSpeed(1,512,100);    
    movimiento(1,512);/////// aliniacion
    movimiento(2,512);
    movimiento(3,512);
    movimiento(4,512);
    movimiento(5,674);//// ancha t
    CyDelay(2000);
    movimiento(2,324);/// mover parte de adelante
    movimiento(3,424);///mover parte trasera
    CyDelay(2000);
    movimiento(5,512);//// ancha t
    MoveSpeed(1,350,100);///ancla delantera
    CyDelay(2000);
    movimiento(2,512);
    movimiento(3,512);///mover parte trasera
    CyDelay(2000);
        
    }
      break;  
    
    }
    
    LED_1_Write(1);
    
    
    
    
    
 ledoff(0,dirasig,LED4);
//movimiento(dirasig,500);
      CyDelay(100);  
    LED_1_Write(0);
    ledoff(1,dirasig,LED4);
   // movimiento(dirasig,559);
     CyDelay(100);  
    }else{//LED_1_Write(1); 
     
      CyDelay(1000);  
    //LED_1_Write(0);
    
     CyDelay(1000); }
}
}
