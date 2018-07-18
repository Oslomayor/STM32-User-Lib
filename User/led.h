/************************************************************************************
*  Copyright (c), 2018, oslomayor technology
*            All rights reserved.
*
* Wechat Offical Account: crazyengineer
* github  : https://github.com/Oslomayor

* File name: led.h
* Processor: STM32F103C8T6
* Compiler : MDK for ARM
* 
* Author : Õò³¤
* Version: 1.00
* Date   : 2018.6.6
* Pins   :
*          PA4 -> LED1
*          PA5 -> LED2
*          PB0 -> LED3
*          PB1 -> LED4
*************************************************************************************/

#ifndef LED_H
#define LED_H

#include "stm32f10x.h"

#define LED1 (0xF1)
#define LED2 (0XF2)
#define LED3 (0xF4)
#define LED4 (0xF8)

extern void delay(u32 i);
extern void LED_Init(void);
extern void LED_ON(unsigned char);
extern void LED_OFF(unsigned char);
extern void LED_Blink(void);
extern void LED_Breathe(u16 adc_value);
extern void LED_Key_Control(void);
extern void LED_Flow(u16 adc_value, u8 mode);

#endif // end of LED_H
