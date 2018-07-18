/********************************************************************************
* @file    main.c
* @author  ��
* @date    2018-6-6
*
* Wechat Offical Account: CrazyEngineer
* github  : https://github.com/Oslomayor
*
********************************************************************************/  
/*

*/

#include "stm32f10x.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "led.h"
#include "key.h"
#include "adc.h"
#include "globe.h"



unsigned char LED_state = 0xF0;
unsigned char Key_state = 0xF0;
unsigned char current_state = STA_NORMAL;
u16  adc_value;
u8 LED_Breathe_ENABLE = 0;
u8 LED_Flow_ENABLE = 0;
u8 LED_Flow_Mode = 0;
u8 LED_Key_Control_ENABLE = 0;
u8 MPU_GETDATA_ENABL = 1;

//���ڻ�����
unsigned char usart1_receive_buff[32];
u8 usart1_recive_flag;
int usart1_send_buff[32];

int main()
{
	u8 Key_delay = 0; //������ʾ����״̬��ʱ��
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		  //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temperature;
	
	DelayInit();
	I2C_Configuration();
	usart_config(9600);
	LED_Init();
	OLED_Init();
	Key_init();
	Exit_init();
	adc_init();
	MPU_Init();
	
	OLED_Fill(0x00);//ȫ��Ϩ��
	LED_OFF(LED1 | LED2 | LED3 | LED4);
	OLED_Draw16X16(0,0,0);
	OLED_Draw16X16(26,0,0);
	OLED_Draw16X16(52,0,0);
	OLED_Draw16X16(78,0,0);
	OLED_ShowStr(108,0,"K",2);
	// ��ʾLED��״̬
	OLED_DisplayLED();
	
	//MPU6050 �����˶����ʼ��
	while(mpu_dmp_init())
	{
		OLED_ShowStr(0,2,"MPU6050 Error",1);
		DelayMs(400);
		OLED_ShowStr(0,2,"             ",1);
		DelayMs(100);
	}
	OLED_ShowStr(0,2,"MPU6050 Succeed",1);
	DelayS(2);
	OLED_ShowStr(0,2,"               ",1);
	//��ʾŷ���Ǳ�ǩ
	OLED_ShowStr(0,2,"Pit: ",1); 
	OLED_ShowStr(0,3,"Rol: ",1); 
	OLED_ShowStr(0,4,"Yaw: ",1); 
	//��ʾ���ٶȱ�ǩ
	OLED_ShowStr(4,5,"aX: ",1); 
	OLED_ShowStr(4,6,"aY: ",1); 
	OLED_ShowStr(4,7,"aZ:  ",1);
	//��ʾ���ٶȱ�ǩ
	OLED_ShowStr(64,5,"gX: ",1); 
	OLED_ShowStr(64,6,"gY: ",1); 
	OLED_ShowStr(64,7,"gZ: ",1); 
	//��ʾ�¶ȱ�ǩ
	OLED_ShowStr(72,2,"t:",1); 
	
	
	while(1)
	{
		adc_read(&adc_value);
		OLED_ShowStr(64,3,"ADC",2);
		OLED_ShowInt(88,3,adc_value,2);
		usart1_send_buff[3] = adc_value;

		// ��ʾ��ǰ������״̬
		switch(Key_state)
		{
			case 0xF1: OLED_ShowStr(108,0,"K1",2);break;
			case 0xF2: OLED_ShowStr(108,0,"K2",2);break;
			case 0xF4: OLED_ShowStr(108,0,"K3",2);break;
			case 0xF8: OLED_ShowStr(108,0,"K4",2);break;
			case 0xFF: OLED_ShowStr(108,0,"KL",2);break;// ���� K1 ����
			default  : OLED_ShowStr(108,0,"  ",2);
		}
		// ���������ʾ
		Key_delay++;
		if(Key_delay > 5) {Key_delay = 0; Key_state = 0xF0;}
		
		if(MPU_GETDATA_ENABL && mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			temperature = MPU_Get_Temperature();
			
			// ���ٶ�ֵ�͸����ڷ��ͻ�����
			usart1_send_buff[4] = aacx;
			usart1_send_buff[5] = aacy;
			usart1_send_buff[6] = aacz;
			// ���ٶ�ֵ�͸����ڷ��ͻ�����
			usart1_send_buff[7] = gyrox;
			usart1_send_buff[8] = gyroy;
			usart1_send_buff[9] = gyroz;
			// ŷ�����͸����ڷ��ͻ�����
			usart1_send_buff[10] = pitch;
			usart1_send_buff[11] = roll;
			usart1_send_buff[12] = yaw;
			
			OLED_ShowInt(24,2,(pitch),1);
	    OLED_ShowInt(24,3,(roll),1);
	    OLED_ShowInt(24,4,(yaw),1);
			
			OLED_ShowInt(22,5,(aacx),1); 
			OLED_ShowInt(22,6,(aacy),1);
			OLED_ShowInt(22,7,(aacz),1);
			
			OLED_ShowInt(88,5,(gyrox),1); 
			OLED_ShowInt(88,6,(gyroy),1);
			OLED_ShowInt(88,7,(gyroz),1);
			
			OLED_ShowInt(84,2,(temperature),1);
			
		}
		
	  // ״̬��
		switch(current_state)
		{
			case STA_NORMAL : break;
			case STA_LED_BREATHE: 
				LED_Breathe_ENABLE = 1;
				LED_Flow_ENABLE = 0;
				LED_Key_Control_ENABLE = 0;
				break;
			case STA_LED_FLOW: 
				LED_Flow_ENABLE = 1;
				LED_Breathe_ENABLE = 0;
				LED_Key_Control_ENABLE = 0;
				break;
			case STA_LED_KEY_CONTROL: 
				LED_Key_Control_ENABLE = 1;
				LED_Breathe_ENABLE = 0;
				LED_Flow_ENABLE = 0;
			default : ;
		}
		
		//��ʾ��ǰ״̬
		switch(current_state)
		{
			case 0: OLED_ShowChar(96,0,'0',1);break;
			case 1: OLED_ShowChar(96,0,'1',1);break;
			case 2: OLED_ShowChar(96,0,'2',1);break;
			case 3: OLED_ShowChar(96,0,'3',1);break;
		}
		
		// �������
		usart_send_info(usart1_send_buff);

		
		
		// ִ����Ӧ����
		if(LED_Breathe_ENABLE) LED_Breathe(adc_value);
		if(LED_Flow_ENABLE) LED_Flow(adc_value,LED_Flow_Mode);
		if(LED_Key_Control_ENABLE) LED_Key_Control();
		if(usart1_recive_flag == 1) 
    { 
			usart1_recive_flag = 0; 
			//OLED_ShowStr(0,7,usart1_receive_buff,1);
			usart1_rx_buff_process();
		}
		
	}
  return 0;
}

