/********************************************************************************
* @file    main.c
* @author  ��
* @date    2018-6-13
*
* Wechat Offical Account: CrazyEngineer
* github  : https://github.com/Oslomayor
*
********************************************************************************/  

#ifndef __globe_H
#define __globe_H

#include "stm32f10x.h"

// ȫ��״̬������
// ����Ҫ�ڰ����ж��������״̬�ж�
#define STA_NORMAL (0)
#define STA_LED_BREATHE (1)
#define STA_LED_FLOW (2)
#define STA_LED_KEY_CONTROL (3)

// ȫ�ֱ���Ҫ�� extern ��������Ȼ���ھ���� C �ļ��ж������ֵ�� ����ᱨ�� multiply defined 
extern unsigned char LED_state;
extern unsigned char Key_state;
extern u16 adc_value;
extern unsigned char current_state;
extern u16 Key_LongPress;
extern u16 Key_LongPress_Cache;

// ʹ�ܿ���
extern u8 LED_Breathe_ENABLE;
extern u8 LED_Flow_ENABLE;
extern u8 LED_Flow_Mode;
extern u8 LED_Key_Control_ENABLE;
extern u8 MPU_GETDATA_ENABL;

// ���ڷ��ͺͽ���
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
extern unsigned char usart1_receive_buff[32];
extern u8 usart1_recive_flag;
extern int usart1_send_buff[32];


#endif





