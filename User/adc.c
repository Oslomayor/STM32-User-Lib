/************************************************************************************
*  Copyright (c), 2018, oslomayor technology
*            All rights reserved.
*
* Wechat Offical Account: crazyengineer
* github  : https://github.com/Oslomayor

* File name: adc.c
* Processor: STM32F103C8T6
* Compiler : MDK for ARM
* 
* Author : ��
* Version: 1.00
* Date   : 2018.6.6
* Pins   :
*          PA1 -> ADC12_IN1
*          
*          
*          
*************************************************************************************/

#include "adc.h"


/*******************************************************************************
* �� �� ��         : adc_init
* ��������		   : IO�˿�ʱ�ӳ�ʼ������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void adc_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_ADC1,ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12M  ���14M ����ADCʱ�ӣ�ADCCLK��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;     //ADC  Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1; 
	ADC_Init(ADC1, &ADC_InitStructure);

	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1,ENABLE);	
	ADC_ResetCalibration(ADC1);//����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//��ȡADC����У׼�Ĵ�����״̬
	ADC_StartCalibration(ADC1);//��ʼָ��ADC��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1)); //��ȡָ��ADC��У׼����

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
}
void adc_read(u16 *adc_value)
{
	   ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);
	   ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		 while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//ת��������־λ
		 *adc_value = ADC_GetConversionValue(ADC1);
}




