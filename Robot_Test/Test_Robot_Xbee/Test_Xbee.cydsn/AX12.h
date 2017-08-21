#include <DynamixelSerial1.h>
#include <project.h>
int8 Checksum;
//The functions are based on the arduino library
int LED(unsigned char ID, int Status){    
    //Checksum for verification data to send
    Checksum = (~(ID + AX_LED_LENGTH + AX_WRITE_DATA + AX_LED + Status))&0xFF;
    // Send Instructions over Serial
    MOTOR_PutChar(AX_START);              
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_LED_LENGTH);
    MOTOR_PutChar(AX_WRITE_DATA);
    MOTOR_PutChar(AX_LED);
    MOTOR_PutChar(Status);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    //Finish Function
    return 1;
}

int Move(unsigned char ID, int Position){
    char Position_H,Position_L;
    //Shift register 16 bits
    Position_H = Position >> 8;           
    Position_L = Position;
	Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;
    // Send Instructions over Serial
    MOTOR_PutChar(AX_START);                 
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_GOAL_LENGTH);
    MOTOR_PutChar(AX_WRITE_DATA);
    MOTOR_PutChar(AX_GOAL_POSITION_L);
    MOTOR_PutChar(Position_L);
    MOTOR_PutChar(Position_H);
    MOTOR_PutChar(Checksum);
	CyDelayUs(TX_DELAY_TIME);
    // Return the read error
    return MOTOR_ReadRxData();                 
}

int setEndless(unsigned char ID, int Status){
    if (Status){	
    char AX_CCW_AL_LT = 0;     
    // Changing the CCW Angle Limits for Full Rotation.
	Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L))&0xFF;
    // Send Instructions over Serial
    //Checksum for verification data to send
    MOTOR_PutChar(AX_START);                
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_GOAL_LENGTH);
    MOTOR_PutChar(AX_WRITE_DATA);
    MOTOR_PutChar(AX_CCW_ANGLE_LIMIT_L );
    MOTOR_PutChar(AX_CCW_AL_LT);
    MOTOR_PutChar(AX_CCW_AL_LT);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
	return MOTOR_ReadRxData(); 
 }
 else{
	Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L + AX_CCW_AL_L + AX_CCW_AL_H))&0xFF;
    // Send Instructions over Serial
    //Checksum for verification data to send
	MOTOR_PutChar(AX_START);                 
	MOTOR_PutChar(AX_START);
	MOTOR_PutChar(ID);
	MOTOR_PutChar(AX_GOAL_LENGTH);
	MOTOR_PutChar(AX_WRITE_DATA);
	MOTOR_PutChar(AX_CCW_ANGLE_LIMIT_L);
	MOTOR_PutChar(AX_CCW_AL_L);
	MOTOR_PutChar(AX_CCW_AL_H);
	MOTOR_PutChar(Checksum);
	CyDelayUs(TX_DELAY_TIME);
    // Return the read error
	return MOTOR_ReadRxData();                 
  }
 } 

int torqueStatus( unsigned char ID, int Status){
    Checksum = (~(ID + AX_TORQUE_LENGTH + AX_WRITE_DATA + AX_TORQUE_ENABLE + Status))&0xFF;
    // Send Instructions over Serial
    //Checksum for verification data to send
    MOTOR_PutChar(AX_START); 
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_TORQUE_LENGTH);
    MOTOR_PutChar(AX_WRITE_DATA);
    MOTOR_PutChar(AX_TORQUE_ENABLE);
    MOTOR_PutChar(Status);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    // Return the read error
    return MOTOR_ReadRxData();               
}

int MoveSpeed(unsigned char ID, int Position, int Speed){
    char Position_H,Position_L,Speed_H,Speed_L;
    //Shift register 16 bits
    Position_H = Position >> 8;    
    Position_L = Position;                
    //Shift register 16 bits
    Speed_H = Speed >> 8;
    Speed_L = Speed;        
    //Checksum for verification data to send
	Checksum = (~(ID + AX_GOAL_SP_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H + Speed_L + Speed_H))&0xFF;
    // Send Instructions over Serial
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_GOAL_SP_LENGTH);
    MOTOR_PutChar(AX_WRITE_DATA);
    MOTOR_PutChar(AX_GOAL_POSITION_L);
    MOTOR_PutChar(Position_L);
    MOTOR_PutChar(Position_H);
    MOTOR_PutChar(Speed_L);
    MOTOR_PutChar(Speed_H);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    // Return the read error
    return MOTOR_ReadRxData();                
}

int moveRW(unsigned char ID, int Position){
    char Position_H,Position_L;
    //Shift register 16 bits
    Position_H = Position >> 8;           
    Position_L = Position;
    //Checksum for verification data to send
    Checksum = (~(ID + AX_GOAL_LENGTH + AX_REG_WRITE + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;
    // Send Instructions over Serial
    MOTOR_PutChar(AX_START); 
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_GOAL_LENGTH);
    MOTOR_PutChar(AX_REG_WRITE);
    MOTOR_PutChar(AX_GOAL_POSITION_L);
    MOTOR_PutChar(Position_L);
    MOTOR_PutChar(Position_H);
    MOTOR_PutChar(Checksum);
	CyDelayUs(TX_DELAY_TIME);
    // Return the read error
    return MOTOR_ReadRxData();                  
}

void action(){
	// Send Instructions over Serial
    MOTOR_PutChar(AX_START);                
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(BROADCAST_ID);
    MOTOR_PutChar(AX_ACTION_LENGTH);
    MOTOR_PutChar(AX_ACTION);
    MOTOR_PutChar(AX_ACTION_CHECKSUM);
	CyDelayUs(TX_DELAY_TIME);
}

int turn(unsigned char ID, int SIDE, int Speed){		
		if (SIDE == 0){                          
			//Left Move
			char Speed_H,Speed_L;
            //Shift register 16 bits
			Speed_H = Speed >> 8;
			Speed_L = Speed;        
            //Checksum for verification data to send
			Checksum = (~(ID + AX_SPEED_LENGTH + AX_WRITE_DATA + AX_GOAL_SPEED_L + Speed_L + Speed_H))&0xFF;
		    // Send Instructions over Serial
			MOTOR_PutChar(AX_START);                
			MOTOR_PutChar(AX_START);
			MOTOR_PutChar(ID);
			MOTOR_PutChar(AX_SPEED_LENGTH);
			MOTOR_PutChar(AX_WRITE_DATA);
			MOTOR_PutChar(AX_GOAL_SPEED_L);
			MOTOR_PutChar(Speed_L);
			MOTOR_PutChar(Speed_H);
			MOTOR_PutChar(Checksum);
			CyDelayUs(TX_DELAY_TIME);
			// Return the read error	
			return MOTOR_ReadRxData();                	
		}
		else
		{                                            
            // Rigth Move
			char Speed_H,Speed_L;
            //Shift register 16 bits
			Speed_H = (Speed >> 8) + 4;
			Speed_L = Speed;              
            //Checksum for verification data to send
			Checksum = (~(ID + AX_SPEED_LENGTH + AX_WRITE_DATA + AX_GOAL_SPEED_L + Speed_L + Speed_H))&0xFF;
			// Send Instructions over Serial
			MOTOR_PutChar(AX_START);                
			MOTOR_PutChar(AX_START);
			MOTOR_PutChar(ID);
			MOTOR_PutChar(AX_SPEED_LENGTH);
			MOTOR_PutChar(AX_WRITE_DATA);
			MOTOR_PutChar(AX_GOAL_SPEED_L);
			MOTOR_PutChar(Speed_L);
			MOTOR_PutChar(Speed_H);
			MOTOR_PutChar(Checksum);
			CyDelayUs(TX_DELAY_TIME);
			// Return the read error			
			return MOTOR_ReadRxData();                	
		}
}
