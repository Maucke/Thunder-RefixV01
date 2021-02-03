
	  /******************************************************************************
* @ File name --> ds3231.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 02 - 01 - 2014
* @ Brief     --> �߾���ʼ��оƬDS3231��������
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.
* @ Author    -->
* @ Date      -->
* @ Revise    -->
*
******************************************************************************/

#ifndef _ds3231_h_
#define _ds3231_h_

/******************************************************************************
                                �ⲿ����ͷ�ļ�                        
******************************************************************************/


//#include "sys.h"
#include "stm32f4xx_hal.h"

//#define DS3231_SCL_H() HAL_GPIO_WritePin(GPIOA, DS_SCL_Pin, GPIO_PIN_SET)
//#define DS3231_SCL_L() HAL_GPIO_WritePin(GPIOA, DS_SCL_Pin, GPIO_PIN_RESET)

//#define DS3231_SDA_H() HAL_GPIO_WritePin(GPIOA, DS_SDA_Pin, GPIO_PIN_SET)
//#define DS3231_SDA_L() HAL_GPIO_WritePin(GPIOA, DS_SDA_Pin, GPIO_PIN_RESET)

//#define DS3231_IN_SDA() HAL_GPIO_ReadPin(GPIOA, DS_SDA_Pin)

//#define ds_a = 0 IN_SDA

//

typedef struct
{
	char Time[10];
	char Timens[10];
	char Hour[10];
	char Min[10];
	char Sec[10];
	char Data[10];
	char Week[10];
	char Weekm[10];
	char Monm[10];
	char Mon[10];
	char Day[10];
	char Year[10];
} DS3231;

/******************************************************************************
                         DS3231����ʱ�ӼĴ����ṹ����                          
******************************************************************************/

extern DS3231 ds3231;
extern uint8_t DS3231_US_Buf[7];	//ʱ���������ݻ���
extern uint8_t DS3231_Init_Buf[7];
/******************************************************************************
                                  �����궨��                       
******************************************************************************/

//#define DS3231_YEARDATA                  (uint16_t)0x2018	//��ṹ�������19xx�ꡢ21xx��Ļ����޸������ֵ����

#define DS3231_ADD_BASS					0xd0	//��������ַ

#define DS3231_Write_ADD				(DS3231_ADD_BASS | 0x00)	//дDS3231
#define DS3231_Read_ADD					(DS3231_ADD_BASS | 0x01)	//��DS3231

/******************************************************************************
                              �����Ĵ�����ַ�궨��                    
******************************************************************************/

#define Address_second					0x00	//��
#define Address_minute					0x01	//��
#define Address_hour					0x02	//ʱ
#define Address_week					0x03	//����
#define Address_date					0x04	//��
#define Address_month					0x05	//��
#define Address_year					0x06	//��

#define Address_second_Alarm1			0x07	//������
#define Address_minute_Alarm1			0x08	//������
#define Address_hour_Alarm1				0x09	//ʱ����
#define Address_week_Alarm1				0x0a	//�����塢��������

#define Address_minute_Alarm2			0x0b	//������
#define Address_hour_Alarm2				0x0c	//ʱ����
#define Address_week_Alarm2				0x0d	//�����塢��������


#define Address_control					0x0e	//����
#define Address_control_status			0x0f	//���ƺ�״̬��־

#define Address_offset					0x10	//Aging Offset

#define Address_temp_MSB				0x11	//�¶ȸ�8λ
#define Address_temp_LSB				0x12	//�¶ȵ�8λ

/******************************************************************************
                             ʱ�����������Чλ�궨��                 
******************************************************************************/

#define Shield_secondBit			0x7f
#define Shield_minuteBit			0x7f
#define Shield_hourBit				0x3f
#define Shield_weekBit				0x07
#define Shield_dateBit				0x3f
#define Shield_monthBit				0x1f

/******************************************************************************
                                 ���������                      
******************************************************************************/

//Сʱ�Ĵ���
#define Hour_Mode12					(1<<6)	//12Сʱ��ʽ
#define Hour_Mode24					(0<<6)	//24Сʱ��ʽ

//������Ĵ���1
#define Alarm_second_open			(1<<7)	//�����忪
#define Alarm_second_close			(0<<7)	//�������

//������Ĵ���1&2
#define Alarm_minute_open			(1<<7)	//�����忪
#define Alarm_minute_close			(0<<7)	//�������

//ʱ����Ĵ���1&2��������Ϊ12Сʱģʽ����24Сʱģʽ��
#define Alarm_hour_open12			(3<<6)	//ʱ���忪��12Сʱ��ʽ
#define Alarm_hour_close12			(1<<6)	//ʱ����أ�24Сʱ��ʽ

#define Alarm_hour_open24			(2<<6)	//ʱ���忪��12Сʱ��ʽ
#define Alarm_hour_close24			(0<<6)	//ʱ����أ�24Сʱ��ʽ

//���ڡ�������Ĵ���1&2����ѡ��������������������壩
#define Alarm_week_open				(3<<6)	//�������忪
#define Alarm_week_close			(1<<6)	//���������

#define Alarm_date_open				(2<<6)	//�������忪
#define Alarm_date_close			(0<<6)	//���������


//������ƼĴ���
#define OSC_Enable					(0<<7)	//��������
#define OSC_Disable					(1<<7)	//ֹͣ����

#define SET_BBSQW					(1<<6)	//Vbat pin
#define RESET_BBSQW					(0<<6)	//int/sqw����

#define Temp_CONV_SET				(1<<5)	//ǿ���¶�ת��λ������
#define Temp_CONV_Clear				(0<<5)

#define SQW_OUT1Hz					(0<<3)	//1Hz
#define SQW_OUT1024Hz				(1<<3)	//1.024KHz
#define SQW_OUT4096Hz				(2<<3)	//4.096KHz
#define SQW_OUT8192Hz				(3<<3)	//8.192KHz

#define OUTPUT_INTSQW				(0<<2)	//����������ϵ��λ��1��INTCNλ��

#define A2IE_Enable					(1<<1)	//enable alarm 2
#define A2IE_Disable				(0<<1)	//disable alarm 2

#define A1IE_Enable					(1<<0)	//enable alarm 1
#define A1IE_Disable				(0<<0)	//disable alarm 1

//���ơ�״̬�Ĵ���
#define Clear_OSF_Flag				(0<<7)	//clear OSF flag

#define Enable_OSC32768				(1<<3)	//EN32KHz EN
#define Disable_OSC32768			(0<<3)	//EN32KHz����

#define Clear_A2IE_Flag				(0<<1)	//�������2�жϱ�־
#define Clear_A1IE_Flag				(0<<0)	//�������2�жϱ�־

/******************************************************************************
                                �ⲿ���ܺ���                      
******************************************************************************/

uint8_t DS3231_Check(void);	//DS3231��⺯��
void DS3231_Write_Byte(uint8_t REG_ADD,uint8_t dat);	//DS3231ĳ�Ĵ���д��һ���ֽ�����
uint8_t DS3231_Read_Byte(uint8_t REG_ADD);	//DS3231ĳ�Ĵ�����ȡһ���ֽ�����
void DS3231_ReadWrite_Time(uint8_t mode);	//DS3231��ȡ����д��ʱ����Ϣ
void DS3231_Time_Init(uint8_t *Time_ch);	//ʱ��������ʼ��
void Time_Handle(void);	//ʱ���������ݴ�����
void DS3231_SetUart(void);
void DS3231_SetWiFi(void);
void DS3231_Read_Temp(uint8_t *Temp);	//��ȡоƬ�¶ȼĴ���

uint8_t	BCD2HEX(uint8_t val);
uint8_t	HEX2BCD(uint8_t val);

#endif

