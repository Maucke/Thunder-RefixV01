/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

#include <stdio.h>
#define Uart_Max_Length 250
#define Uart_FFT_Length 192
extern uint8_t Uart_Recv1_Buf[];
extern uint8_t Uart_Overflow1_Flag;
extern uint16_t Uart_Recv1_Length;

extern uint8_t Uart_Recv3_Buf[];
extern uint8_t Uart_Overflow3_Flag;
extern uint16_t Uart_Recv3_Length;
#define End_Frame_ADDR 0x55AA
extern char Device_Name[20];
extern u8 Uart_Recv3_Data;
typedef struct
{
	u8 hour;
	u8 minute;
	u8 cation;//֪ͨ
	u8 mute;//���� NO use
	u8 alarm;//����
	u8 tmsys;
	u8 wifi;//WiFi״̬
	u8 aiwork;
} CLOCK_MSG;

extern CLOCK_MSG Clock_Msg;

#define CPU_Temp 						0x1
#define CPU_Clock 					0x2
#define CPU_Load 						0x3
#define CPU_Fan 						0x4

#define GPU_Temp 						0x11
#define GPU_Clock 					0x12
#define GPU_Load 						0x13
#define GPU_Fan 						0x14

#define Main_Temp 					0x21
#define Main_Fan 						0x22
#define Main_Vol						0x23

#define RAM_Load 	  				0x31
#define RAM_Data 						0x32

#define HDD_Load 	  				0x41
#define HDD_Temp 						0x42

#define Uart_Year 	 			 	0x201
#define Uart_Month 					0x202
#define Uart_Day 	  				0x203
#define Uart_Week 					0x204
#define Uart_Hour 	  			0x205
#define Uart_Minute 				0x206
#define Uart_Second 	  		0x207
#define CPU_POWER 	  			0x208

#define CPU_NAME 	 		 		0x101
#define GPU_NAME 	 		 		0x102
#define Main_NAME 	 		 	0x103
#define Uart_END	 	  		0x55AA

#define Command_Mode      0x8001
#define Command_Motion    0x8002
#define Command_Style     0x8003
#define Command_Speed     0x8004
#define Command_Brightness 0x8005
#define Command_Set       0x8006
#define Command_RGBMode   0x8007
#define Command_RGBColor  0x8008
#define Command_RGBBrightness 0x8009

#define Command_TOPTHEME 	0x800A
#define Command_LOGOTHEME 0x800B
#define Command_GAMETYPE 0x800C
#define Command_DEVICENAME 0x400A

//#define FLASH_SAVE_ADDR  0X0807f000 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
#define FLASH_NAME_ADDR  0x0807D000 
#define FLASH_SAVE_ADDR  0x0803D000 

#define FFT_Data 0x0301
#define Left_Vol 0x0302
#define Right_Vol 0x0303

//			SendValue(0x8001, Convert.ToUInt16(Ctl.Mode)); // ��ʾģʽ
//			Thread.Sleep(2);
//			SendValue(0x8002, Convert.ToUInt16(Ctl.Motion)); // ��ʾģʽ
//			Thread.Sleep(2);
//			SendValue(0x8003, Convert.ToUInt16(Ctl.Style)); // ��ʾģʽ
//			Thread.Sleep(2);
//			SendValue(0x8004, Convert.ToUInt16(Ctl.Speed)); // ��ʾģʽ
//			Thread.Sleep(2);
typedef struct
{
	u16 cputemp;
	u16 cpuclock;
	u16 cpuload;
	u16 cpufan;
	u16 cpupower;

	u16 gputemp;
	u16 gpuclock;
	u16 gpuload;
	u16 gpufan;

	u16 maintemp;
	u16 mainfan;
	u16 mainvoltage;

	u16 ramload;
	u16 ramusrdata;

	u16 hddload;
	u16 hddtemp;

	u16 uartyear;
	u16 uartmonth;
	u16 uartday;
	u16 uartweek;
	u16 uarthour;
	u16 uartminute;
	u16 uartsecond;

	char cpuname[40];
	char gpuname[40];
	char mainname[40];
	u8 fft[Uart_FFT_Length];
	u16 leftvol;
	u16 rightvol;
}DEVICE_MSG;

typedef struct
{
	u16 commandmode;
	u16 commandmotion;
	u16 commandstyle;
	u16 commandspeed;
	u16 commandset;
	u16 commandbrightness;
	u16 commandrgbmode;
	u16 commandrgbcolor;
	u16 commandrgbbrightness;
	u16 commandtoptheme;
	u16 commandlogotheme;
	u16 commandgametype;
	u16 commandtimetheme;
}DEVICE_CMD;


typedef struct
{
	char cputemp[10];	
	char cpuclock[10];	
	char cpuload[10];	
	char cpufan[10];	

	char gputemp[10];	
	char gpuclock[10];	
	char gpuload[10];	
	char gpufan[10];		

	char maintemp[10];		
	char mainfan[10];

	char ramload[10];	
	char ramusrdata[10];
	
		
//	char vtime[10];
//	char vhour[10];
//	char vmin[10];	
//	char vsec[10];	
//	char date[10];	
	
}DEVICE_STR;


extern u8 SaveFlag;
extern DEVICE_STR Device_Str;
extern DEVICE_STR Device_NStr;
extern DEVICE_STR Device_VStr;
extern DEVICE_MSG Device_Msg;
extern DEVICE_CMD Device_Cmd;
extern uint8_t OfflineCount;
extern u8 WifiActive;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern uint8_t Uart_Recv1_Data;
extern uint8_t Uart_Recv3_Data;
extern u16 own_hour,own_min,own_sec;
void ConvertData(void);
void InitData(void);
void InitWifi(void);
void AnalysisComputermsg(uint8_t *Buf);
void AnalysisComputername(uint8_t *Buf);
void AnalysisMiStaus(uint8_t Data);
void AnalysisFFT(uint8_t *Buf);
void AnalysisCommand(uint8_t *Buf);
void ClearFFT(void);

void drache_cmd(UART_HandleTypeDef *huart,u16 addr,u16 data);
int drache_printf(UART_HandleTypeDef *huart,const char *pcFormat, ...);
void drache_clear(UART_HandleTypeDef *huart);

void Tranfcmd(void);
void Recvcmd(void);
void AnalysisWiFiString(uint8_t *Buf);
void AnalysisWiFiInter(uint8_t *Buf);
void AnalysisKey(uint8_t *Buf);

void SendCommand(void);
int cont_str(char *s);
void delay_ms(unsigned int Ms);
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern uint8_t Uart_Recv1_Data;
extern uint8_t Uart_Recv3_Data;
extern u16 own_hour,own_min,own_sec;
void ConvertData(void);
void InitData(void);
void InitWifi(void);
void AnalysisComputermsg(uint8_t *Buf);
void AnalysisComputername(uint8_t *Buf);
void AnalysisMiStaus(uint8_t Data);
void AnalysisFFT(uint8_t *Buf);
void AnalysisCommand(uint8_t *Buf);
void ClearFFT(void);

void drache_cmd(UART_HandleTypeDef *huart,u16 addr,u16 data);
int drache_printf(UART_HandleTypeDef *huart,const char *pcFormat, ...);
void drache_clear(UART_HandleTypeDef *huart);

void Tranfcmd(void);
void Recvcmd(void);
void AnalysisWiFiString(uint8_t *Buf);
void AnalysisWiFiInter(uint8_t *Buf);
void AnalysisKey(uint8_t *Buf);

void SendCommand(void);
int cont_str(char *s);
void delay_ms(unsigned int Ms);
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
