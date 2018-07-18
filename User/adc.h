/************************************************************************************
*  Copyright (c), 2018, oslomayor technology
*            All rights reserved.
*
* Wechat Offical Account: crazyengineer
* github  : https://github.com/Oslomayor

* File name: adc.h
* Processor: STM32F103C8T6
* Compiler : MDK for ARM
* 
* Author : Õò³¤
* Version: 1.00
* Date   : 2018.6.6
* Pins   :
*          PA1 -> ADC12_IN1
*          
*          
*          
*************************************************************************************/

#ifndef __adc_H
#define __adc_H

#include "stm32f10x.h"



extern void adc_init(void);
extern void adc_read(u16 *adc_value);

#endif
