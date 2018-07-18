/************************************************************************************
*  Copyright (c), 2018, oslomayor technology
*            All rights reserved.
*
* Wechat Offical Account: crazyengineer
* github  : https://github.com/Oslomayor

* File name: Key.c
* Processor: STM32F103C8T6
* Compiler : MDK for ARM
* 
* Author : 镇长 
* Version: 1.00
* Date   : 2018.6.6
* Pins   :
*          PA4  -> Key1
*          PA5  -> Key2
*          PA8  -> Key3
*          PA15 -> Key4
*************************************************************************************/

#include "key.h"
#include "led.h"
#include "globe.h"

u16 Key_LongPress = 0;
u16 Key_LongPress_Cache = 0;

void Key_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //声明一个结构体变量，用来初始化GPIO

	// SystemInit();
	/* 开启GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_15;	   //设置的IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                        //上拉输入  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                   //设置传输速率
	GPIO_Init(GPIOA,&GPIO_InitStructure);		                                             // 初始化GPIO 

}


void Exit_init()
{
  EXTI_InitTypeDef EXTI_InitStructure;                 //EXTI结构体
  NVIC_InitTypeDef NVIC_InitStructure;                 //NVIC结构体
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //复用功能时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4); //中断线路4
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5); //中断线路5
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); //中断线路8
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); //中断线路15
	/* Key1  GPIOA4 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
  EXTI_Init(&EXTI_InitStructure);                      //初始化
	/* Key2  GPIOA5 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
  EXTI_Init(&EXTI_InitStructure);                      //初始化
	/* Key3  GPIOA8 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
  EXTI_Init(&EXTI_InitStructure);                      //初始化
	/* Key4  GPIOA15 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
  EXTI_Init(&EXTI_InitStructure);                      //初始化
	/* 设置NVIC是为了设置中断优先级和打开总中断 */ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //抢占优先级和响应优先级分组
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;          //EXTI4 全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级 = 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应优先级 = 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能
	NVIC_Init(&NVIC_InitStructure);                           //初始化
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //抢占优先级和响应优先级分组
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;          //EXTI1 全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级 = 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应优先级 = 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能
	NVIC_Init(&NVIC_InitStructure);                           //初始化
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //抢占优先级和响应优先级分组
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;          //EXTI3 全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级 = 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应优先级 = 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能
	NVIC_Init(&NVIC_InitStructure);                           //初始化
	
}

/* K1 按键中断 */
void EXTI4_IRQHandler() //K1, PA4 中断函数
{
  if(EXTI_GetITStatus(EXTI_Line4)==SET)                //查询标志
  {
    EXTI_ClearITPendingBit(EXTI_Line4);                //清除标志
		
		/**************************** Key1 code here *****************************/
		
		Key_LongPress = 0;//长按计数清零
		Key_LongPress_Cache = 0;
    while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0&&Key_LongPress<=65530)
		{
			Key_LongPress_Cache++;
			if(Key_LongPress_Cache >= 65530) 
			{
				Key_LongPress_Cache = 0;
				if(Key_LongPress <= 65530) Key_LongPress++;
			}
		}
		// Key_LongPress 大于20，表示按下超过2秒
		// Key_state = 0xFF 代表长按
		// Key_state = 0xF1 代表短按
		if(Key_LongPress >= 20) Key_state = 0xFF; else Key_state = 0xF1;
		
		// 长按
		if(Key_state == 0xFF)  
		{
			// 暂时仅有3种模式
			if(current_state < 3) 
			{
				// 任何情况长按进入下一个状态
				current_state++;
				
				/* 删掉了switch */
			}
			else current_state = 0;
		}
		
		// 短按
		if(Key_state == 0xF1)
		{
			// 短按则根据当前状态执行下一步
			switch(current_state)
			{
				case STA_NORMAL : break;
				case STA_LED_BREATHE: break;
				case STA_LED_FLOW: break;
				case STA_LED_KEY_CONTROL: if((LED_state&0x01) == 0) LED_state |= LED1; else LED_state &= (0xFF-LED1+0xF0);break;
				default : ;
			}
		}
  }
	// Key1的状态送给串口缓冲区
	usart1_send_buff[13] = 'D';
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0);
	/**************************** Key1 code above *****************************/
}

/* K2 和 K3 按键中断 */
void EXTI9_5_IRQHandler() //Key2,Key3, PA5,PA8 中断函数
{
	/* K2 按键中断 */
	if(EXTI_GetITStatus(EXTI_Line5)==SET)                //查询标志
  {
    EXTI_ClearITPendingBit(EXTI_Line5);                //清除标志
		
		/* Key2 code here */
		Key_state = 0xF2;
		switch(current_state)
		{
			case STA_NORMAL : break;
			case STA_LED_BREATHE: break;
			case STA_LED_FLOW: break;
			case STA_LED_KEY_CONTROL: if((LED_state&0x02) == 0) LED_state |= LED2; else LED_state &= (0xFF-LED2+0xF0);break;
			default : ;
		}
  }
	// Key2的状态送给串口缓冲区
	usart1_send_buff[14] = 'D';
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0);
	
	/* K3 按键中断 */
	if(EXTI_GetITStatus(EXTI_Line8)==SET)                //查询标志
  {
    EXTI_ClearITPendingBit(EXTI_Line8);                //清除标志

    /* Key3 code here */
		Key_state = 0xF4;
		switch(current_state)
		{
			case STA_NORMAL : break;
			case STA_LED_BREATHE: break;
			case STA_LED_FLOW: break;
			case STA_LED_KEY_CONTROL: if((LED_state&0x04) == 0) LED_state |= LED3; else LED_state &= (0xFF-LED3+0xF0);break;
			default : ;
		}
  }
	// Key3的状态送给串口缓冲区
	usart1_send_buff[15] = 'D';
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0);
}

/* K4 按键中断 */
void EXTI15_10_IRQHandler() //Key4, PA15 中断函数
{
	if(EXTI_GetITStatus(EXTI_Line15)==SET)                //查询标志
  {
    EXTI_ClearITPendingBit(EXTI_Line15);                //清除标志
		
	  /* Key4 code here */
		Key_state = 0xF8;
		switch(current_state)
		{
			case STA_NORMAL : break;
			case STA_LED_BREATHE: break;
			case STA_LED_FLOW: break;
			case STA_LED_KEY_CONTROL: if((LED_state&0x08) == 0) LED_state |= LED4; else LED_state &= (0xFF-LED4+0xF0);break;
			default : ;
		}
  }
	usart1_send_buff[16] = 'D';
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0);
}

