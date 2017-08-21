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
     
    CAN_Start(); ///  start CAN
    CAN_isr_StartEx(interrutcan);//interrucion can
    dir=1;  ///direcion del motor
    RX_Start();///rx motor
    MOTOR_Start();//tx motor
    TXC_Write(3);// rx y tx desabilitados
    CAN_GlobalIntEnable();
         
    inicio();///asignacion de posicion
    //cal(0);
    int desfase;
    for(;;)//////////////////////////////////////////////////////////////////////////////////////////////////////////
    { if(Mydir==DIRM){
 
        cal(desfase); /// desplasamiento 
        if(desfase==360){
            
            desfase=0;
        }else desfase=desfase+30;
        
        
        
        
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
