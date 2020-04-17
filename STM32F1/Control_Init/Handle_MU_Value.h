#ifndef __HANDLE_MU_VALUE_H_
#define __HANDLE_MU_VALUE_H_

#include "Init.h"

#define Read_X_Angle_Temp   0x81
#define Read_Y_Angle_Temp   0x82
#define Read_X_Y_Angle_Temp 0x84
#define SET_Zero_Temp       0x85
#define Query_Zero_Temp     0x8D

void Send_X_Angle(void);
void Send_Y_Angle(void);
void Send_XY_Angle(void);
void Send_Templature(void);
void SET_Zero(uint8_t Switch_flag);
void Query_Zero(void);
u8 Handle_Data(uint8_t flag);
uint8_t BCD_Decimal(uint8_t bcd);
float Read_Angle(uint8_t *buffer,uint8_t address);
uint8_t Set_Zero_Response(uint8_t *buffer,uint8_t address);
uint8_t Query_Zero_Response(uint8_t *buffer,uint8_t address);
void ReadMU_Value(void);

float Read_Templature(void);
float READ_X_Angle(void);
float READ_Y_Angle(void);







#endif


