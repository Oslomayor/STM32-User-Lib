/************************************************************************************
*  Copyright (c), 2018, oslomayor technology
*            All rights reserved.
*
* Wechat Offical Account: crazyengineer
* github  : https://github.com/Oslomayor

* File name: led.c
* Processor: STM32F103C8T6
* Compiler : MDK for ARM
* 
* Author : 镇长
* Version: 1.00
* Date   : 2018.6.6
* Pins   :
*          PA6 -> LED1
*          PA7 -> LED2
*          PB0 -> LED3
*          PB1 -> LED4
*************************************************************************************/
#include "led.h"
#include "Key.h"
#include "OLED_I2C.h"
#include "globe.h"

void delay(u32 i)  //delay(6000000) = 1 s
{
	while(i--);
}
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStrc;                      //结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //开启RCC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStrc.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   //引脚
	GPIO_InitStrc.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
	GPIO_InitStrc.GPIO_Speed = GPIO_Speed_50MHz;         //50MHz
	GPIO_Init(GPIOA,&GPIO_InitStrc);                     //初始化
	
	GPIO_InitStrc.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStrc);
	
	
	//GPIO_SetBits(GPIOA,GPIO_Pin_All);                    //置1
	GPIO_ResetBits(GPIOA,GPIO_Pin_All);                    //置0
	GPIO_ResetBits(GPIOB,GPIO_Pin_All);
}


//关闭LED1和LED2，  LED_OFF(LED1 | LED2) 
void LED_OFF(unsigned char arg)
{
	// 取低4位判断
	switch(arg&0x0F)
	{
		case 1: GPIO_SetBits(GPIOA,GPIO_Pin_6);break;
		case 2: GPIO_SetBits(GPIOA,GPIO_Pin_7);break;
		case 3: GPIO_SetBits(GPIOA,GPIO_Pin_7|GPIO_Pin_6);break;
		case 4: GPIO_SetBits(GPIOB,GPIO_Pin_0);break;
		case 5: GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_0);break;
		case 6: GPIO_SetBits(GPIOA,GPIO_Pin_7);GPIO_SetBits(GPIOB,GPIO_Pin_0);break;
		case 7: GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_SetBits(GPIOA,GPIO_Pin_7);GPIO_SetBits(GPIOB,GPIO_Pin_0);break;
		case 8: GPIO_SetBits(GPIOB,GPIO_Pin_1);break;
		case 9: GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_1);break;
		case 10: GPIO_SetBits(GPIOA,GPIO_Pin_7);GPIO_SetBits(GPIOB,GPIO_Pin_1);break;
		case 11: GPIO_SetBits(GPIOA,GPIO_Pin_7|GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_1);break;
		case 12: GPIO_SetBits(GPIOB,GPIO_Pin_0);GPIO_SetBits(GPIOB,GPIO_Pin_1);break;
		case 13: GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_0);GPIO_SetBits(GPIOB,GPIO_Pin_1);break;
		case 14: GPIO_SetBits(GPIOA,GPIO_Pin_7);GPIO_SetBits(GPIOB,GPIO_Pin_0);GPIO_SetBits(GPIOB,GPIO_Pin_1);break;
		case 15: GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_SetBits(GPIOA,GPIO_Pin_7);GPIO_SetBits(GPIOB,GPIO_Pin_0);GPIO_SetBits(GPIOB,GPIO_Pin_1);break;
	}
	// arg 的低四位取相反数，再与LED_state相与赋给LED_state
	LED_state &= ~(arg&0x0F);    

}

//开启LED1和LED2，  LED_ON(LED1 | LED2) 
void LED_ON(unsigned char arg)
{
	// 取低4位判断
	switch(arg&0x0F)
	{
		case 1: GPIO_ResetBits(GPIOA,GPIO_Pin_6);break;
		case 2: GPIO_ResetBits(GPIOA,GPIO_Pin_7);break;
		case 3: GPIO_ResetBits(GPIOA,GPIO_Pin_7|GPIO_Pin_6);break;
		case 4: GPIO_ResetBits(GPIOB,GPIO_Pin_0);break;
		case 5: GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_0);break;
		case 6: GPIO_ResetBits(GPIOA,GPIO_Pin_7);GPIO_ResetBits(GPIOB,GPIO_Pin_0);break;
		case 7: GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOA,GPIO_Pin_7);GPIO_ResetBits(GPIOB,GPIO_Pin_0);break;
		case 8: GPIO_ResetBits(GPIOB,GPIO_Pin_1);break;
		case 9: GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_1);break;
		case 10: GPIO_ResetBits(GPIOA,GPIO_Pin_7);GPIO_ResetBits(GPIOB,GPIO_Pin_1);break;
		case 11: GPIO_ResetBits(GPIOA,GPIO_Pin_7|GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_1);break;
		case 12: GPIO_ResetBits(GPIOB,GPIO_Pin_0);GPIO_ResetBits(GPIOB,GPIO_Pin_1);break;
		case 13: GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_0);GPIO_ResetBits(GPIOB,GPIO_Pin_1);break;
		case 14: GPIO_ResetBits(GPIOA,GPIO_Pin_7);GPIO_ResetBits(GPIOB,GPIO_Pin_0);GPIO_ResetBits(GPIOB,GPIO_Pin_1);break;
		case 15: GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOA,GPIO_Pin_7);GPIO_ResetBits(GPIOB,GPIO_Pin_0);GPIO_ResetBits(GPIOB,GPIO_Pin_1);break;
	}    
	LED_state |= arg;

}


//void LED_Blink()
//{
//	LED_ON();
//	delay(3000000);
//	LED_OFF();
//	delay(3000000);
//}

void LED_Breathe(u16 adc_value)
{
	u8 i;
	u16 j;
	u16 time = (adc_value>>4)+100;
	// 0x0F 代表呼吸灯状态
	// LED_state = 0x0F; 这一行有不易察觉的BUG，它实际上跟LEN_ON()函数最后一行冲突
	// 因为实际上呼吸灯执行的时间占用时间非常多，上位机根本看不到 LED_state = 0x0F 的效果
	LED_state = 0x0F; 
	// 这一行解决上一行的BUG,直接把 LED 的状态送给串口缓存区
	usart1_send_buff[2] = 0x0F;
	OLED_DisplayLED();
	i=200;
	for(;i>0;i--)
	{
		j=time;
		for(;j>0;j--)
		{
			LED_ON(LED1|LED2|LED3|LED4);
			delay(i);
			LED_OFF(LED1|LED2|LED3|LED4);
			delay(200-i);
		}
	}
	i=200;
	for(;i>0;i--)
	{
		j=time;
		for(;j>0;j--)
		{
			LED_OFF(LED1|LED2|LED3|LED4);
			delay(i);
			LED_ON(LED1|LED2|LED3|LED4);
			delay(200-i);
		}
	}
}

void LED_Key_Control()
{
	
	if((LED_state&LED1) == LED1) LED_ON(LED1); else LED_OFF(LED1);
	if((LED_state&LED2) == LED2) LED_ON(LED2); else LED_OFF(LED2);
	if((LED_state&LED3) == LED3) LED_ON(LED3); else LED_OFF(LED3);
	if((LED_state&LED4) == LED4) LED_ON(LED4); else LED_OFF(LED4);
	// 把 LED 的状态送给串口缓存区
	usart1_send_buff[2] = LED_state;
	OLED_DisplayLED();
	
}

void LED_Flow(u16 acd_value, u8 mode)
{
	unsigned int time;
	// 0x1F 代表流水灯
	usart1_send_buff[2] = 0x1F;
	LED_OFF(LED1|LED2|LED3|LED4);
	switch(mode)
	{
		case '1': LED_ON(LED1);OLED_DisplayLED();DelayMs(200);LED_OFF(LED1);OLED_DisplayLED();DelayMs(200); 
						LED_ON(LED2);OLED_DisplayLED();DelayMs(200);LED_OFF(LED2);OLED_DisplayLED();DelayMs(200); 
						LED_ON(LED3);OLED_DisplayLED();DelayMs(200);LED_OFF(LED3);OLED_DisplayLED();DelayMs(200); 
						LED_ON(LED4);OLED_DisplayLED();DelayMs(200);LED_OFF(LED4);OLED_DisplayLED();DelayMs(200);
		        return;	
		case '2': LED_ON(LED1|LED2);OLED_DisplayLED();DelayMs(200);LED_OFF(LED1|LED2);OLED_DisplayLED();DelayMs(200);
						LED_ON(LED3|LED4);OLED_DisplayLED();DelayMs(200);LED_OFF(LED3|LED4);OLED_DisplayLED();DelayMs(200);
		        return;
		case '3': LED_ON(LED1|LED2);OLED_DisplayLED();DelayMs(200);LED_OFF(LED1|LED2);OLED_DisplayLED();DelayMs(200);
						LED_ON(LED2|LED3);OLED_DisplayLED();DelayMs(200);LED_OFF(LED2|LED3);OLED_DisplayLED();DelayMs(200);
	          LED_ON(LED3|LED4);OLED_DisplayLED();DelayMs(200);LED_OFF(LED3|LED4);OLED_DisplayLED();DelayMs(200);
						LED_ON(LED4|LED1);OLED_DisplayLED();DelayMs(200);LED_OFF(LED4|LED1);OLED_DisplayLED();DelayMs(200);
		        return;
		default : // 除了123，默认ADC控制流速
	}
	// ADC 控制流速
	time = (adc_value>>3)+20;
	LED_ON(LED1);OLED_DisplayLED();	
	DelayMs(time);
	LED_OFF(LED1);OLED_DisplayLED();	
	
	LED_ON(LED2);OLED_DisplayLED();	
	DelayMs(time);
	LED_OFF(LED2);OLED_DisplayLED();	
	
	LED_ON(LED3);OLED_DisplayLED();	
	DelayMs(time);
	LED_OFF(LED3);OLED_DisplayLED();	
	
	LED_ON(LED4);OLED_DisplayLED();	
	DelayMs(time);
	LED_OFF(LED4);OLED_DisplayLED();	
	
	DelayMs(time);
}







