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
* Author : �� 
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
	GPIO_InitTypeDef GPIO_InitStructure;	  //����һ���ṹ�������������ʼ��GPIO

	// SystemInit();
	/* ����GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_15;	   //���õ�IO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                        //��������  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                   //���ô�������
	GPIO_Init(GPIOA,&GPIO_InitStructure);		                                             // ��ʼ��GPIO 

}


void Exit_init()
{
  EXTI_InitTypeDef EXTI_InitStructure;                 //EXTI�ṹ��
  NVIC_InitTypeDef NVIC_InitStructure;                 //NVIC�ṹ��
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //���ù���ʱ��
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4); //�ж���·4
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5); //�ж���·5
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); //�ж���·8
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15); //�ж���·15
	/* Key1  GPIOA4 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
  EXTI_Init(&EXTI_InitStructure);                      //��ʼ��
	/* Key2  GPIOA5 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
  EXTI_Init(&EXTI_InitStructure);                      //��ʼ��
	/* Key3  GPIOA8 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
  EXTI_Init(&EXTI_InitStructure);                      //��ʼ��
	/* Key4  GPIOA15 */
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;            
  EXTI_Init(&EXTI_InitStructure);                      //��ʼ��
	/* ����NVIC��Ϊ�������ж����ȼ��ʹ����ж� */ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //��ռ���ȼ�����Ӧ���ȼ�����
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;          //EXTI4 ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� = 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧ���ȼ� = 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ��
	NVIC_Init(&NVIC_InitStructure);                           //��ʼ��
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //��ռ���ȼ�����Ӧ���ȼ�����
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;          //EXTI1 ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� = 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧ���ȼ� = 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ��
	NVIC_Init(&NVIC_InitStructure);                           //��ʼ��
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //��ռ���ȼ�����Ӧ���ȼ�����
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;          //EXTI3 ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� = 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧ���ȼ� = 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ��
	NVIC_Init(&NVIC_InitStructure);                           //��ʼ��
	
}

/* K1 �����ж� */
void EXTI4_IRQHandler() //K1, PA4 �жϺ���
{
  if(EXTI_GetITStatus(EXTI_Line4)==SET)                //��ѯ��־
  {
    EXTI_ClearITPendingBit(EXTI_Line4);                //�����־
		
		/**************************** Key1 code here *****************************/
		
		Key_LongPress = 0;//������������
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
		// Key_LongPress ����20����ʾ���³���2��
		// Key_state = 0xFF ������
		// Key_state = 0xF1 ����̰�
		if(Key_LongPress >= 20) Key_state = 0xFF; else Key_state = 0xF1;
		
		// ����
		if(Key_state == 0xFF)  
		{
			// ��ʱ����3��ģʽ
			if(current_state < 3) 
			{
				// �κ��������������һ��״̬
				current_state++;
				
				/* ɾ����switch */
			}
			else current_state = 0;
		}
		
		// �̰�
		if(Key_state == 0xF1)
		{
			// �̰�����ݵ�ǰ״ִ̬����һ��
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
	// Key1��״̬�͸����ڻ�����
	usart1_send_buff[13] = 'D';
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0);
	/**************************** Key1 code above *****************************/
}

/* K2 �� K3 �����ж� */
void EXTI9_5_IRQHandler() //Key2,Key3, PA5,PA8 �жϺ���
{
	/* K2 �����ж� */
	if(EXTI_GetITStatus(EXTI_Line5)==SET)                //��ѯ��־
  {
    EXTI_ClearITPendingBit(EXTI_Line5);                //�����־
		
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
	// Key2��״̬�͸����ڻ�����
	usart1_send_buff[14] = 'D';
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0);
	
	/* K3 �����ж� */
	if(EXTI_GetITStatus(EXTI_Line8)==SET)                //��ѯ��־
  {
    EXTI_ClearITPendingBit(EXTI_Line8);                //�����־

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
	// Key3��״̬�͸����ڻ�����
	usart1_send_buff[15] = 'D';
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0);
}

/* K4 �����ж� */
void EXTI15_10_IRQHandler() //Key4, PA15 �жϺ���
{
	if(EXTI_GetITStatus(EXTI_Line15)==SET)                //��ѯ��־
  {
    EXTI_ClearITPendingBit(EXTI_Line15);                //�����־
		
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

