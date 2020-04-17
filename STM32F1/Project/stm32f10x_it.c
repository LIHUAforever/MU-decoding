/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "led.h"
#include "usart.h"	 
#include "dma.h"
#include "stm32f10x_dma.h"
#include "lcd.h"
 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}
extern u8 Row[40];
//extern u16 Row_color[320];   //ת������ֵ
u8 ov_frame; 	//ͳ��֡��
//��ʱ��1�����жϷ������	 
void TIM1_TRG_COM_IRQHandler(void)
{ 	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);  //���TIMx���жϴ�����λ:TIM �ж�Դ  
}
 //�ⲿ�ж�3�������  ����ͬ���ź�HREF�ж�
void EXTI3_IRQHandler(void)
{		 	
	u16 j;
	u32 *p;
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)//��3�ߵ��ж�
	{    
    MYDMA_Enable(DMA1_Channel4);			//TIM3��UPDATE��ӦDMA��ͨ��3   ��ʼһ��DMA����
		p=(u32 *)(0x22000000+(((u32)Row &0xFFFFF)<<5)+0*4);    //��λ������ ����Ӧ���ܹ����ӵĽ�ʡʱ��
		for(j=0;j<320;j++)
		{
			if(*p) LCD->LCD_RAM=BLACK;
			else LCD->LCD_RAM=WHITE;
			p++;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);  //���EXTI6��·����λ						  
}
 //�ⲿ�ж�4�������  ����ͬ���ź�HREF�ж�
void EXTI4_IRQHandler(void)
{		 	
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)//��3�ߵ��ж�
	{    
		  LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������ 
			LCD_SetCursor(0x00,0x00);	//���ù��λ�� 
			LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
	  	MYDMA_Enable(DMA1_Channel4);  //Ϊ�˲�����һ������
		  LED0=!LED0;
		  ov_frame ++;   //ͳ��֡��
	}
	EXTI_ClearITPendingBit(EXTI_Line4);  //���EXTI6��·����λ						  
}
//��ʱ��6�жϷ������	 
void TIM6_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
			LED1=!LED1;
			printf("frame:%dfps\r\n",ov_frame);	//��ӡ֡��
			ov_frame=0;	
	} 
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ   
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
