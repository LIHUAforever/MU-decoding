#include "Init.h"
char i = 1;
float T;

void init(void)
{
	delay_init(72);
	Usart2_Init(9600);
	while(1)
	{
		if(i==1)
		{
			i = 0;
	  	T = READ_X_Angle();
		}
	}
}





