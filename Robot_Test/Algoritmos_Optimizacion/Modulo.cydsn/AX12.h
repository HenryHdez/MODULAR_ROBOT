#include <DynamixelSerial1.h>
#include <project.h>

//////actualizado 2.0     <---------------------------------------------
unsigned char Checksum;
unsigned char Incoming_Byte,Temperature_Byte,Error_Byte,Incoming_Byte2,Position_Long_Byte, Position_High_Byte,Position_Low_Byte;
int Time_Counter;
//The functions are based on the arduino library


int error_data (){
    Time_Counter=0;
    CyDelay(20); ///// delay para recibir
    while((RX_GetRxBufferSize() < 6) & (Time_Counter==TIME_OUT))
    {Time_Counter++;
    CyDelayUs(1000);
        }
    while (RX_GetRxBufferSize() > 0){ 
        Incoming_Byte =    RX_GetChar();
        Incoming_Byte2 =    RX_GetChar();
		if ( (Incoming_Byte == 255) & (Incoming_Byte2 == 255) ){
            RX_GetChar();    //ID motor
            RX_GetChar();   // Length                                                               
	        Error_Byte =   RX_GetChar();   // Error                    
		    RX_GetChar(); ///check
	        RX_ClearRxBuffer(); //limpia buffer 
            CyDelay(10);
            return (Error_Byte);        
		} 
   }
    return (Error_Byte);  //ax no responde
}


int readtemperature(unsigned char ID){
    Checksum = (~(ID + AX_TEM_LENGTH  + AX_READ_DATA + AX_PRESENT_TEMPERATURE + AX_BYTE_READ))&0xFF;
    TXC_Write(2); ////habilita tx  2 
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_TEM_LENGTH);
    MOTOR_PutChar(AX_READ_DATA);
    MOTOR_PutChar(AX_PRESENT_TEMPERATURE);
    MOTOR_PutChar(AX_BYTE_READ);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1 
    Time_Counter=0;
    CyDelay(20);/// delay para recibir
    while((RX_GetRxBufferSize() < 6) & (Time_Counter==TIME_OUT))
    {
        Time_Counter++;
        CyDelayUs(1000);    
    }
    while (RX_GetRxBufferSize() > 0){
        Incoming_Byte = RX_ReadRxData();
        Incoming_Byte2= RX_ReadRxData();
		if ( (Incoming_Byte == 255) & (Incoming_Byte2  == 255) ){
	        RX_ReadRxData();       // Ax-12 ID
			RX_ReadRxData();       // Length
	    	RX_ReadRxData();       //error
        	Temperature_Byte= 	RX_ReadRxData(); ///dato
	        RX_ReadRxData();            //cheaksum           
            RX_ClearRxBuffer(); //limpia buffer
            CyDelay(10);
            return (Temperature_Byte);
		}
   }
return (Temperature_Byte);
}

int setID (unsigned char ID,unsigned char newID){
    Checksum = (~(ID + AX_ID_LENGTH + AX_WRITE_DATA + AX_ID + newID))&0xFF;
    TXC_Write(2); ////habilita tx
    MOTOR_PutChar(AX_START);              
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_ID_LENGTH);
    MOTOR_PutChar(AX_WRITE_DATA);
    MOTOR_PutChar(AX_ID);
    MOTOR_PutChar(newID);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1
    return (error_data()); 
}

int ping (unsigned char ID){
    Checksum = (~(ID + AX_READ_DATA + AX_PING))&0xFF;
    TXC_Write(2); ////habilita tx
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_READ_DATA);
    MOTOR_PutChar(AX_PING);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1
    return(error_data()) ; 
}

int reset (unsigned char ID ){
    Checksum = (~(ID + AX_RESET_LENGTH + AX_RESET))&0xFF;
    TXC_Write(2); ////habilita tx
    MOTOR_PutChar(AX_START);              
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_RESET_LENGTH);	
    MOTOR_PutChar(AX_RESET);
    MOTOR_PutChar(Checksum);	
    CyDelayUs(TX_DELAY_TIME);
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1
    return (error_data()); 
}

int LED(unsigned char ID, int Status){    
    //Checksum for verification data to send
    Checksum = (~(ID + AX_LED_LENGTH + AX_WRITE_DATA + AX_LED + Status))&0xFF;
    // Send Instructions over Serial
    TXC_Write(2); ////habilita tx
    MOTOR_PutChar(AX_START);              
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_LED_LENGTH);
    MOTOR_PutChar(AX_WRITE_DATA);
    MOTOR_PutChar(AX_LED);
    MOTOR_PutChar(Status);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1);
    //Finish Function
    return(error_data());
}

int Move(unsigned char ID, int Position){
    char Position_H,Position_L;
    //Shift register 16 bits
    Position_H = Position >> 8;           
    Position_L = Position;
	Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;
    // Send Instructions over Serial
    TXC_Write(2); ////habilita tx
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
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1
    return (error_data());                 
}

int setEndless(unsigned char ID, int Status,int rag){
    if (Status){	
        // Changing the CCW Angle Limits for Full Rotation.
        Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L))&0xFF;
        // Send Instructions over Serial
        //Checksum for verification data to send
        TXC_Write(2); ////habilita tx
        MOTOR_PutChar(AX_START);                
        MOTOR_PutChar(AX_START);
        MOTOR_PutChar(ID);
        MOTOR_PutChar(AX_GOAL_LENGTH);
        MOTOR_PutChar(AX_WRITE_DATA);
        MOTOR_PutChar(AX_CCW_ANGLE_LIMIT_L );
        MOTOR_PutChar(Checksum);
        CyDelayUs(TX_DELAY_TIME);
        CyDelay(5);  /// cambio de estado delay para buffer de salida
        TXC_Write(1); ///habilita rx 1
    	return (error_data()); 
    }
    else{
        Checksum = (~(ID + AX_GOAL_LENGTH + AX_WRITE_DATA + AX_CCW_ANGLE_LIMIT_L + AX_CCW_AL_L + AX_CCW_AL_H))&0xFF;
	    // Send Instructions over Serial
        //Checksum for verification data to send
        TXC_Write(2); ////habilita tx
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
        CyDelay(5);  /// cambio de estado delay para buffer de salida
        TXC_Write(1); ///habilita rx 1
    	return (error_data());                 
    }
} 

int torqueStatus( unsigned char ID, int Status){
    Checksum = (~(ID + AX_TORQUE_LENGTH + AX_WRITE_DATA + AX_TORQUE_ENABLE + Status))&0xFF;
    // Send Instructions over Serial
    //Checksum for verification data to send
    TXC_Write(2); ////habilita tx
    MOTOR_PutChar(AX_START); 
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_TORQUE_LENGTH);
    MOTOR_PutChar(AX_WRITE_DATA);
    MOTOR_PutChar(AX_TORQUE_ENABLE);
    MOTOR_PutChar(Status);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1
    // Return the read error
    return (error_data());               
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
    TXC_Write(2); ////habilita tx
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
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1
    return (error_data());                
}

int moveRW(unsigned char ID, int Position){
    char Position_H,Position_L;
    //Shift register 16 bits
    Position_H = Position >> 8;           
    Position_L = Position;
    //Checksum for verification data to send
    Checksum = (~(ID + AX_GOAL_LENGTH + AX_REG_WRITE + AX_GOAL_POSITION_L + Position_L + Position_H))&0xFF;
    // Send Instructions over Serial
    TXC_Write(2); ////habilita tx
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
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1
    return (error_data());                  
}

void action(){
	// Send Instructions over Serial
    TXC_Write(2); ////habilita tx
    MOTOR_PutChar(AX_START);                
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(BROADCAST_ID);
    MOTOR_PutChar(AX_ACTION_LENGTH);
    MOTOR_PutChar(AX_ACTION);
    MOTOR_PutChar(AX_ACTION_CHECKSUM);
	CyDelayUs(TX_DELAY_TIME);
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1
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
        TXC_Write(2); ////habilita tx
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
        CyDelay(5);  /// cambio de estado delay para buffer de salida
        TXC_Write(1); ///habilita rx 1
		return (error_data()); 
        }
		else{ 
            // Rigth Move
			char Speed_H,Speed_L;
            //Shift register 16 bits
			Speed_H = (Speed >> 8) + 4;
			Speed_L = Speed;              
            //Checksum for verification data to send
			Checksum = (~(ID + AX_SPEED_LENGTH + AX_WRITE_DATA + AX_GOAL_SPEED_L + Speed_L + Speed_H))&0xFF;
			// Send Instructions over Serial
            TXC_Write(2); ////habilita tx
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
            CyDelay(5);  /// cambio de estado delay para buffer de salida
            TXC_Write(1); ///habilita rx 1	
			return (error_data());                	
		}
}

int readPosition (unsigned char ID){
    int data=0;
    RX_ClearRxBuffer();
    Checksum = (~(ID + AX_POS_LENGTH  + AX_READ_DATA + AX_PRESENT_POSITION_L + AX_BYTE_READ_POS))&0xFF;
    TXC_Write(2); ////habilita tx
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
    MOTOR_PutChar(AX_POS_LENGTH);
    MOTOR_PutChar(AX_READ_DATA);
    MOTOR_PutChar(AX_PRESENT_POSITION_L);
    MOTOR_PutChar(AX_BYTE_READ_POS);
    MOTOR_PutChar(Checksum);
    CyDelayUs(TX_DELAY_TIME);
    CyDelay(5);  /// cambio de estado delay para buffer de salida
    TXC_Write(1); ///habilita rx 1	   
    Position_Long_Byte = -1;
    Time_Counter = 0;
    CyDelay(20);/// delay de espera 
    while((RX_GetRxBufferSize() < 7) & (Time_Counter < TIME_OUT)){
		Time_Counter++;
		CyDelayUs(1000);
    }
    while (RX_GetRxBufferSize() > 0){
		Incoming_Byte = RX_ReadRxData();
        Incoming_Byte2 = RX_ReadRxData();
		if ( (Incoming_Byte == 255) & (Incoming_Byte2 == 255) ){                         
			RX_ReadRxData();                            // Ax-12 ID
			RX_ReadRxData();                            // Length
			Error_Byte = RX_ReadRxData();    //error
   			Position_Low_Byte = RX_ReadRxData();            // Position Bytes
			Position_High_Byte = RX_ReadRxData();           // Position Bytes  
            RX_ReadRxData();
			data = Position_High_Byte << 8; 
			data = data + Position_Low_Byte;
		    RX_ClearRxBuffer();
            CyDelay(10);
        }    
    }
	return (data);     // Returns the read position   
}

int setAngleLimit(unsigned char ID, int CWLimit, int CCWLimit){
	char CW_H,CW_L,CCW_H,CCW_L;
    CW_H = CWLimit >> 8;    
    CW_L = CWLimit;                // 16 bits - 2 x 8 bits variables
    CCW_H = CCWLimit >> 8;
    CCW_L = CCWLimit;  
	Checksum = (~(ID + AX_VL_LENGTH +AX_WRITE_DATA+ AX_CW_ANGLE_LIMIT_L + CW_H + CW_L + AX_CCW_ANGLE_LIMIT_L + CCW_H + CCW_L))&0xFF;
    TXC_Write(2); ////habilita tx	 
    MOTOR_PutChar(AX_START);
	MOTOR_PutChar(AX_START);
    MOTOR_PutChar(ID);
	MOTOR_PutChar(AX_CCW_CW_LENGTH);
	MOTOR_PutChar(AX_WRITE_DATA);
	MOTOR_PutChar(AX_CW_ANGLE_LIMIT_L);
    MOTOR_PutChar(CW_L);
    MOTOR_PutChar(CW_H);
	MOTOR_PutChar(AX_CCW_ANGLE_LIMIT_L);
    MOTOR_PutChar(CCW_L);
	MOTOR_PutChar(CCW_H);
	MOTOR_PutChar(Checksum);
	CyDelayUs(TX_DELAY_TIME);
	CyDelay(5);
	TXC_Write(1); ////habilita rx	 
    return (error_data()); 
}
