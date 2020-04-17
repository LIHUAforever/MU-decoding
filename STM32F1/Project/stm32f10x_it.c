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
//extern u16 Row_color[320];   //转换像素值
u8 ov_frame; 	//统计帧数
//定时器1触发中断服务程序	 
void TIM1_TRG_COM_IRQHandler(void)
{ 	
	TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);  //清除TIMx的中断待处理位:TIM 中断源  
}
 //外部中断3服务程序  即行同步信号HREF中断
void EXTI3_IRQHandler(void)
{		 	
	u16 j;
	u32 *p;
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)//是3线的中断
	{    
    MYDMA_Enable(DMA1_Channel4);			//TIM3的UPDATE对应DMA的通道3   开始一次DMA传输
		p=(u32 *)(0x22000000+(((u32)Row &0xFFFFF)<<5)+0*4);    //用位带操作 这样应该能够更加的节省时间
		for(j=0;j<320;j++)
		{
			if(*p) LCD->LCD_RAM=BLACK;
			else LCD->LCD_RAM=WHITE;
			p++;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除EXTI6线路挂起位						  
}
 //外部中断4服务程序  即场同步信号HREF中断
void EXTI4_IRQHandler(void)
{		 	
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)//是3线的中断
	{    
		  LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右 
			LCD_SetCursor(0x00,0x00);	//设置光标位置 
			LCD_WriteRAM_Prepare();     //开始写入GRAM	
	  	MYDMA_Enable(DMA1_Channel4);  //为了采样第一行数据
		  LED0=!LED0;
		  ov_frame ++;   //统计帧率
	}
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI6线路挂起位						  
}
//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
			LED1=!LED1;
			printf("frame:%dfps\r\n",ov_frame);	//打印帧率
			ov_frame=0;	
	} 
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源   
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
