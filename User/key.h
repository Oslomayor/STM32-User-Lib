/************************************************************************************
*  Copyright (c), 2018, oslomayor technology
*            All rights reserved.
*
* Wechat Offical Account: crazyengineer
* github  : https://github.com/Oslomayor

* File name: key.h
* Processor: STM32F103C8T6
* Compiler : MDK for ARM
* 
* Author : Õò³¤
* Version: 1.00
* Date   : 2018.6.6
* Pins   :
*          PA4  -> Key1
*          PA5  -> Key2
*          PA8  -> Key3
*          PA15 -> Key4
*************************************************************************************/

#ifndef __key_H
#define __key_H


#include "stm32f10x.h"
#define k1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)    
#define k2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define k3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)   
#define k4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)   

#define KEY_DOWN 0
#define KEY_UP 1



extern void key_init(void);
extern void Exit_init(void);

#endif
