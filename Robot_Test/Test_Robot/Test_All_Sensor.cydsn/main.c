#include <project.h>
#include <VCNL4010_1.h>
#include <VCNL4010_2.h>
#include <VCNL4010_3.h>
#include <VCNL4010_4.h>
// If any sensor does not work the program will not start
int16 P=0;

int main()
{
    CyGlobalIntEnable; 
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
    initVCNL_4();
    for(;;)
    {
        //P=readProximity_1();
        if(P<3000){
            LED_1_Write(1);
        }
        else{
            LED_1_Write(0);
        }
        P=readProximity_2();
        if(P<3000){
            LED_2_Write(1);
        }
        else{
            LED_2_Write(0);
        }
        P=readProximity_3();
        if(P<3000){
            LED_3_Write(1);
        }
        else{
            LED_3_Write(0);
        }
        P=readProximity_4();
        if(P<3000){
            LED_4_Write(1);
        }
        else{
            LED_4_Write(0);
        }        
        }
}

