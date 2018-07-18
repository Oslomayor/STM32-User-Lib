/************************************************************************************
*  Copyright (c), 2018, oslomayor technology
*            All rights reserved.
*
* Wechat Offical Account: crazyengineer
* github  : https://github.com/Oslomayor

* File name: OLED_I2C.c
* Processor: STM32F103C8T6
* Compiler : MDK for ARM
* 
* Author : ��
* Version: 1.00
* Date   : 2018.6.6
* 
* Description:128*64�����OLED��ʾ�������ļ�
*
* Function List:
*	1. void I2C_Configuration(void) -- ����CPU��Ӳ��I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- ��Ĵ�����ַдһ��byte������
* 3. void WriteCmd(unsigned char I2C_Command) -- д����
* 4. void WriteDat(unsigned char I2C_Data) -- д����
* 5. void OLED_Init(void) -- OLED����ʼ��
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- ������ʼ������
* 7. void OLED_Fill(unsigned char fill_Data) -- ȫ�����
* 8. void OLED_CLS(void) -- ����
* 9. void OLED_ON(void) -- ����
* 10. void OLED_OFF(void) -- ˯��
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- ��ʾ�ַ���(�����С��6*8��8*16����)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- ��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMPͼƬ
*
* History: none;
*
*************************************************************************************/

#include "OLED_I2C.h"
#include "stm32f10x_i2c.h"
#include "delay.h"
#include "codetab.h"
#include "globe.h"
#include "led.h"

#define size 1

void I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*STM32F103C8T6оƬI2C: PB10 -- SCL; PB11 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C���뿪©���
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C2);//ʹ��I2C2
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//������I2C��ַ,���д��
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C2, ENABLE);
	I2C_Init(I2C2, &I2C_InitStructure);
}

void I2C_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C2, ENABLE);//����I2C2
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	I2C_Send7bitAddress(I2C2, OLED_ADDRESS, I2C_Direction_Transmitter);//������ַ -- Ĭ��0x78
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, addr);//�Ĵ�����ַ
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, data);//��������
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C2, ENABLE);//�ر�I2C2����
}

void WriteCmd(unsigned char I2C_Command)//д����
{
	I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//д����
{
	I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
	DelayMs(100); //�������ʱ����Ҫ
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//ȫ�����
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//����
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED�������л���
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

/****************���������� ��ʾ6*8��8*16��5λ����   ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
/*  2017/9/3  15:48   DZT@���ߵ� */
/*��           
        OLED_ShowInt(0,0,0,1);   //��(0,0)������ʾ6*8��"0"
        OLED_ShowInt(5,4,12345,2);//��(5,4)������ʾ8*16��"12345"
*/
void OLED_ShowInt(unsigned char x, unsigned char y, int Data, unsigned char TextSize)
{
	 unsigned char temp;	
	 OLED_SetPos(x,y);	
   switch(TextSize)	 
	 {
		 case 1:
		 {
			   	if(Data<0)
					{
						OLED_ShowChar(x,y,'-',1);
						x+=6;
						Data=-Data;
					}
				//������Ҫ��ʾ�����������һ����ʾ�����ĸ�λ
				//������������һ�����ţ�����ռ��һ����ʾλ
			  OLED_ShowChar(x+30,y,' ',1);
    
					temp=Data/10000;
					OLED_ShowChar(x,y,(temp+'0'),1);
    
					Data%=10000;
					temp=Data/1000;
					OLED_ShowChar(x+6,y,(temp+'0'),1);
    
					Data%=1000;
					temp=Data/100;
					OLED_ShowChar(x+12,y,(temp+'0'),1);
    
					Data%=100;
					temp=Data/10;
					OLED_ShowChar(x+18,y,(temp+'0'),1);
    
					Data%=10;
					temp=Data;
					OLED_ShowChar(x+24,y,(temp+'0'),1);
		}break;
		case 2:
		{
			   	if(Data<0)
					{
						OLED_ShowChar(x,y,'-',2);
						x+=8;
						Data=-Data;
					}
				//������Ҫ��ʾ�����������һ����ʾ�����ĸ�λ
				//������������һ�����ţ�����ռ��һ����ʾλ
				OLED_ShowChar(x+32,y,' ',2);
    
					//temp=Data/10000;
					//OLED_ShowChar(x,y,(temp+'0'),2);
    
					//Data%=10000;
					temp=Data/1000;
					OLED_ShowChar(x,y,(temp+'0'),2);
    
					Data%=1000;
					temp=Data/100;
					OLED_ShowChar(x+8,y,(temp+'0'),2);
    
					Data%=100;
					temp=Data/10;
					OLED_ShowChar(x+16,y,(temp+'0'),2);
    
					Data%=10;
					temp=Data;
					OLED_ShowChar(x+24,y,(temp+'0'),2);
		}break;
	}
}

/***************������������ʾ6*8��8*16һ����׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
/*����  OLED_P6x8Str(39,0,'A',1)*/
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch, unsigned char TextSize)
{
		unsigned char c=0,i=0;
		c =ch-32;
		if(x>120){x=0;y++;}
		OLED_SetPos(x,y);
		switch(TextSize)
		{
			case 1: 
			{
				for(i=0;i<6;i++)
				WriteDat(F6x8[c][i]);
				break;
			}
			case 2:
			{
				for(i=0;i<8;i++)
				WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
				WriteDat(F8X16[c*16+i+8]);
				x += 8;
				break;
			}
		}
}

//--------------------------------------------------------------
// Prototype      : void OLED_Draw16X16(unsigned char x, unsigned char y, unsigned char arg)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); arg --  16x16 �������, arg ����16x16������еĵڼ������
// Description    : �� 16 x 16 ����
//--------------------------------------------------------------
void OLED_Draw16X16(unsigned char x, unsigned char y, unsigned char arg)
{
	unsigned char i = 0;
	OLED_SetPos(x,y);
	for(i=0;i<16;i++)
		WriteDat(F16X16[arg*32+i]);
	OLED_SetPos(x,y+1);
	for(i=0;i<16;i++)
		WriteDat(F16X16[arg*32+i+16]);
}


//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
// Description    : ��ʾBMPλͼ
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

void OLED_DisplayLED()
{
		if((LED_state&LED1)==LED1) OLED_Draw16X16(0,0,1);  else OLED_Draw16X16(0,0,0);
		if((LED_state&LED2)==LED2) OLED_Draw16X16(26,0,1); else OLED_Draw16X16(26,0,0);
		if((LED_state&LED3)==LED3) OLED_Draw16X16(52,0,1); else OLED_Draw16X16(52,0,0);
		if((LED_state&LED4)==LED4) OLED_Draw16X16(78,0,1); else OLED_Draw16X16(78,0,0);
	// 0x0F ���������״̬
	  if(LED_state == 0x0F) {OLED_Draw16X16(0,0,2);OLED_Draw16X16(26,0,2);OLED_Draw16X16(52,0,2);OLED_Draw16X16(78,0,2);}

}
