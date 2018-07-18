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
* Description: USART配置文件
* Pin:
*     USART1
*     PA9 -> TX
*     PA10 -> RX
*************************************************************************************/

/* ASCII 码表 */
/************************************************************************************
字符  16进制码
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
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
//若注释掉以下代码，需要在 Target 标签中勾选 USE MicroLIB
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
/********************************************************/

//重定义fputc函数 
int fputc(int ch, FILE *f)
{   
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	
//  寄存器写法
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//  USART1->DR = (u8) ch;      
	return ch;
}
// 串口发送库函数写法
// USART_SendData(USART1,"string");
// while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);  //确保发送完毕
// USART_ClearFlag(USART1,USART_FLAG_TC);                  //清楚标志位



void usart_config(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	//USART1   GPIO 配置
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;            	//TX   PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;						  //RX   PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	//串口配置
	USART_InitStructure.USART_BaudRate=bound;//波特率
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制 
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//收发模式 
	
	USART_Init(USART1,&USART_InitStructure);//初始化 USART1
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启串口中断
	
	//中断
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	
	//USART1    使能
	USART_Cmd(USART1, ENABLE);

}

//USART1   中断接收处理
void USART1_IRQHandler()
{ //串口一旦收到1个字符，就进入1次中断, i 用 static 类型
	static u8 i = 0;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)
	{
	  *(usart1_receive_buff+i) = USART_ReceiveData(USART1);
		// 收到回车符号 0x0A 结束
		if(*(usart1_receive_buff+i) == 0x0A) 
    {
		  *(usart1_receive_buff+i) = '\0'; 
			// 收完一串字符对 i 清零
		  i = 0; 
		  usart1_recive_flag = 1;
			USART_ClearFlag(USART1,USART_IT_RXNE);	
		  return;
		}
		else i++;
	}
	//超出长度重新接收, 防止由于误码结束标志没收到
	if(i>30) usart1_recive_flag = 0;
}
/**************************************以下待写************************************************/
// 仅处理LED灯控制，其余由单片机主动发送
void usart1_rx_buff_process()
{
	u8 check_code_sum = 0;
	if(usart1_receive_buff[0] == '6'&& usart1_receive_buff[1] == 'M')
	{
		// 校验和多余的，不写了
		if(check_code_sum == 0)
		{
			// 帧头 + ‘+’+ 灯号
			if(usart1_receive_buff[2] == '+')
			{
				// 失能流水灯
				LED_Flow_ENABLE = 0;
				switch(usart1_receive_buff[3])
				{
					case '1' : LED_ON(LED1);break;
					case '2' : LED_ON(LED2);break;
					case '3' : LED_ON(LED3);break;
					case '4' : LED_ON(LED4);break;
				}
			}
			// 帧头 + ‘-’+ 灯号
			if(usart1_receive_buff[2] == '-')
			{
				// 失能流水灯
				LED_Flow_ENABLE = 0;
				switch(usart1_receive_buff[3])
				{
					case '1' : LED_OFF(LED1);break;
					case '2' : LED_OFF(LED2);break;
					case '3' : LED_OFF(LED3);break;
					case '4' : LED_OFF(LED4);break;
				}
			}
			// 帧头 + ‘*’+ 灯控数据(4字节码，如0110)
			if(usart1_receive_buff[2] == '*')
			{
				// 失能流水灯
				LED_Flow_ENABLE = 0;
				// 先关闭所有的，再开启要亮的
				LED_OFF(LED1|LED2|LED3|LED4);
				// 这一行参数忘记了看看 LED_ON() 函数，当初写这个函数真是机智的决定
				LED_ON(usart1_receive_buff[3]);
			}		
      // 帧头 + ‘L’+ 流水灯模式码‘0’-‘3’(ASCII码)
			if(usart1_receive_buff[2] == 'L')
			{
				LED_Flow_ENABLE = 1;
				LED_Flow_Mode = usart1_receive_buff[3];
			}			
		}
	}
}

// 待发送的信息已经存入buff，保留了空位给帧头的2个字节,做一些通信协议的处理并发送
// 具体协议和数据排列
/************************************************************************
串口接收缓冲区说明：
协议数据帧头：“ 6M" 即 0x36,0x4D（2字节）
协议数据帧尾： '\n'  即 0x0A     （1字节）
发送缓冲区：usart1_send_buff[128]
usart1_send_buff[0-1]:    保留给帧头
usart1_send_buff[2]:      LED 状态
usart1_send_buff[3]:      ADC 的值 0-4095
usart1_send_buff[4-9]:    aX,aY,aZ,gX,gY,gZ
usart1_send_buff[10-12]:  Pit,Rol,Yaw
usart1_send_buff[13-16]:  Key1-Key4 按键状态码，状态'D'和'U'对应按下和松开
usart1_send_buff[17-18]:  校验和，帧头+有效数据的和取低8位(如0xDF)，再翻译成16进制码(0xDF对应 0x44,0x46)
usart1_send_buff[19]:     保留给帧尾
usart1_send_buff[20]:     字符串结束符 '\0'

*************************************************************************/
void usart_send_info(int *usart1_send_buff)
{
	u8 i = 0;
	u8 check_sum = 0;
	u8 check_code_L4,check_code_H4;
	// 帧头 6M
	*usart1_send_buff = '6';
	*(usart1_send_buff+1) = 'M';
	// 校验和
	for(i=0;i<=3;i++)
	{
		check_sum += *(usart1_send_buff+i);
	}
		// 处理高4位
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
	// 处理低4位,check_code<<4>>4 哪里不对
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
	// 帧尾 \n
	*(usart1_send_buff+19) = '\n';
	// 字符串结束
	*(usart1_send_buff+20) = '\0';
	/**************** 以下发送数据 *****************/
	// 帧头
	printf("%c",usart1_send_buff[0]);
	printf("%c ",usart1_send_buff[1]);
	// LED 状态
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
	// ADC 值
	printf("A");
	printf("%d ",usart1_send_buff[3]);
	// 加速度和角速度
	for(i=4;i<=12;i++)
		printf("%d ",usart1_send_buff[i]);
	// 按键状态发送
	for(i=13;i<=16;i++)
		printf("%c",usart1_send_buff[i]);
	// 按键状态清空
	for(i=13;i<=16;i++)
		usart1_send_buff[i] = 'U';
	printf(" 0x%x",usart1_send_buff[17]);
	printf(" 0x%x",usart1_send_buff[18]);
 	printf("\n");
  /**************** 以上发送数据 *****************/
}




