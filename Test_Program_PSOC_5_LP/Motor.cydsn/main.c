#include <project.h>
#include <DynamixelSerial1.h>
int8 dato_enviado;
int8 dato_recibido;
int8 Checksum;

int LED(unsigned char ID, int Status){    
    Checksum = (~(ID + AX_LED_LENGTH + AX_WRITE_DATA + AX_LED + Status))&0xFF;
    EMITE_PutChar(AX_START);              // Send Instructions over Serial
    EMITE_PutChar(AX_START);
    EMITE_PutChar(ID);
    EMITE_PutChar(AX_LED_LENGTH);
    EMITE_PutChar(AX_WRITE_DATA);
    EMITE_PutChar(AX_LED);
    EMITE_PutChar(Status);
    EMITE_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    return 1;//(read_error());              // Return the read error
}


int mover(unsigned char ID, int Position)
{
    char Position_H,Position_L;
    Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
    Position_L = Position;
	Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;
    //EMITE_LoadTxConfig();
    //	switchCom(Direction_Pin,Tx_MODE);
    EMITE_PutChar(AX_START);                 // Send Instructions over Serial
    EMITE_PutChar(AX_START);
    EMITE_PutChar(ID);
    EMITE_PutChar(AX_GOAL_LENGTH);
    EMITE_PutChar(AX_WRITE_DATA);
    EMITE_PutChar(AX_GOAL_POSITION_L);
    EMITE_PutChar(Position_L);
    EMITE_PutChar(Position_H);
    EMITE_PutChar(Checksum);
	CyDelayUs(TX_DELAY_TIME);
//	switchCom(Direction_Pin,Rx_MODE);
    //EMITE_LoadRxConfig();
    return EMITE_ReadRxData();                 // Return the read error
}

int setEndless(unsigned char ID, int Status)
{
 if ( Status ) {	
	  char AX_CCW_AL_LT = 0;     // Changing the CCW Angle Limits for Full Rotation.
	  Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L))&0xFF;
	  //EMITE_LoadTxConfig();
	  //switchCom(Direction_Pin,Tx_MODE);
      EMITE_PutChar(AX_START);                // Send Instructions over Serial
      EMITE_PutChar(AX_START);
      EMITE_PutChar(ID);
      EMITE_PutChar(AX_GOAL_LENGTH);
      EMITE_PutChar(AX_WRITE_DATA);
      EMITE_PutChar(AX_CCW_ANGLE_LIMIT_L );
      EMITE_PutChar(AX_CCW_AL_LT);
      EMITE_PutChar(AX_CCW_AL_LT);
      EMITE_PutChar(Checksum);
      CyDelayUs(TX_DELAY_TIME);
	  //switchCom(Direction_Pin,Rx_MODE);
      //EMITE_LoadRxConfig();
	  return EMITE_ReadRxData(); 
 }
 else
 {
	 //turn(ID,0,0);
	 Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L + AX_CCW_AL_L + AX_CCW_AL_H))&0xFF;
	 //EMITE_LoadTxConfig();
	 //switchCom(Direction_Pin,Tx_MODE);
	 EMITE_PutChar(AX_START);                 // Send Instructions over Serial
	 EMITE_PutChar(AX_START);
	 EMITE_PutChar(ID);
	 EMITE_PutChar(AX_GOAL_LENGTH);
	 EMITE_PutChar(AX_WRITE_DATA);
	 EMITE_PutChar(AX_CCW_ANGLE_LIMIT_L);
	 EMITE_PutChar(AX_CCW_AL_L);
	 EMITE_PutChar(AX_CCW_AL_H);
	 EMITE_PutChar(Checksum);
	 CyDelayUs(TX_DELAY_TIME);
     //EMITE_LoadRxConfig();
	 //switchCom(Direction_Pin,Rx_MODE);
	 
	 return EMITE_ReadRxData();                 // Return the read error
  }
 } 

int torqueStatus( unsigned char ID, int Status)
{
    Checksum = (~(ID + AX_TORQUE_LENGTH + AX_WRITE_DATA + AX_TORQUE_ENABLE + Status))&0xFF;
	//switchCom(Direction_Pin,Tx_MODE);
    //EMITE_LoadTxConfig();
    EMITE_PutChar(AX_START);              // Send Instructions over Serial
    EMITE_PutChar(AX_START);
    EMITE_PutChar(ID);
    EMITE_PutChar(AX_TORQUE_LENGTH);
    EMITE_PutChar(AX_WRITE_DATA);
    EMITE_PutChar(AX_TORQUE_ENABLE);
    EMITE_PutChar(Status);
    EMITE_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
	//switchCom(Direction_Pin,Rx_MODE);
    //EMITE_LoadRxConfig();
    return EMITE_ReadRxData();               // Return the read error
}

int moveSpeed(unsigned char ID, int Position, int Speed)
{
    char Position_H,Position_L,Speed_H,Speed_L;
    Position_H = Position >> 8;    
    Position_L = Position;                // 16 bits - 2 x 8 bits variables
    Speed_H = Speed >> 8;
    Speed_L = Speed;                      // 16 bits - 2 x 8 bits variables
	Checksum = (~(ID + AX_GOAL_SP_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H + Speed_L + Speed_H))&0xFF;
    //EMITE_LoadTxConfig();
	//switchCom(Direction_Pin,Tx_MODE);
    EMITE_PutChar(AX_START);                // Send Instructions over Serial
    EMITE_PutChar(AX_START);
    EMITE_PutChar(ID);
    EMITE_PutChar(AX_GOAL_SP_LENGTH);
    EMITE_PutChar(AX_WRITE_DATA);
    EMITE_PutChar(AX_GOAL_POSITION_L);
    EMITE_PutChar(Position_L);
    EMITE_PutChar(Position_H);
    EMITE_PutChar(Speed_L);
    EMITE_PutChar(Speed_H);
    EMITE_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    //EMITE_LoadRxConfig();
	//switchCom(Direction_Pin,Rx_MODE);
    
    return EMITE_ReadRxData();                // Return the read error
}

int moveRW(unsigned char ID, int Position)
{
    char Position_H,Position_L;
    Position_H = Position >> 8;           // 16 bits - 2 x 8 bits variables
    Position_L = Position;
    Checksum = (~(ID + AX_GOAL_LENGTH + AX_REG_WRITE + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;
    //EMITE_LoadTxConfig();
	//switchCom(Direction_Pin,Tx_MODE);
    EMITE_PutChar(AX_START);                 // Send Instructions over Serial
    EMITE_PutChar(AX_START);
    EMITE_PutChar(ID);
    EMITE_PutChar(AX_GOAL_LENGTH);
    EMITE_PutChar(AX_REG_WRITE);
    EMITE_PutChar(AX_GOAL_POSITION_L);
    EMITE_PutChar(Position_L);
    EMITE_PutChar(Position_H);
    EMITE_PutChar(Checksum);
	CyDelayUs(TX_DELAY_TIME);
	//switchCom(Direction_Pin,Rx_MODE);
	//EMITE_LoadRxConfig();
    return EMITE_ReadRxData();                  // Return the read error
}

void action()
{	//EMITE_LoadTxConfig();
	//switchCom(Direction_Pin,Tx_MODE);
    EMITE_PutChar(AX_START);                // Send Instructions over Serial
    EMITE_PutChar(AX_START);
    EMITE_PutChar(BROADCAST_ID);
    EMITE_PutChar(AX_ACTION_LENGTH);
    EMITE_PutChar(AX_ACTION);
    EMITE_PutChar(AX_ACTION_CHECKSUM);
	CyDelayUs(TX_DELAY_TIME);
	//switchCom(Direction_Pin,Rx_MODE);
}


int main()
{
    int conta=0;
    int dato_aux;
    CyGlobalIntEnable; /* Enable global interrupts. */
    EMITE_Start();
    //EMITE_LoadTxConfig();
    LCD_Char_1_Start();
    LCD_Char_1_Position(0,0);
    LCD_Char_1_PrintString("Datos...");
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        
        //mover(1,700);  // Move the Servo radomly from 200 to 800
        //CyDelay(1000);
        dato_recibido=moveSpeed(1,200,200);
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintInt8(dato_recibido);
        CyDelay(2000);
        //setEndless(1,ON);
        //turn(1,RIGTH,1000);
        //CyDelay(3000);
        //turn(1,LEFT,1000);
        //CyDelay(3000);
        //setEndless(1,OFF);
        dato_recibido=LED(1,1);
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintInt8(dato_recibido);
        //moveRW(1,512);
        CyDelay(1000);
        action();
        dato_recibido=LED(1,0);
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintInt8(dato_recibido);
        CyDelay(1000);
        dato_recibido=moveSpeed(1,600,500);
        LCD_Char_1_Position(1,0);
        LCD_Char_1_PrintInt8(dato_recibido);
        }
}
