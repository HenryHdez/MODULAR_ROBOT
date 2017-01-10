#include <project.h>
#define VCNL4010_ADDRESS 0x13

//Send Data
void writeVCNL(int address, int data){
    uint8 Write_buf[2]={0};
    Write_buf[0]=address;
    Write_buf[1]=data;
    I2C_1_MasterWriteBuf(VCNL4010_ADDRESS,(uint8 *)Write_buf,2,I2C_1_MODE_COMPLETE_XFER);
    while((I2C_1_MasterStatus()&I2C_1_MSTAT_WR_CMPLT)==0){}
    return;
}
//Read Data
int readVCNL(int address){
    uint8 Write_buf[1]={0};
    Write_buf[0]=address;
    uint8 read_Buf[1]={0};
    I2C_1_MasterWriteBuf(VCNL4010_ADDRESS,(uint8 *)Write_buf,1,I2C_1_MODE_NO_STOP);
    while((I2C_1_MasterStatus()&I2C_1_MSTAT_WR_CMPLT)==0){}
    I2C_1_MasterReadBuf(VCNL4010_ADDRESS,(uint8 *)read_Buf,1,I2C_1_MODE_REPEAT_START);
    while((I2C_1_MasterStatus()&I2C_1_MSTAT_RD_CMPLT)==0){}
    return read_Buf[0];
}
//Init Sensor
void initVCNL(){
  int temp = readVCNL(0x81);
  if (temp != 0x21){  
    //If LCD provided
    LCD_Char_1_Position(1,0);
    LCD_Char_1_PrintString("Fail...");
    
  }
   else{
    LCD_Char_1_Position(1,0);
    LCD_Char_1_PrintString("Ok...");
  } 
  //VCNL Init Params
  writeVCNL(0x84, 0x0F); // Configures ambient light measures - Single conversion mode, 128 averages
  writeVCNL(0x83, 20);   // sets IR current in steps of 10mA 0-200mA --> 200mA
  writeVCNL(0x89, 0);    // Proximity IR test signal freq, 0-3 - 781.25 kHz
  writeVCNL(0x8A, 0x81); // proximity modulator timing - 129, recommended by Vishay 
}
//Read Distance Signal
unsigned int readProximity(){
  int temp = readVCNL(0x80);
  writeVCNL(0x80, temp | 0x08);             // command the sensor to perform a proximity measure
  while(!(readVCNL(0x80)&0x20));            // Wait for the proximity data ready bit to be set
  unsigned int data = readVCNL(0x87) << 8;
  data |= readVCNL(0x88);
  return data;
}
//Read Ambient Signal
unsigned int readAmbient(){
  int temp = readVCNL(0x80);
  writeVCNL(0x80, temp | 0x10);             // command the sensor to perform ambient measure
  while(!(readVCNL(0x80)&0x40));            // wait for the proximity data ready bit to be set
  unsigned int data = readVCNL(0x85) << 8;  
  data |= readVCNL(0x86);
  return data;
}

int main()
{
    CyGlobalIntEnable; 
    LCD_Char_1_Start();
    I2C_1_Start();
    I2C_1_Enable();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("Sensor");
    CyDelay(100);
    initVCNL();
    for(;;)
    {
        LCD_Char_1_Position(0,0);
        LCD_Char_1_PrintString("Amb=    ");
        LCD_Char_1_Position(0,8);
        LCD_Char_1_PrintDecUint16(readAmbient());
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintString("Prox=   ");
        LCD_Char_1_Position(1,8);
        LCD_Char_1_PrintDecUint16(readProximity());
        }
}

/* [] END OF FILE */
