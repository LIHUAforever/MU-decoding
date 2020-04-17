#include "Handle_MU_Value.h"
#include <string.h>
float X_Angle,Y_Angle;
float Templature_Value;
extern char Usart2_i;

//
void Send_X_Angle(void)
{
	uint8_t Send_Commond[8] = {0x77,0x07,0x00,0x00,0x00,0x00,0x01,0x08};

	for(char i = 0;i < 8;i++)
	{
		USART_SendData(USART2,Send_Commond[i]);
		delay_ms(1);
	}
}

//
void Send_Y_Angle(void)
{
	uint8_t Send_Commond[] = {0x77,0x07,0x00,0x00,0x00,0x00,0x02,0x09};

	for(char i = 0;i < 8;i++)
	{
		USART_SendData(USART2,Send_Commond[i]); 
		delay_ms(1);
	}
}

//
void Send_XY_Angle(void)
{
	uint8_t Send_Commond[] = {0x77,0x07,0x00,0x00,0x00,0x00,0x04,0x0B};

	for(char i = 0;i < 8;i++)
	{
		USART_SendData(USART2,Send_Commond[i]); 
		delay_ms(1);
	}
}

void Send_Templature(void)
{
	uint8_t Send_Commond[] = {0x77,0x07,0x00,0x00,0x00,0x00,0x23,0x2a};
	for(char i = 0;i < 8;i++)
	{
		USART_SendData(USART2,Send_Commond[i]); 
		delay_ms(1);
	}
}

//
void SET_Zero(uint8_t Switch_flag)
{
	uint8_t Send_Relative_Commond[] = {0x77,0x08,0x00,0x00,0x00,0x00,0x05,0x01,0x0E};
	uint8_t Send_absolute_Commond[] = {0x77,0x08,0x00,0x00,0x00,0x00,0x05,0x00,0x0E};

	if(Switch_flag == 1)
	{
		for(char i = 0;i < 9;i++)
		{
			USART_SendData(USART2,Send_Relative_Commond[i]); 
			delay_ms(1);
		}
	}
  else
	{
		for(char i = 0;i < 9;i++)
		{
			USART_SendData(USART2,Send_absolute_Commond[i]); 
			delay_ms(1);
		}
	}		
}

void Query_Zero(void)
{
	uint8_t Send_Commond[] = {0x77,0x07,0x00,0x00,0x00,0x00,0x0D,0x14};

	for(char i = 0;i < 8;i++)
	{
		USART_SendData(USART2,Send_Commond[i]); 
		delay_ms(1);
	}
}
//
uint8_t BCD_Decimal(uint8_t bcd)
{
 uint8_t Decimal;
 Decimal=bcd>>4;
 return(Decimal=Decimal*10+(bcd&=0x0F));
}

//
float Read_Angle(uint8_t *buffer,uint8_t address)
{
	uint8_t Read_Value[5];
	float   Angle_Value;
	for(char i=0;i<4;i++)
	{
		Read_Value[i] = BCD_Decimal(buffer[i+address]);
	}
	Angle_Value = Read_Value[0]%10*100 + Read_Value[1] + (float)Read_Value[2]/100 + (float)Read_Value[3]/1000;
	if(Read_Value[0]/10 == 1)
	{
		Angle_Value = -Angle_Value;
	}
	return Angle_Value;
}

char SET_Zero_successful_Flag;
char Query_Zero_Flag;

//return £º1 : successful 
//         0 : error 
uint8_t Set_Zero_Response(uint8_t *buffer,uint8_t address)
{
	uint8_t Value = buffer[address];
	if(Value == 0x00)
	{
		return 1;
	}
	else return 0;
}

/*return 1 : Relative zero
				 0 : absolute zero
*/
uint8_t Query_Zero_Response(uint8_t *buffer,uint8_t address)
{
	uint8_t Value = buffer[address];
	if(Value == 0x00)
	{
		return 1;
	}
	else return 0;
}

u8 Handle_Data(uint8_t flag)
{
	uint8_t Read_T_Value[4];
	int Check_Buffer = 0;
	for(char Receive_i = 0;Receive_i < 25;Receive_i++)
	{
		if(Receive_i>20)
		{
			return 0;
		}
		else
		{
			if((Rx_Buffer[Receive_i] == 0x77)&&(flag == 1))
			{
				for(char angle_i = 1;angle_i<19;angle_i++)
				{
					Check_Buffer = Check_Buffer+Rx_Buffer[angle_i];
				}
				Check_Buffer = Check_Buffer&0x00ff;
				if(Check_Buffer == Rx_Buffer[Receive_i + 19])
				{
					switch(Rx_Buffer[Receive_i+6])
					{
						case Read_X_Angle_Temp:X_Angle = Read_Angle(Rx_Buffer,Receive_i+7);Usart2_i = 0;memset(Rx_Buffer,0,sizeof(Rx_Buffer));break;
						case Read_Y_Angle_Temp:Y_Angle = Read_Angle(Rx_Buffer,Receive_i+7);Usart2_i = 0;memset(Rx_Buffer,0,sizeof(Rx_Buffer));break;
						case Read_X_Y_Angle_Temp:X_Angle = Read_Angle(Rx_Buffer,Receive_i+7);Y_Angle = Read_Angle(Rx_Buffer,Receive_i+11);Usart2_i = 0;memset(Rx_Buffer,0,sizeof(Rx_Buffer));break;
						case SET_Zero_Temp:if(Set_Zero_Response(Rx_Buffer,Receive_i+7)){SET_Zero_successful_Flag = 1;}else{SET_Zero_successful_Flag = 1;}Usart2_i = 0;memset(Rx_Buffer,0,sizeof(Rx_Buffer));break;
						case Query_Zero_Temp:if(Query_Zero_Response(Rx_Buffer,Receive_i+7)){Query_Zero_Flag = 1;}else {Query_Zero_Flag = 0;}Usart2_i = 0;memset(Rx_Buffer,0,sizeof(Rx_Buffer));break;
						default:break; 
					}
					return 1;
			  }
				else
				{
					return 0;
				}
			}
			
			if(Rx_Buffer[Receive_i] == 0x77&&(flag == 0))
			{
				for(char i = 1;i<15;i++)
				{
					Check_Buffer = Check_Buffer+Rx_Buffer[i];
				}
				Check_Buffer = Check_Buffer&0x00ff;
				if(Check_Buffer == Rx_Buffer[Receive_i + 15])
				{
					if(Rx_Buffer[Receive_i + 6] == 0x23)
					{
						for(char i = 0;i<4;i++)
						{
							Read_T_Value[i] = BCD_Decimal(Rx_Buffer[i+Receive_i+7]);
						}
						Templature_Value = Read_T_Value[0]*100+Read_T_Value[1]+(float)Read_T_Value[2]/100+(float)Read_T_Value[3]/100;
						memset(Rx_Buffer,0,sizeof(Rx_Buffer));
						Usart2_i = 0;
				  }
					return 1;
				}
				else 
				{
				  return 0;
				}
	  	}
			return 0;
	}
 }
	return 0;
}


u8 Sequence_1 = 0;

void ReadMU_Value(void)
{
	if(Sequence_1 == 0)
	{
		delay_ms(100);
		Send_XY_Angle();
		delay_ms(100);
		if(Usart2_i >= 20)
		{
			Handle_Data(1);
			Sequence_1 = 1;
		}
	}
	if(Sequence_1 == 1)
	{
		delay_ms(100);
		Send_Templature();
		delay_ms(100);
		if(Usart2_i >= 16)
		{
			Handle_Data(0);
			Sequence_1 = 0;
		}
	}
}

float Read_Templature(void)
{
	float Error_Flag = 460;
	char Time_i = 0;
	delay_ms(30);
	Send_Templature();
	delay_ms(30);
	while(Handle_Data(0) == 0&&Time_i<10)
	{
		Usart2_i = 0;
		memset(Rx_Buffer,0,sizeof(Rx_Buffer));
		delay_ms(30);
		Send_XY_Angle();
		delay_ms(30);
		Time_i++;
	}
	if(Time_i == 10)return Error_Flag;
	return Templature_Value;
}

float READ_X_Angle(void)
{
	float Error_Flag = 460;
	float X_Angle_Temp = 0;
	char Time_i = 0;
	delay_ms(30);
	Send_XY_Angle();
	delay_ms(30);
	while(Handle_Data(1) == 0&&Time_i<10)
	{
		Usart2_i = 0;
		memset(Rx_Buffer,0,sizeof(Rx_Buffer));
		delay_ms(30);
		Send_XY_Angle();
		delay_ms(30);
		Time_i++;
	}
	if(Time_i == 10)return Error_Flag;
	X_Angle_Temp = X_Angle;
	return X_Angle_Temp;
}

float READ_Y_Angle(void)
{
	float Error_Flag = 460;
	float Y_Angle_Temp = 0;
	char Time_i = 0;
	delay_ms(30);
	Send_XY_Angle();
	delay_ms(30);
	while(Handle_Data(1) == 0&&Time_i<10)
	{
		Usart2_i = 0;
		memset(Rx_Buffer,0,sizeof(Rx_Buffer));
		delay_ms(30);
		Send_XY_Angle();
		delay_ms(30);
		Time_i++;
	}
	if(Time_i == 10)return Error_Flag;
	Y_Angle_Temp = Y_Angle;
	return Y_Angle_Temp;
}







