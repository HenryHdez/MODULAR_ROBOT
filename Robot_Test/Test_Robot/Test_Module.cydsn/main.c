#include <project.h>
//Import Libraries
#include <AX12.h>
#include <VCNL4010_1.h>
#include <VCNL4010_2.h>
#include <VCNL4010_3.h>
#include <VCNL4010_4.h>
// If any sensor does not work the program will not start
//Global Variables
int16 S1=0,S2=0,S3=0,S4=0;
uint8 dato_enviado;
uint8 dato_recibido;

//Function for Motor Test
void test_M(){
        //Move Motor to Left
        LED_2_Write(1);
        LED(1,1);
        action();
        turn(1,0,300);
        Move(1,200);
        CyDelay(1000);
        //Shutdown Motor
        LED_2_Write(0);
        LED(1,0);
        action();
        MoveSpeed(1,0,0);
        CyDelay(2000);
        //Move Motor to Rigth
        LED_2_Write(1);
        LED(1,1);
        action();
        turn(1,1,300);
        Move(1,200);
        CyDelay(1000); 
        //Shutdown Motor
        LED_2_Write(0);
        LED(1,0);
        action();
        MoveSpeed(1,0,0);
        CyDelay(2000);
}

int main()
{
    CyGlobalIntEnable; 
    //Init LED's
    LED_1_Write(0);
    LED_2_Write(0);
    LED_3_Write(0);
    LED_4_Write(0);
    //Init CAN
    CAN_Start();
    //Init Serial
    MOTOR_Start();
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
    initVCNL_4();
    for(;;)
    {
        //Intructions for read sensor status
        //S1=readProximity_1();
        S2=readProximity_2();
        S3=readProximity_3();
        S4=readProximity_4();
        if(S1<3000||S2<3000||S3<3000||S4<3000){LED_1_Write(1);}
        else{LED_1_Write(0);} 
        test_M();
        //Send and read data over CAN 
        CAN_SendMsg0();
        CyDelay(500);
        dato_enviado++;
        CAN_ReceiveMsg0();
        if(dato_recibido==0){LED_3_Write(0);}
        else{LED_3_Write(1);}
        }
}

