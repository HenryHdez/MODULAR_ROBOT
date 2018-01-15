#include <project.h>
#include <inicio.h>
int conta=0;


CY_ISR(interrutcan){
    LED_3_Write(1);
    CAN_MsgRXIsr();///para marcar la interrucion como leida y resive msm
    recibmail();
    recibmail_1();
    LED_3_Write(0);
}

int main()
{  
    CyGlobalIntEnable;              /* Enable global interrupts. */
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

    initVCNL_1();
    initVCNL_2();
    initVCNL_3();
    initVCNL_4();
    Orientacion_Modulo();    
    CAN_Start();                    //  start CAN
    CAN_isr_StartEx(interrutcan);   //interrucion can
    dir=1;                          //direcion del motor
    RX_Start();                     //rx motor
    MOTOR_Start();                  //tx motor
    TXC_Write(3);                   // rx y tx desabilitados
    CAN_GlobalIntEnable();
    inicio();
    Condicion();
    if(Mydir==0){Send_Angle(0,1);}
    for(;;){
    //Bucle Infinito
        if(Mydir==0){
            dato_enviado_2[0]=SENSORES;
            if(readProximity_1()>2500){
                dato_enviado_2[1]=1;
            }
            else{dato_enviado_2[1]=0;}
            if(readProximity_2()>2500&&readPosition(1)<=512){
                dato_enviado_2[2]=1;
            }
            else{dato_enviado_2[2]=0;}
            if(readProximity_3()>2500){
                dato_enviado_2[3]=1;
            }
            else{dato_enviado_2[3]=0;}
        }
    }
}
