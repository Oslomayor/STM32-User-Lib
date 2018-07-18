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
* Author : ��
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
	GPIO_InitTypeDef GPIO_InitStrc;                      //�ṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //����RCCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStrc.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   //����
	GPIO_InitStrc.GPIO_Mode = GPIO_Mode_Out_PP;          //�������
	GPIO_InitStrc.GPIO_Speed = GPIO_Speed_50MHz;         //50MHz
	GPIO_Init(GPIOA,&GPIO_InitStrc);                     //��ʼ��
	
	GPIO_InitStrc.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStrc);
	
	
	//GPIO_SetBits(GPIOA,GPIO_Pin_All);                    //��1
	GPIO_ResetBits(GPIOA,GPIO_Pin_All);                    //��0
	GPIO_ResetBits(GPIOB,GPIO_Pin_All);
}


//�ر�LED1��LED2��  LED_OFF(LED1 | LED2) 
void LED_OFF(unsigned char arg)
{
	// ȡ��4λ�ж�
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
	// arg �ĵ���λȡ�෴��������LED_state���븳��LED_state
	LED_state &= ~(arg&0x0F);    

}

//����LED1��LED2��  LED_ON(LED1 | LED2) 
void LED_ON(unsigned char arg)
{
	// ȡ��4λ�ж�
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
	// 0x0F ���������״̬
	// LED_state = 0x0F; ��һ���в��ײ����BUG����ʵ���ϸ�LEN_ON()�������һ�г�ͻ
	// ��Ϊʵ���Ϻ�����ִ�е�ʱ��ռ��ʱ��ǳ��࣬��λ������������ LED_state = 0x0F ��Ч��
	LED_state = 0x0F; 
	// ��һ�н����һ�е�BUG,ֱ�Ӱ� LED ��״̬�͸����ڻ�����
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
	// �� LED ��״̬�͸����ڻ�����
	usart1_send_buff[2] = LED_state;
	OLED_DisplayLED();
	
}

void LED_Flow(u16 acd_value, u8 mode)
{
	unsigned int time;
	// 0x1F ������ˮ��
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
		default : // ����123��Ĭ��ADC��������
	}
	// ADC ��������
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







