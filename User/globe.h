/********************************************************************************
* @file    main.c
* @author  镇长
* @date    2018-6-13
*
* Wechat Offical Account: CrazyEngineer
* github  : https://github.com/Oslomayor
*
********************************************************************************/  

#ifndef __globe_H
#define __globe_H

#include "stm32f10x.h"

// 全局状态机变量
// 按需要在按键中断里面添加状态判断
#define STA_NORMAL (0)
#define STA_LED_BREATHE (1)
#define STA_LED_FLOW (2)
#define STA_LED_KEY_CONTROL (3)

// 全局变量要用 extern 先声明，然后在具体的 C 文件中定义具体值， 否则会报错 multiply defined 
extern unsigned char LED_state;
extern unsigned char Key_state;
extern u16 adc_value;
extern unsigned char current_state;
extern u16 Key_LongPress;
extern u16 Key_LongPress_Cache;

// 使能控制
extern u8 LED_Breathe_ENABLE;
extern u8 LED_Flow_ENABLE;
extern u8 LED_Flow_Mode;
extern u8 LED_Key_Control_ENABLE;
extern u8 MPU_GETDATA_ENABL;

// 串口发送和接收
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
extern unsigned char usart1_receive_buff[32];
extern u8 usart1_recive_flag;
extern int usart1_send_buff[32];


#endif





