#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

extern void I2C_Configuration(void);
extern void I2C_WriteByte(uint8_t addr,uint8_t data);
extern void WriteCmd(unsigned char I2C_Command);
extern void WriteDat(unsigned char I2C_Data);
extern void OLED_Init(void);
extern void OLED_SetPos(unsigned char x, unsigned char y);
extern void OLED_Fill(unsigned char fill_Data);
extern void OLED_CLS(void);
extern void OLED_ON(void);
extern void OLED_OFF(void);
extern void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
extern void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch, unsigned char TextSize);
extern void OLED_ShowInt(unsigned char x, unsigned char y, int Data, unsigned char TextSize);
extern void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
extern void OLED_Draw16X16(unsigned char x, unsigned char y, unsigned char arg);
extern void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
extern void OLED_DisplayLED();

#endif
