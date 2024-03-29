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
* @ Brief     --> 高精度始终芯片DS3231驱动函数
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
                               定义显示时间格式
                         要改变显示的格式请修改此数组
******************************************************************************/

//时间显示缓存   格式  00:00:00

/******************************************************************************
                               定义相关的变量函数
******************************************************************************/

uint8_t DS3231_US_Buf[7];	//时间日历数据缓存
uint8_t DS3231_Init_Buf[7] = { 0x55, 0x11, 0x10, 0x01, 0x26, 0x06, 0x18 };


/*I2C读取一个字节*/
//uint8_t IIC_Read_Byte(void)
//{
//	uint8_t i;
//	uint8_t x=0;

//	SDA_H();	//首先置数据线为高电平

//	IIC_SDA_IN();	//设置成输入

//	for(i = 0;i < 8;i++)
//	{
//		x <<= 1;	//读入数据，高位在前

//		delay_us(2);
//		SCL_H();	//突变
//		delay_us(2);
//		
//		if(IN_SDA())	x |= 0x01;	//收到高电平

//		SCL_L();
//		delay_us(2);
//	}	//数据接收完成

//	SCL_L();

//	return x;	//返回读取到的数据
//}

/********************************************************************************************************
** 	函数名称:			BCD2HEX(uint8_t val)
**	功能描述:			BCD转HEX
********************************************************************************************************/
uint8_t	BCD2HEX(uint8_t val)
{
	return	((val >> 4) * 10) + (val & 0x0f);
}
/********************************************************************************************************
**	功能描述:			HEX转BCD
**	入口参数:			val:HAX码
********************************************************************************************************/
uint8_t	HEX2BCD(uint8_t val)
{
	return	(((val % 100) / 10) << 4) | (val % 10);
}

/******************************************************************************
* Function Name --> DS3231某寄存器写入一个字节数据
* Description   --> none
* Input         --> REG_ADD：要操作寄存器地址
*                   dat：要写入的数据
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void DS3231_Write_Byte(uint8_t REG_ADD,uint8_t dat)
{
	IIC_Start();
	if(!(IIC_Write_Byte(DS3231_Write_ADD)))	//发送写命令并检查应答位
	{
		IIC_Write_Byte(REG_ADD);
		IIC_Write_Byte(dat);	//发送数据
	}
	IIC_Stop();
}
/******************************************************************************
* Function Name --> DS3231某寄存器读取一个字节数据
* Description   --> none
* Input         --> REG_ADD：要操作寄存器地址
* Output        --> none
* Reaturn       --> 读取到的寄存器的数值 
******************************************************************************/
uint8_t DS3231_Read_Byte(uint8_t REG_ADD)
{
	uint8_t ReData=0;
	IIC_Start();
	if(!(IIC_Write_Byte(DS3231_Write_ADD)))	//发送写命令并检查应答位
	{
		IIC_Write_Byte(REG_ADD);	//确定要操作的寄存器
		IIC_Start();	//重启总线
		IIC_Write_Byte(DS3231_Read_ADD);	//发送读取命令
		ReData = IIC_Read_Byte1();	//读取数据
		IIC_NAck();	//发送非应答信号结束数据传送
	}
	IIC_Stop();
	return ReData;
}
/******************************************************************************
* Function Name --> DS3231对时间日历寄存器操作，写入数据或者读取数据
* Description   --> 连续写入n字节或者连续读取n字节数据
* Input         --> REG_ADD：要操作寄存器起始地址
*                   *WBuff：写入数据缓存
*                   num：写入数据数量
*                   mode：操作模式。0：写入数据操作。1：读取数据操作
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_Operate_Register(uint8_t REG_ADD,uint8_t *pBuff,uint8_t num,uint8_t mode)
{
	uint8_t i;
	if(mode)	//读取数据
	{
		IIC_Start();
		if(!(IIC_Write_Byte(DS3231_Write_ADD)))	//发送写命令并检查应答位
		{
			IIC_Write_Byte(REG_ADD);	//定位起始寄存器地址
			IIC_Start();	//重启总线
			IIC_Write_Byte(DS3231_Read_ADD);	//发送读取命令
			for(i = 0;i < num;i++)
			{
				*pBuff = IIC_Read_Byte1();	//读取数据
				if(i == (num - 1))	IIC_NAck();	//发送非应答信号
				else IIC_Ack();	//发送应答信号
				pBuff++;
			}
		}
		IIC_Stop();	
	}
	else	//写入数据
	{		 	
		IIC_Start();
		if(!(IIC_Write_Byte(DS3231_Write_ADD)))	//发送写命令并检查应答位
		{
			IIC_Write_Byte(REG_ADD);	//定位起始寄存器地址
			for(i = 0;i < num;i++)
			{
				IIC_Write_Byte(*pBuff);	//写入数据
				pBuff++;
			}
		}
		IIC_Stop();
	}
}
/******************************************************************************
* Function Name --> DS3231读取或者写入时间信息
* Description   --> 连续写入n字节或者连续读取n字节数据
* Input         --> *pBuff：写入数据缓存
*                   mode：操作模式。0：写入数据操作。1：读取数据操作
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_ReadWrite_Time(uint8_t mode)
{
	uint8_t Time_Register[8];	//定义时间缓存
	
	if(mode)	//读取时间信息
	{
		DS3231_Operate_Register(Address_second,Time_Register,7,1);	//从秒地址（0x00）开始读取时间日历数据
		
		/******将数据复制到时间结构体中，方便后面程序调用******/
		DS3231_US_Buf[0] = Time_Register[0] & Shield_secondBit;	//秒数据
		DS3231_US_Buf[1] = Time_Register[1] & Shield_minuteBit;	//分钟数据
		DS3231_US_Buf[2] = Time_Register[2] & Shield_hourBit;	//小时数据
	  DS3231_US_Buf[3] = Time_Register[3] & Shield_weekBit;	//星期数据
		DS3231_US_Buf[4] = Time_Register[4] & Shield_dateBit;	//日数据
		DS3231_US_Buf[5] = Time_Register[5] & Shield_monthBit;	//月数据
		DS3231_US_Buf[6] = Time_Register[6] ;	//年数据
	}
	else
	{
		/******从时间结构体中复制数据进来******/
		Time_Register[0] = DS3231_US_Buf[0];	//秒
		Time_Register[1] = DS3231_US_Buf[1];	//分钟
		Time_Register[2] = DS3231_US_Buf[2];	//小时
		Time_Register[3] = DS3231_US_Buf[3];	//星期
		Time_Register[4] = DS3231_US_Buf[4];	//日
		Time_Register[5] = DS3231_US_Buf[5];	//月
		Time_Register[6] = DS3231_US_Buf[6];	//年
		
		DS3231_Operate_Register(Address_second,Time_Register,7,0);	//从秒地址（0x00）开始写入时间日历数据
	}
}
/******************************************************************************
* Function Name --> 时间日历初始化
* Description   --> none
* Input         --> *TimeVAL：RTC芯片寄存器值指针
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS3231_Time_Init(uint8_t *Time_ch)
{	
	//时间日历数据
	
	DS3231_Operate_Register(Address_second,Time_ch,7,0);	//从秒（0x00）开始写入7组数据
	DS3231_Write_Byte(Address_control, OSC_Enable);
	DS3231_Write_Byte(Address_control_status, Clear_OSF_Flag);
}
/******************************************************************************
* Function Name --> DS3231检测函数
* Description   --> 将读取到的时间日期信息转换成ASCII后保存到时间格式数组中
* Input         --> none
* Output        --> none
* Reaturn       --> 0: 正常
*                   1: 不正常或者需要初始化时间信息
******************************************************************************/
uint8_t DS3231_Check(void)
{
	if(DS3231_Read_Byte(Address_control_status) & 0x80)  //晶振停止工作了
	{
		return 1;  //异常
	}
	else if(DS3231_Read_Byte(Address_control) & 0x80)  //或者 EOSC被禁止了
	{
		return 1;  //异常
	}
	else	return 0;  //正常
}
DS3231 ds3231;
/******************************************************************************
* Function Name --> 时间日历数据处理函数
* Description   --> 将读取到的时间日期信息转换成ASCII后保存到时间格式数组中
* Input         --> none
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void Time_Handle(void)
{
	/******************************************************
	                   读取时间日期信息
	******************************************************/
	
	DS3231_ReadWrite_Time(1);	//获取时间日历数据
	
	/******************************************************
	            时间信息转换为ASCII码可视字符
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
* Function Name --> 读取芯片温度寄存器
* Description   --> 温度寄存器地址为0x11和0x12，这两寄存器为只读
* Input         --> none
* Output        --> *Temp：最终温度显示字符缓存
* Reaturn       --> none
******************************************************************************/
void DS3231_Read_Temp(uint8_t *Temp)
{
	uint8_t temph,templ;
	float temp_dec;

	temph = DS3231_Read_Byte(Address_temp_MSB);	//读取温度高8bits
	templ = DS3231_Read_Byte(Address_temp_LSB) >> 6;	//读取温度低2bits

	//温度值转换
	if(temph & 0x80)	//判断温度值的正负
	{	//负温度值
		temph = ~temph;	//高位取反
		templ = ~templ + 0x01;	//低位取反加1
		Temp[0] = 0x2d;	//显示“-”
	}
	else	Temp[0] = 0x20;	//正温度不显示符号，显示正号填0x2b

	//小数部分计算处理
	temp_dec = (float)templ * (float)0.25;	//0.25℃分辨率

	//整数部分计算处理
	temph = temph & 0x70;	//去掉符号位
	Temp[1] = temph % 1000 / 100 + 0x30;	//百位
	Temp[2] = temph % 100 / 10 + 0x30;	//十位
	Temp[3] = temph % 10 + 0x30;	//个位
	Temp[4] = 0x2e;	//.

	//小数部分处理
	Temp[5] = (uint8_t)(temp_dec * 10) + 0x30;	//小数点后一位
	Temp[6] = (uint8_t)(temp_dec * 100) % 10 + 0x30;	//小数点后二位

	if(Temp[1] == 0x30)	Temp[1] = 0x20;	//百位为0时不显示
	if(Temp[2] == 0x30)	Temp[2] = 0x20;	//十位为0时不显示
}
void DS3231_SetUart(void)
{
	static int hourflag=0;
	if(hourflag!=Device_Msg.uarthour)
	{
		hourflag=Device_Msg.uarthour;
		if(Device_Msg.uartday&&Device_Msg.uartmonth&&Device_Msg.uartweek<=7)
		{
			DS3231_Init_Buf[0] = HEX2BCD(Device_Msg.uartsecond);//秒
			DS3231_Init_Buf[1] = HEX2BCD(Device_Msg.uartminute);//分
			DS3231_Init_Buf[2] = HEX2BCD(Device_Msg.uarthour);//时
			DS3231_Init_Buf[4] = HEX2BCD(Device_Msg.uartday);//日
			DS3231_Init_Buf[5] = HEX2BCD(Device_Msg.uartmonth);//月
			if (Device_Msg.uartweek == 0)
				DS3231_Init_Buf[3] = 7;//星期
			else
				DS3231_Init_Buf[3] = HEX2BCD(Device_Msg.uartweek & 0x7);//星期
			DS3231_Init_Buf[6] = HEX2BCD(Device_Msg.uartyear - 2000);//年
			DS3231_Time_Init(DS3231_Init_Buf);
			printf("SetOK\r\n");
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
//		DS3231_Init_Buf[0] = HEX2BCD(WiFi_Msg.second);//秒
//		DS3231_Init_Buf[1] = HEX2BCD(WiFi_Msg.minute);//分
//		DS3231_Init_Buf[2] = HEX2BCD(WiFi_Msg.hour);//时
//		DS3231_Init_Buf[4] = HEX2BCD(WiFi_Msg.day);//日
//		DS3231_Init_Buf[5] = HEX2BCD(WiFi_Msg.month);//月
//		week = getweek(WiFi_Msg.year,WiFi_Msg.month,WiFi_Msg.day);
//		if (week == 0)
//			DS3231_Init_Buf[3] = 7;//星期
//		else
//			DS3231_Init_Buf[3] = HEX2BCD(week);//星期
//		DS3231_Init_Buf[6] = HEX2BCD(WiFi_Msg.year - 2000);//年
//		DS3231_Time_Init(DS3231_Init_Buf);
//		
//	}
//}
