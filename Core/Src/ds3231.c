#include "ds3231.h"
#include "usart.h"
#include "stdlib.h"
#include "i2c.h"
#include "stdio.h"
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

/******************************************************************************
                               ������ʾʱ���ʽ
                         Ҫ�ı���ʾ�ĸ�ʽ���޸Ĵ�����
******************************************************************************/

//ʱ����ʾ����   ��ʽ  00:00:00

/******************************************************************************
                               ������صı�������
******************************************************************************/

uint8_t DS3231_US_Buf[7];	//ʱ���������ݻ���
uint8_t DS3231_Init_Buf[7] = { 0x55, 0x11, 0x10, 0x01, 0x26, 0x06, 0x18 };


/*I2C��ȡһ���ֽ�*/
//uint8_t IIC_Read_Byte(void)
//{
//	uint8_t i;
//	uint8_t x=0;

//	SDA_H();	//������������Ϊ�ߵ�ƽ

//	IIC_SDA_IN();	//���ó�����

//	for(i = 0;i < 8;i++)
//	{
//		x <<= 1;	//�������ݣ���λ��ǰ

//		delay_us(2);
//		SCL_H();	//ͻ��
//		delay_us(2);
//		
//		if(IN_SDA())	x |= 0x01;	//�յ��ߵ�ƽ

//		SCL_L();
//		delay_us(2);
//	}	//���ݽ������

//	SCL_L();

//	return x;	//���ض�ȡ��������
//}

/********************************************************************************************************
** 	��������:			BCD2HEX(uint8_t val)
**	��������:			BCDתHEX
********************************************************************************************************/
uint8_t	BCD2HEX(uint8_t val)
{
	return	((val >> 4) * 10) + (val & 0x0f);
}
/********************************************************************************************************
**	��������:			HEXתBCD
**	��ڲ���:			val:HAX��
********************************************************************************************************/
uint8_t	HEX2BCD(uint8_t val)
{
	return	(((val % 100) / 10) << 4) | (val % 10);
}

/******************************************************************************
* Function Name --> DS3231ĳ�Ĵ���д��һ���ֽ�����
* Description   --> none
* Input         --> REG_ADD��Ҫ�����Ĵ�����ַ
*                   dat��Ҫд�������
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void DS3231_Write_Byte(uint8_t REG_ADD,uint8_t dat)
{
	IIC_Start();
	if(!(IIC_Write_Byte(DS3231_Write_ADD)))	//����д������Ӧ��λ
	{
		IIC_Write_Byte(REG_ADD);
		IIC_Write_Byte(dat);	//��������
	}
	IIC_Stop();
}
/******************************************************************************
* Function Name --> DS3231ĳ�Ĵ�����ȡһ���ֽ�����
* Description   --> none
* Input         --> REG_ADD��Ҫ�����Ĵ�����ַ
* Output        --> none
* Reaturn       --> ��ȡ���ļĴ�������ֵ 
******************************************************************************/
uint8_t DS3231_Read_Byte(uint8_t REG_ADD)
{
	uint8_t ReData=0;
	IIC_Start();
	if(!(IIC_Write_Byte(DS3231_Write_ADD)))	//����д������Ӧ��λ
	{
		IIC_Write_Byte(REG_ADD);	//ȷ��Ҫ�����ļĴ���
		IIC_Start();	//��������
		IIC_Write_Byte(DS3231_Read_ADD);	//���Ͷ�ȡ����
		ReData = IIC_Read_Byte1();	//��ȡ����
		IIC_NAck();	//���ͷ�Ӧ���źŽ������ݴ���
	}
	IIC_Stop();
	return ReData;
}
/******************************************************************************
* Function Name --> DS3231��ʱ�������Ĵ���������д�����ݻ��߶�ȡ����
* Description   --> ����д��n�ֽڻ���������ȡn�ֽ�����
* Input         --> REG_ADD��Ҫ�����Ĵ�����ʼ��ַ
*                   *WBuff��д�����ݻ���
*                   num��д����������
*                   mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_Operate_Register(uint8_t REG_ADD,uint8_t *pBuff,uint8_t num,uint8_t mode)
{
	uint8_t i;
	if(mode)	//��ȡ����
	{
		IIC_Start();
		if(!(IIC_Write_Byte(DS3231_Write_ADD)))	//����д������Ӧ��λ
		{
			IIC_Write_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
			IIC_Start();	//��������
			IIC_Write_Byte(DS3231_Read_ADD);	//���Ͷ�ȡ����
			for(i = 0;i < num;i++)
			{
				*pBuff = IIC_Read_Byte1();	//��ȡ����
				if(i == (num - 1))	IIC_NAck();	//���ͷ�Ӧ���ź�
				else IIC_Ack();	//����Ӧ���ź�
				pBuff++;
			}
		}
		IIC_Stop();	
	}
	else	//д������
	{		 	
		IIC_Start();
		if(!(IIC_Write_Byte(DS3231_Write_ADD)))	//����д������Ӧ��λ
		{
			IIC_Write_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
			for(i = 0;i < num;i++)
			{
				IIC_Write_Byte(*pBuff);	//д������
				pBuff++;
			}
		}
		IIC_Stop();
	}
}
/******************************************************************************
* Function Name --> DS3231��ȡ����д��ʱ����Ϣ
* Description   --> ����д��n�ֽڻ���������ȡn�ֽ�����
* Input         --> *pBuff��д�����ݻ���
*                   mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_ReadWrite_Time(uint8_t mode)
{
	uint8_t Time_Register[8];	//����ʱ�仺��
	
	if(mode)	//��ȡʱ����Ϣ
	{
		DS3231_Operate_Register(Address_second,Time_Register,7,1);	//�����ַ��0x00����ʼ��ȡʱ����������
		
		/******�����ݸ��Ƶ�ʱ��ṹ���У��������������******/
		DS3231_US_Buf[0] = Time_Register[0] & Shield_secondBit;	//������
		DS3231_US_Buf[1] = Time_Register[1] & Shield_minuteBit;	//��������
		DS3231_US_Buf[2] = Time_Register[2] & Shield_hourBit;	//Сʱ����
	  DS3231_US_Buf[3] = Time_Register[3] & Shield_weekBit;	//��������
		DS3231_US_Buf[4] = Time_Register[4] & Shield_dateBit;	//������
		DS3231_US_Buf[5] = Time_Register[5] & Shield_monthBit;	//������
		DS3231_US_Buf[6] = Time_Register[6] ;	//������
	}
	else
	{
		/******��ʱ��ṹ���и������ݽ���******/
		Time_Register[0] = DS3231_US_Buf[0];	//��
		Time_Register[1] = DS3231_US_Buf[1];	//����
		Time_Register[2] = DS3231_US_Buf[2];	//Сʱ
		Time_Register[3] = DS3231_US_Buf[3];	//����
		Time_Register[4] = DS3231_US_Buf[4];	//��
		Time_Register[5] = DS3231_US_Buf[5];	//��
		Time_Register[6] = DS3231_US_Buf[6];	//��
		
		DS3231_Operate_Register(Address_second,Time_Register,7,0);	//�����ַ��0x00����ʼд��ʱ����������
	}
}
/******************************************************************************
* Function Name --> ʱ��������ʼ��
* Description   --> none
* Input         --> *TimeVAL��RTCоƬ�Ĵ���ֵָ��
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_Time_Init(uint8_t *Time_ch)
{	
	//ʱ����������
	
	DS3231_Operate_Register(Address_second,Time_ch,7,0);	//���루0x00����ʼд��7������
	DS3231_Write_Byte(Address_control, OSC_Enable);
	DS3231_Write_Byte(Address_control_status, Clear_OSF_Flag);
}
/******************************************************************************
* Function Name --> DS3231��⺯��
* Description   --> ����ȡ����ʱ��������Ϣת����ASCII�󱣴浽ʱ���ʽ������
* Input         --> none
* Output        --> none
* Reaturn       --> 0: ����
*                   1: ������������Ҫ��ʼ��ʱ����Ϣ
******************************************************************************/
uint8_t DS3231_Check(void)
{
	if(DS3231_Read_Byte(Address_control_status) & 0x80)  //����ֹͣ������
	{
		return 1;  //�쳣
	}
	else if(DS3231_Read_Byte(Address_control) & 0x80)  //���� EOSC����ֹ��
	{
		return 1;  //�쳣
	}
	else	return 0;  //����
}
DS3231 ds3231;
/******************************************************************************
* Function Name --> ʱ���������ݴ�����
* Description   --> ����ȡ����ʱ��������Ϣת����ASCII�󱣴浽ʱ���ʽ������
* Input         --> none
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void Time_Handle(void)
{
	/******************************************************
	                   ��ȡʱ��������Ϣ
	******************************************************/
	
	DS3231_ReadWrite_Time(1);	//��ȡʱ����������
	
	/******************************************************
	            ʱ����Ϣת��ΪASCII������ַ�
	******************************************************/
	
//	if(DS3231_US_Buf[6]<50&&DS3231_US_Buf[3]<8)
	{
		sprintf(ds3231.Time,"%02x:%02x:%02x",DS3231_US_Buf[2],DS3231_US_Buf[1],DS3231_US_Buf[0]);
		sprintf(ds3231.Timens,"%02x:%02x",DS3231_US_Buf[2],DS3231_US_Buf[1]);
		
		sprintf(ds3231.Hour,"%02x",DS3231_US_Buf[2]);
		sprintf(ds3231.Min,"%02x",DS3231_US_Buf[1]);
		sprintf(ds3231.Sec,"%02x",DS3231_US_Buf[0]);
		
		switch(DS3231_US_Buf[3])
		{
			case 1:sprintf(ds3231.Week,"MONDAY");break;
			case 2:sprintf(ds3231.Week,"TUESDAY");break;
			case 3:sprintf(ds3231.Week,"WEDNESDAY");break;
			case 4:sprintf(ds3231.Week,"THURSDAY");break;
			case 5:sprintf(ds3231.Week,"FRIDAY");break;
			case 6:sprintf(ds3231.Week,"SATURDAY");break;
			case 7:sprintf(ds3231.Week,"SUNDAY");break;
		}
		switch(DS3231_US_Buf[3])
		{
			case 1:sprintf(ds3231.Weekm,"MON");break;
			case 2:sprintf(ds3231.Weekm,"TUE");break;
			case 3:sprintf(ds3231.Weekm,"WED");break;
			case 4:sprintf(ds3231.Weekm,"THU");break;
			case 5:sprintf(ds3231.Weekm,"FRI");break;
			case 6:sprintf(ds3231.Weekm,"SAT");break;
			case 7:sprintf(ds3231.Weekm,"SUN");break;
		}
		
		switch(DS3231_US_Buf[5])
		{
			case 1:sprintf(ds3231.Monm,"JAN");break;
			case 2:sprintf(ds3231.Monm,"FEB");break;
			case 3:sprintf(ds3231.Monm,"MAR");break;
			case 4:sprintf(ds3231.Monm,"APR");break;
			case 5:sprintf(ds3231.Monm,"MAY");break;
			case 6:sprintf(ds3231.Monm,"JUN");break;
			case 7:sprintf(ds3231.Monm,"JUL");break;
			case 8:sprintf(ds3231.Monm,"AUG");break;
			case 9:sprintf(ds3231.Monm,"SEP");break;
			case 0x10:sprintf(ds3231.Monm,"OCT");break;
			case 0x11:sprintf(ds3231.Monm,"NOV");break;
			case 0x12:sprintf(ds3231.Monm,"DEC");break;
		}
		switch(DS3231_US_Buf[5])
		{
			case 1:sprintf(ds3231.Mon,"JANUARY");break;
			case 2:sprintf(ds3231.Mon,"FEBRUARY");break;
			case 3:sprintf(ds3231.Mon,"MARCH");break;
			case 4:sprintf(ds3231.Mon,"APRIL");break;
			case 5:sprintf(ds3231.Mon,"MAY");break;
			case 6:sprintf(ds3231.Mon,"JUNE");break;
			case 7:sprintf(ds3231.Mon,"JULY");break;
			case 8:sprintf(ds3231.Mon,"AUGUST");break;
			case 9:sprintf(ds3231.Mon,"SEPTEMBER");break;
			case 0x10:sprintf(ds3231.Mon,"OCTOBER");break;
			case 0x11:sprintf(ds3231.Mon,"NOVEMBER");break;
			case 0x12:sprintf(ds3231.Mon,"DECEMBER");break;
		}
		
		sprintf(ds3231.Day,"%02X",DS3231_US_Buf[4]);
		sprintf(ds3231.Year,"20%02X",DS3231_US_Buf[6]);
	}
	
}
/******************************************************************************
* Function Name --> ��ȡоƬ�¶ȼĴ���
* Description   --> �¶ȼĴ�����ַΪ0x11��0x12�������Ĵ���Ϊֻ��
* Input         --> none
* Output        --> *Temp�������¶���ʾ�ַ�����
* Reaturn       --> none
******************************************************************************/
void DS3231_Read_Temp(uint8_t *Temp)
{
	uint8_t temph,templ;
	float temp_dec;

	temph = DS3231_Read_Byte(Address_temp_MSB);	//��ȡ�¶ȸ�8bits
	templ = DS3231_Read_Byte(Address_temp_LSB) >> 6;	//��ȡ�¶ȵ�2bits

	//�¶�ֵת��
	if(temph & 0x80)	//�ж��¶�ֵ������
	{	//���¶�ֵ
		temph = ~temph;	//��λȡ��
		templ = ~templ + 0x01;	//��λȡ����1
		Temp[0] = 0x2d;	//��ʾ��-��
	}
	else	Temp[0] = 0x20;	//���¶Ȳ���ʾ���ţ���ʾ������0x2b

	//С�����ּ��㴦��
	temp_dec = (float)templ * (float)0.25;	//0.25��ֱ���

	//�������ּ��㴦��
	temph = temph & 0x70;	//ȥ������λ
	Temp[1] = temph % 1000 / 100 + 0x30;	//��λ
	Temp[2] = temph % 100 / 10 + 0x30;	//ʮλ
	Temp[3] = temph % 10 + 0x30;	//��λ
	Temp[4] = 0x2e;	//.

	//С�����ִ���
	Temp[5] = (uint8_t)(temp_dec * 10) + 0x30;	//С�����һλ
	Temp[6] = (uint8_t)(temp_dec * 100) % 10 + 0x30;	//С������λ

	if(Temp[1] == 0x30)	Temp[1] = 0x20;	//��λΪ0ʱ����ʾ
	if(Temp[2] == 0x30)	Temp[2] = 0x20;	//ʮλΪ0ʱ����ʾ
}
void DS3231_SetUart(void)
{
	static int runcount=0;
	if(!(runcount%600))
	{
		runcount=0;
		
		if(Device_Msg.uartday&&Device_Msg.uartmonth&&Device_Msg.uartweek<=7)
		{
			DS3231_Init_Buf[0] = HEX2BCD(Device_Msg.uartsecond);//��
			DS3231_Init_Buf[1] = HEX2BCD(Device_Msg.uartminute);//��
			DS3231_Init_Buf[2] = HEX2BCD(Device_Msg.uarthour);//ʱ
			DS3231_Init_Buf[4] = HEX2BCD(Device_Msg.uartday);//��
			DS3231_Init_Buf[5] = HEX2BCD(Device_Msg.uartmonth);//��
			if (Device_Msg.uartweek == 0)
				DS3231_Init_Buf[3] = 7;//����
			else
				DS3231_Init_Buf[3] = HEX2BCD(Device_Msg.uartweek & 0x7);//����
			DS3231_Init_Buf[6] = HEX2BCD(Device_Msg.uartyear - 2000);//��
			DS3231_Time_Init(DS3231_Init_Buf);
	//		printf("SetOK\r\n");
		}
	}
}
//int getweek(int y,int m,int d)
//{
//	 if(m==1||m==2) m+=12,y=y-1;
//	return (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400+1)%7;
//}

//void DS3231_SetWiFi(void)
//{
//	uint8_t week;
//	if(WiFi_Msg.day&&WiFi_Msg.month)
//	{
//		DS3231_Init_Buf[0] = HEX2BCD(WiFi_Msg.second);//��
//		DS3231_Init_Buf[1] = HEX2BCD(WiFi_Msg.minute);//��
//		DS3231_Init_Buf[2] = HEX2BCD(WiFi_Msg.hour);//ʱ
//		DS3231_Init_Buf[4] = HEX2BCD(WiFi_Msg.day);//��
//		DS3231_Init_Buf[5] = HEX2BCD(WiFi_Msg.month);//��
//		week = getweek(WiFi_Msg.year,WiFi_Msg.month,WiFi_Msg.day);
//		if (week == 0)
//			DS3231_Init_Buf[3] = 7;//����
//		else
//			DS3231_Init_Buf[3] = HEX2BCD(week);//����
//		DS3231_Init_Buf[6] = HEX2BCD(WiFi_Msg.year - 2000);//��
//		DS3231_Time_Init(DS3231_Init_Buf);
//		
//	}
//}
