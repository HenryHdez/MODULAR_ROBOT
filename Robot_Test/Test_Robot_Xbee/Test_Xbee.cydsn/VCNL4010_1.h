#include <project.h>
#define VCNL4010_ADDRESS 0x13
int i=0;
//Send Data
void writeVCNL_1(int address, int data){
    uint8 Write_buf[2]={0};
    Write_buf[0]=address;
    Write_buf[1]=data;
    SENSOR_1_I2CMasterWriteBuf(VCNL4010_ADDRESS,(uint8 *)Write_buf,2,SENSOR_1_I2C_MODE_COMPLETE_XFER);
    while((SENSOR_1_I2CMasterStatus()&SENSOR_1_I2C_MSTAT_WR_CMPLT)==0){}
    return;
}
//Read Data
int readVCNL_1(int address){
    uint8 Write_buf[1]={0};
    Write_buf[0]=address;
    uint8 read_Buf[1]={0};
    SENSOR_1_I2CMasterWriteBuf(VCNL4010_ADDRESS,(uint8 *)Write_buf,1,SENSOR_1_I2C_MODE_NO_STOP);
    while((SENSOR_1_I2CMasterStatus()&SENSOR_1_I2C_MSTAT_WR_CMPLT)==0){}
    SENSOR_1_I2CMasterReadBuf(VCNL4010_ADDRESS,(uint8 *)read_Buf,1,SENSOR_1_I2C_MODE_REPEAT_START);
    while((SENSOR_1_I2CMasterStatus()&SENSOR_1_I2C_MSTAT_RD_CMPLT)==0){}
    return read_Buf[0];
}
//Init Sensor
void initVCNL_1(){
  int temp = readVCNL_1(0x81);
  if (temp != 0x21){  
    //Led Sequence Fail
    LED_1_Write(1);
    CyDelay(500);
    LED_1_Write(0);
    CyDelay(500);    
  }
   else{
    //Led Sequence Sucess
    LED_1_Write(0);
    for(i=1;i<5;i++){
        LED_1_Write(1);
        CyDelay(200);
        LED_1_Write(0);
        CyDelay(200); 
        }
    LED_1_Write(0);
  } 
  //VCNL Init Params
  writeVCNL_1(0x84, 0x0F); // Configures ambient light measures - Single conversion mode, 128 averages
  writeVCNL_1(0x83, 20);   // sets IR current in steps of 10mA 0-200mA --> 200mA
  writeVCNL_1(0x89, 0);    // Proximity IR test signal freq, 0-3 - 781.25 kHz
  writeVCNL_1(0x8A, 0x81); // proximity modulator timing - 129, recommended by Vishay 
}
//Read Distance Signal
unsigned int readProximity_1(){
  int temp = readVCNL_1(0x80);
  writeVCNL_1(0x80, temp | 0x08);             // command the sensor to perform a proximity measure
  while(!(readVCNL_1(0x80)&0x20));            // Wait for the proximity data ready bit to be set
  unsigned int data = readVCNL_1(0x87) << 8;
  data |= readVCNL_1(0x88);
  return data;
}
//Read Ambient Signal
unsigned int readAmbient_1(){
  int temp = readVCNL_1(0x80);
  writeVCNL_1(0x80, temp | 0x10);             // command the sensor to perform ambient measure
  while(!(readVCNL_1(0x80)&0x40));            // wait for the proximity data ready bit to be set
  unsigned int data = readVCNL_1(0x85) << 8;  
  data |= readVCNL_1(0x86);
  return data;
}
