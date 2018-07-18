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
* Author : 镇长
* Version: 1.00
* Date   : 2018.6.6
* 
* Description:128*64点阵的OLED显示屏驱动文件
*
* Function List:
*	1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
* 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
* 4. void WriteDat(unsigned char I2C_Data) -- 写数据
* 5. void OLED_Init(void) -- OLED屏初始化
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
* 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
* 8. void OLED_CLS(void) -- 清屏
* 9. void OLED_ON(void) -- 唤醒
* 10. void OLED_OFF(void) -- 睡眠
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
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

	/*STM32F103C8T6芯片I2C: PB10 -- SCL; PB11 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C2);//使用I2C2
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//主机的I2C地址,随便写的
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C2, ENABLE);
	I2C_Init(I2C2, &I2C_InitStructure);
}

void I2C_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C2, ENABLE);//开启I2C2
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C2, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, data);//发送数据
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C2, ENABLE);//关闭I2C2总线
}

void WriteCmd(unsigned char I2C_Command)//写命令
{
	I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
	DelayMs(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
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

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
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

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
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

/****************功能描述： 显示6*8或8*16的5位整数   显示的坐标（x,y），y为页范围0～7****************************/
/*  2017/9/3  15:48   DZT@无线电 */
/*例           
        OLED_ShowInt(0,0,0,1);   //在(0,0)处，显示6*8的"0"
        OLED_ShowInt(5,4,12345,2);//在(5,4)处，显示8*16的"12345"
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
				//接下来要显示正数，清空上一次显示负数的个位
				//负数比正数多一个负号，额外占了一个显示位
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
				//接下来要显示正数，清空上一次显示负数的个位
				//负数比正数多一个负号，额外占了一个显示位
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

/***************功能描述：显示6*8或8*16一个标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
/*例：  OLED_P6x8Str(39,0,'A',1)*/
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
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); arg --  16x16 点阵序号, arg 代表画16x16点阵表中的第几组点阵
// Description    : 画 16 x 16 点阵
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
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
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
	// 0x0F 代表呼吸灯状态
	  if(LED_state == 0x0F) {OLED_Draw16X16(0,0,2);OLED_Draw16X16(26,0,2);OLED_Draw16X16(52,0,2);OLED_Draw16X16(78,0,2);}

}
