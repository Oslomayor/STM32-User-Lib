/************************************************************************************
*  Copyright (c), 2018, oslomayor technology
*            All rights reserved.
*
* Wechat Offical Account: CrazyEngineer
* github  : https://github.com/Oslomayor

* File name: usart.c
* Processor: STM32F103C8T6
* Compiler : MDK for ARM
* 
* Author : Oslomayor
* Version: 1.00
* Date   : 2018.6.9
* 
* Description: USART�����ļ�
* Pin:
*     USART1
*     PA9 -> TX
*     PA10 -> RX
*************************************************************************************/

/* ASCII ��� */
/************************************************************************************
�ַ�  16������
 6      0x36
 M      0x4D
 
 D      0x44
 F      0x46
 
 \r     0x0D
 \n     0x0A

*************************************************************************************/


#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "stm32f10x_usart.h"
#include "globe.h"
#include "led.h"



/*****************************************************/
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
//��ע�͵����´��룬��Ҫ�� Target ��ǩ�й�ѡ USE MicroLIB
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
/********************************************************/

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{   
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	
//  �Ĵ���д��
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//  USART1->DR = (u8) ch;      
	return ch;
}
// ���ڷ��Ϳ⺯��д��
// USART_SendData(USART1,"string");
// while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);  //ȷ���������
// USART_ClearFlag(USART1,USART_FLAG_TC);                  //�����־λ



void usart_config(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	//USART1   GPIO ����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;            	//TX   PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;				//�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;						  //RX   PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	//��������
	USART_InitStructure.USART_BaudRate=bound;//������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;//����żУ��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ������������ 
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ 
	
	USART_Init(USART1,&USART_InitStructure);//��ʼ�� USART1
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���������ж�
	
	//�ж�
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	
	//USART1    ʹ��
	USART_Cmd(USART1, ENABLE);

}

//USART1   �жϽ��մ���
void USART1_IRQHandler()
{ //����һ���յ�1���ַ����ͽ���1���ж�, i �� static ����
	static u8 i = 0;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)
	{
	  *(usart1_receive_buff+i) = USART_ReceiveData(USART1);
		// �յ��س����� 0x0A ����
		if(*(usart1_receive_buff+i) == 0x0A) 
    {
		  *(usart1_receive_buff+i) = '\0'; 
			// ����һ���ַ��� i ����
		  i = 0; 
		  usart1_recive_flag = 1;
			USART_ClearFlag(USART1,USART_IT_RXNE);	
		  return;
		}
		else i++;
	}
	//�����������½���, ��ֹ�������������־û�յ�
	if(i>30) usart1_recive_flag = 0;
}
/**************************************���´�д************************************************/
// ������LED�ƿ��ƣ������ɵ�Ƭ����������
void usart1_rx_buff_process()
{
	u8 check_code_sum = 0;
	if(usart1_receive_buff[0] == '6'&& usart1_receive_buff[1] == 'M')
	{
		// У��Ͷ���ģ���д��
		if(check_code_sum == 0)
		{
			// ֡ͷ + ��+��+ �ƺ�
			if(usart1_receive_buff[2] == '+')
			{
				// ʧ����ˮ��
				LED_Flow_ENABLE = 0;
				switch(usart1_receive_buff[3])
				{
					case '1' : LED_ON(LED1);break;
					case '2' : LED_ON(LED2);break;
					case '3' : LED_ON(LED3);break;
					case '4' : LED_ON(LED4);break;
				}
			}
			// ֡ͷ + ��-��+ �ƺ�
			if(usart1_receive_buff[2] == '-')
			{
				// ʧ����ˮ��
				LED_Flow_ENABLE = 0;
				switch(usart1_receive_buff[3])
				{
					case '1' : LED_OFF(LED1);break;
					case '2' : LED_OFF(LED2);break;
					case '3' : LED_OFF(LED3);break;
					case '4' : LED_OFF(LED4);break;
				}
			}
			// ֡ͷ + ��*��+ �ƿ�����(4�ֽ��룬��0110)
			if(usart1_receive_buff[2] == '*')
			{
				// ʧ����ˮ��
				LED_Flow_ENABLE = 0;
				// �ȹر����еģ��ٿ���Ҫ����
				LED_OFF(LED1|LED2|LED3|LED4);
				// ��һ�в��������˿��� LED_ON() ����������д����������ǻ��ǵľ���
				LED_ON(usart1_receive_buff[3]);
			}		
      // ֡ͷ + ��L��+ ��ˮ��ģʽ�롮0��-��3��(ASCII��)
			if(usart1_receive_buff[2] == 'L')
			{
				LED_Flow_ENABLE = 1;
				LED_Flow_Mode = usart1_receive_buff[3];
			}			
		}
	}
}

// �����͵���Ϣ�Ѿ�����buff�������˿�λ��֡ͷ��2���ֽ�,��һЩͨ��Э��Ĵ�������
// ����Э�����������
/************************************************************************
���ڽ��ջ�����˵����
Э������֡ͷ���� 6M" �� 0x36,0x4D��2�ֽڣ�
Э������֡β�� '\n'  �� 0x0A     ��1�ֽڣ�
���ͻ�������usart1_send_buff[128]
usart1_send_buff[0-1]:    ������֡ͷ
usart1_send_buff[2]:      LED ״̬
usart1_send_buff[3]:      ADC ��ֵ 0-4095
usart1_send_buff[4-9]:    aX,aY,aZ,gX,gY,gZ
usart1_send_buff[10-12]:  Pit,Rol,Yaw
usart1_send_buff[13-16]:  Key1-Key4 ����״̬�룬״̬'D'��'U'��Ӧ���º��ɿ�
usart1_send_buff[17-18]:  У��ͣ�֡ͷ+��Ч���ݵĺ�ȡ��8λ(��0xDF)���ٷ����16������(0xDF��Ӧ 0x44,0x46)
usart1_send_buff[19]:     ������֡β
usart1_send_buff[20]:     �ַ��������� '\0'

*************************************************************************/
void usart_send_info(int *usart1_send_buff)
{
	u8 i = 0;
	u8 check_sum = 0;
	u8 check_code_L4,check_code_H4;
	// ֡ͷ 6M
	*usart1_send_buff = '6';
	*(usart1_send_buff+1) = 'M';
	// У���
	for(i=0;i<=3;i++)
	{
		check_sum += *(usart1_send_buff+i);
	}
		// �����4λ
	switch(check_sum>>4)
	{
		case 0x00 : check_code_H4 = '0';break;
		case 0x01 : check_code_H4 = '1';break;
		case 0x02 : check_code_H4 = '2';break;
		case 0x03 : check_code_H4 = '3';break;
		case 0x04 : check_code_H4 = '4';break;
		case 0x05 : check_code_H4 = '5';break;
		case 0x06 : check_code_H4 = '6';break;
		case 0x07 : check_code_H4 = '7';break;
		case 0x08 : check_code_H4 = '8';break;
		case 0x09 : check_code_H4 = '9';break;
		case 0x0A : check_code_H4 = 'A';break;
		case 0x0B : check_code_H4 = 'B';break;
		case 0x0C : check_code_H4 = 'C';break;
		case 0x0D : check_code_H4 = 'D';break;
		case 0x0E : check_code_H4 = 'E';break;
		case 0x0F : check_code_H4 = 'F';break;
		default   : check_code_H4 = 'Z';
	}
	// �����4λ,check_code<<4>>4 ���ﲻ��
	switch(check_sum&0x0F)
	{
		case 0x00 : check_code_L4 = '0';break;
		case 0x01 : check_code_L4 = '1';break;
		case 0x02 : check_code_L4 = '2';break;
		case 0x03 : check_code_L4 = '3';break;
		case 0x04 : check_code_L4 = '4';break;
		case 0x05 : check_code_L4 = '5';break;
		case 0x06 : check_code_L4 = '6';break;
		case 0x07 : check_code_L4 = '7';break;
		case 0x08 : check_code_L4 = '8';break;
		case 0x09 : check_code_L4 = '9';break;
		case 0x0A : check_code_L4 = 'A';break;
		case 0x0B : check_code_L4 = 'B';break;
		case 0x0C : check_code_L4 = 'C';break;
		case 0x0D : check_code_L4 = 'D';break;
		case 0x0E : check_code_L4 = 'E';break;
		case 0x0F : check_code_L4 = 'F';break;
		default   : check_code_L4 = 'Z';
	}
	*(usart1_send_buff+17) = check_code_H4;
	*(usart1_send_buff+18) = check_code_L4;
	// ֡β \n
	*(usart1_send_buff+19) = '\n';
	// �ַ�������
	*(usart1_send_buff+20) = '\0';
	/**************** ���·������� *****************/
	// ֡ͷ
	printf("%c",usart1_send_buff[0]);
	printf("%c ",usart1_send_buff[1]);
	// LED ״̬
	switch(usart1_send_buff[2])
	{
		case 0xF0 : printf("0000 ");break;
		case 0xF1 : printf("0001 ");break;
		case 0xF2 : printf("0010 ");break;
		case 0xF3 : printf("0011 ");break;
		case 0xF4 : printf("0100 ");break;
		case 0xF5 : printf("0101 ");break;
		case 0xF6 : printf("0110 ");break;
		case 0xF7 : printf("0111 ");break;
		case 0xF8 : printf("1000 ");break;
		case 0xF9 : printf("1001 ");break;
		case 0xFA : printf("1010 ");break;
		case 0xFB : printf("1011 ");break;
		case 0xFC : printf("1100 ");break;
		case 0xFD : printf("1101 ");break;
		case 0xFE : printf("1110 ");break;
		case 0xFF : printf("1111 ");break;
		case 0x0F : printf("BR ");  break;
		case 0x1F : 
		case 0x2F : 
		case 0x3F : printf("FL ");  break;
		default   : printf("0000 ");
	}
	// ADC ֵ
	printf("A");
	printf("%d ",usart1_send_buff[3]);
	// ���ٶȺͽ��ٶ�
	for(i=4;i<=12;i++)
		printf("%d ",usart1_send_buff[i]);
	// ����״̬����
	for(i=13;i<=16;i++)
		printf("%c",usart1_send_buff[i]);
	// ����״̬���
	for(i=13;i<=16;i++)
		usart1_send_buff[i] = 'U';
	printf(" 0x%x",usart1_send_buff[17]);
	printf(" 0x%x",usart1_send_buff[18]);
 	printf("\n");
  /**************** ���Ϸ������� *****************/
}




