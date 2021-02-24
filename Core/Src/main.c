/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED_Animation.h"
#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include "OLED_FFT.h"
#include "OLED_UI.h"
#include "ds3231.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "stm32_dsp.h"
#include "stmflash.h"
#include "table_fft.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define FLASH_SAVE_ADDR  0x0803D000 	//设置FLASH 保存地址(必须为4的倍数，且所在扇区,要大于本代码所占用到的扇区.

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t Flag_Refrash = False;
uint8_t Flag_Blink = False;
uint8_t Flag_Sleep = False;
uint8_t Flag_Continue = False;
uint8_t Flag_Reception = True;
uint8_t OfflineCount = 6;
uint8_t SystemActive = False;
u8 datatemp[256] = {};
u16 RandomX=30,RandomY=30;

u16 Display_Mode = MODE_CHROME;
u16 Current_Mode = MODE_OFFLINE;
	
OLED_GFX oled = OLED_GFX();
OLED_FFT fft = OLED_FFT();
OLED_Animation motion = OLED_Animation();
OLED_UI ui = OLED_UI();
SAVE save={1,1};

//#define Radius 60
//#define RadiusC 56
//#define RadiusB 53
//#define HourRadius 30
//#define MinRadius 40
//#define SecRadius 50
u16 ColorPointer[3];

uint32_t adc_buf[NPT]={0};			//用于存储ADC转换结果的数组	

static long lBufInArray[NPT];					//传入给FFT的数组
//long lBufOutArray[NPT/2];				//FFT输出 因为输出结果是对称的 所以我们只取了前面的一半
//long lBufMagArray[NPT/2];				//每个频率对用的幅值
static long lBufOutArray[NPT];				//FFT输出 
long lBufMagArray[NPT];				//每个频率对用的幅值

void FFT_Start(void)
{
	/*启动ADC的DMA传输，配合定时器触发ADC转换*/
	HAL_ADC_Start_DMA(&hadc1, adc_buf, NPT);
	/*开启定时器，用溢出时间来触发ADC*/
	HAL_TIM_Base_Start(&htim3);
}

void FFT_Stop(void)
{
	/*停止ADC的DMA传输*/
	HAL_ADC_Stop_DMA(&hadc1);
	/*停止定时器*/
	HAL_TIM_Base_Stop(&htim3);
}

void GetPowerMag(void)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
	
    for(i=0; i<NPT; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
			
				//除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0f/ NPT;
        if(i == 0)	
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}

/* 函数名称：void FFT_Pro(void)
 * 功能描述：FFT处理函数
 * 参数：无
 * 返回值：无
 */
void FFT_Pro(void)
{
	uint16_t i = 0;
	//填充数组
	for(i=0;i<NPT;i++)
	{
		//这里因为单片机的ADC只能测正的电压 所以需要前级加直流偏执
		//加入直流偏执后 1.25V 对应AD值为3103
		lBufInArray[i] = ((signed short)(adc_buf[i])-1551) << 18;		
	}
	//256点FFT变换
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
	//计算出对应频率的模 即每个频率对应的幅值
	GetPowerMag();	
}

////发送加速度传感器数据+陀螺仪数据(传感器帧)
////aacx,aacy,aacz:x,y,z三个方向上面的加速度值
////gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值 
//void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
//{
//	u8 tbuf[18]; 
//	tbuf[0]=(aacx>>8)&0XFF;
//	tbuf[1]=aacx&0XFF;
//	tbuf[2]=(aacy>>8)&0XFF;
//	tbuf[3]=aacy&0XFF;
//	tbuf[4]=(aacz>>8)&0XFF;
//	tbuf[5]=aacz&0XFF; 
//	tbuf[6]=(gyrox>>8)&0XFF;
//	tbuf[7]=gyrox&0XFF;
//	tbuf[8]=(gyroy>>8)&0XFF;
//	tbuf[9]=gyroy&0XFF;
//	tbuf[10]=(gyroz>>8)&0XFF;
//	tbuf[11]=gyroz&0XFF;
//	tbuf[12]=0;//因为开启MPL后,无法直接读取磁力计数据,所以这里直接屏蔽掉.用0替代.
//	tbuf[13]=0;
//	tbuf[14]=0;
//	tbuf[15]=0;
//	tbuf[16]=0;
//	tbuf[17]=0;
//	
//	HAL_UART_Transmit(&huart1, (uint8_t *)&tbuf, 18, 0xffff);
//}	
////通过串口1上报结算后的姿态数据给电脑(状态帧)
////roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
////pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
////yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
////csb:超声波高度,单位:cm
////prs:气压计高度,单位:mm
//void usart1_report_imu(short roll,short pitch,short yaw,short csb,int prs)
//{
//	u8 tbuf[12];   	
//	tbuf[0]=(roll>>8)&0XFF;
//	tbuf[1]=roll&0XFF;
//	tbuf[2]=(pitch>>8)&0XFF;
//	tbuf[3]=pitch&0XFF;
//	tbuf[4]=(yaw>>8)&0XFF;
//	tbuf[5]=yaw&0XFF;
//	tbuf[6]=(csb>>8)&0XFF;
//	tbuf[7]=csb&0XFF;
//	tbuf[8]=(prs>>24)&0XFF;
//	tbuf[9]=(prs>>16)&0XFF;
//	tbuf[10]=(prs>>8)&0XFF;
//	tbuf[11]=prs&0XFF;
////	usart1_niming_report(0X01,tbuf,12);//状态帧,0X01
//}  

static u8 TimeTHEME = 0;

void MainSysRun()
{
	static int runcount=0;
	
	if(Display_Mode==Current_Mode&&TimeTHEME==Device_Cmd.commandtimetheme)
	{
		switch(Current_Mode)
		{
			case MODE_GAME:ui.SUIDataPrss();break;
			case MODE_NORMAL:ui.NUIDataPrss();break;
			case MODE_MUSIC:ui.MUIDataPrss();break;
			case MODE_DATE:switch(TimeTHEME) 
			{
				case 0:ui.TUIDataPrss();break;
				case 1:ui.T1UIDataPrss();break;
				case 2:ui.T2UIDataPrss();break;
				default:Device_Cmd.commandtimetheme=0;break;
			}break;
			case MODE_SHOW:ui.HUIDataPrss();break;
			case MODE_CHROME:ui.GAMEUIDataPrss();break;
			case MODE_OFFLINE:break;
//			default:ui.SUIDataPrss();break;
		}
		
		return;
	}
	else
	{
		runcount++;
		if(runcount==1)
		{
			switch(Current_Mode)
			{
				case MODE_GAME:ui.SUI_Out();;break;
				case MODE_NORMAL:ui.NUI_Out();break;
				case MODE_MUSIC:FFT_Stop();ui.MUI_Out();break;
				case MODE_DATE:switch(TimeTHEME) 
				{
					case 0:ui.TUI_Out();break;
					case 1:ui.T1UI_Out();break;
					case 2:ui.T2UI_Out();break;
					default:Device_Cmd.commandtimetheme=0;break;
				}break;
				case MODE_SHOW:ui.HUI_Out();break;
				case MODE_CHROME:ui.GAMEUI_Out();break;
				case MODE_OFFLINE:break;
//				default:ui.SUI_Out();break;
			}
		}
		else if(runcount>60)
		{
			runcount=0;
			Current_Mode=Display_Mode;
			TimeTHEME=Device_Cmd.commandtimetheme;
			switch(Current_Mode)
			{
				case MODE_GAME:ui.SUI_In();;break;
				case MODE_NORMAL:ui.NUI_In();break;
				case MODE_MUSIC:FFT_Start();ui.MUI_In();break;
				case MODE_DATE:switch(TimeTHEME) 
				{
					case 0:ui.TUI_In();break;
					case 1:ui.T1UI_In();break;
					case 2:ui.T2UI_In();break;
					default:Device_Cmd.commandtimetheme=0;break;
				}break;
				case MODE_SHOW:ui.HUI_In();break;
				case MODE_CHROME:ui.GAMEUI_In();break;
				case MODE_OFFLINE:break;
//				default:ui.SUI_In();break;
			}
		}
	}
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int Count=0;

u8 Dataleng;
//char DataDis[20];
//char DataDisf[3];

u8 showfpsflag = 0;
//void CopyString(char *ch1,char *ch2)
//{
//	int i;
//	for(i=0;i<cont_str(ch2);i++)
//		ch1[i]=ch2[i];
//}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


//void savedata(void)
//{
//	u32 msg[2];
//	msg[0] = Device_Cmd.commandstyle;
//	msg[1] = Device_Cmd.commandmotion;
//	
//	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)msg,2);
//}
//void getdata(void)
//{
//	u32 msg[2];
//	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)msg,2);
//	Device_Cmd.commandstyle = msg[0];
//	Device_Cmd.commandmotion = msg[1];
//}
u8 sleepflag=0;
void module_pwr_enter_sleep_mode(void)
{
	sleepflag=1;
  oled.SCR_reg(0x06, 0x00);//shutdown screen
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  GPIO_InitStruct.Pin = SYSLED_Pin;
  HAL_GPIO_Init(SYSLED_GPIO_Port, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = OLED_RST_Pin|OLED_DC_Pin|OLED_CS_Pin|GPIO_PIN_5|GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = DS_SDA_Pin|DS_SCL_Pin;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
//	drache_printf("begin sleep mode\r\n");
//	HAL_SuspendTick();
//	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);
//	oled.Clear_Screen();
//	oled.Refrash_Screen();
  HAL_GPIO_WritePin(GPIOB, OLED_PW_Pin, GPIO_PIN_RESET);//关闭升压
  HAL_GPIO_WritePin(GPIOA, OLED_RST_Pin|OLED_DC_Pin|OLED_CS_Pin|GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_RESET);//关闭

	HAL_TIM_Base_Stop_IT(&htim3);
	HAL_TIM_Base_Stop_IT(&htim4);
	HAL_TIM_Base_Stop_IT(&htim5);
	HAL_TIM_Base_Stop_IT(&htim9);
	HAL_ADC_Stop(&hadc1);
	HAL_SPI_DMAStop(&hspi1);
	Tranfcmd();
	
    /*进STOP模式*/
  __HAL_RCC_PWR_CLK_DISABLE();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	while(1);
//	HAL_ResumeTick();
}

void module_pwr_exit_sleep_mode(void)
{
	sleepflag=0;
//	HAL_ResumeTick();
//	drache_printf("exit sleep mode\r\n");
//  HAL_GPIO_WritePin(GPIOB, OLED_PW_Pin, GPIO_PIN_SET);//开启升压
	
  NVIC_SystemReset();           //软件重启
}


u8 key1UpFlag = False;
u8 key1_fall_flag = False;
u8 short_key1_flag = False;
u8 key1_long_down = False;
u8 long_key1_flag = False;
int key1_holdon_ms = 0;
int key1upCnt = 0;

u8 key2UpFlag = False;
u8 key2_fall_flag = False;
u8 short_key2_flag = False;
u8 key2_long_down = False;
u8 long_key2_flag = False;
int key2_holdon_ms = 0;
int key2upCnt = 0;

void KeyProcess(void)
{
//	return;
	if(short_key1_flag)
	{
		if(Current_Mode==MODE_CHROME) return;
		short_key1_flag=0;
		if(Display_Mode==MODE_MUSIC)
		{
			Device_Cmd.commandstyle++;			
			if(Device_Cmd.commandstyle>6)
				Device_Cmd.commandstyle=1;
			
//			oled.Display_hbmp(SCR_WIDTH/2-81/2,SCR_HEIGHT/2-26/2,81,26,themetop[Device_Cmd.commandstyle-1],0xFFFF);
		}
		else
		{
			Device_Cmd.commandtimetheme++;
			if(Device_Cmd.commandtimetheme>2)
				Device_Cmd.commandtimetheme=0;
//			oled.Display_hbmp(SCR_WIDTH/2-81/2,SCR_HEIGHT/2-26/2,81,26,themetop[Device_Cmd.commandtimetheme],0xFFFF);
		}
//		oled.Refrash_Screen();
//		Tranfcmd();
//		oled.Clear_Screen();
	}
	else if(key1_long_down)
	{
		if(Display_Mode==MODE_MUSIC)
			Display_Mode=MODE_CHROME;
		else if(Display_Mode==MODE_CHROME)
			Display_Mode=MODE_DATE;
		else if(Display_Mode==MODE_DATE)
			Display_Mode=MODE_MUSIC;
//		showfpsflag = 1-showfpsflag;
		key1_long_down=0;
	}
	if(short_key2_flag)
	{
		if(Current_Mode==MODE_CHROME) return;
		short_key2_flag=0;
		Device_Cmd.commandmotion++;			
		if(Device_Cmd.commandmotion>5)
			Device_Cmd.commandmotion=0;
		
//		oled.Display_hbmp(SCR_WIDTH/2-81/2,SCR_HEIGHT/2-26/2,81,26,animationtop[Device_Cmd.commandmotion],0xFFFF);
//		oled.Refrash_Screen();
//		Tranfcmd();
//		oled.Clear_Screen();
	}
	else if(key2_long_down)
	{
		module_pwr_enter_sleep_mode();
		key2_long_down=0;
	}
}
u16 fps = 0;
char fpschar[20];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

//	short temp;						//温度
//	float pitch,roll,yaw; 			//欧拉角
//	short aacx,aacy,aacz;			//加速度传感器原始数据
//	short gyrox,gyroy,gyroz;		//陀螺仪原始数据
//	float battVoltage;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart1,Uart_Recv1_Buf,Uart_Max_Length);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_Base_Start_IT(&htim9);
	drache_printf("Sys OK!\r\n");
  oled.Device_Init();
	Recvcmd();
	motion.OLED_AllMotion_Init();
	
//	Time_Handle();
//	MPU_Init();
//	mpu_dmp_init();
////	DS3231_Time_Init(DS3231_Init_Buf);
//	drache_printf("Time:%s\r\n",ds3231.Time); 
//	temp=MPU_Get_Temperature();	//得到温度值
//	drache_printf("temp:%d\r\n",temp);
	drache_cmd(0xA002,3);
//	HAL_RTC_MspInit(&hrtc);
//  RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); //配置WAKE UP中断,1秒钟中断一次
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		MUSIC_Mode();
//	Time_Handle();
////	DS3231_Time_Init(DS3231_Init_Buf);
//			drache_printf("Time:%s\r\n",ds3231.Time); 
//		    MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//		    MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
////				mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);
//				drache_printf("aacx=%x,aacy=%x,aacz=%x\r\n",aacx,aacy,aacz);
//				drache_printf("gyrox=%d,gyroy=%d,gyroz=%d\r\n",gyrox,gyroy,gyroz);
//		HAL_Delay(100);
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1, 50);
//		drache_printf("ADC:%X\r\n",HAL_ADC_GetValue(&hadc1));
//		while(!Flag_Refrash)__ASM("NOP");
		
//    battVoltage = (float)Get_Adc(ADC_CHANNEL_9) / 4095.0f * 3.3f * 2;
//		drache_printf("battVoltage:%0.1f\r\n",battVoltage);
		{
			Flag_Refrash=False;
			oled.Clear_Screen();
			motion.OLED_CustormMotion(Device_Cmd.commandmotion);
			KeyProcess();
			fps++;
			if(showfpsflag)
				oled.OLED_SHFAny(0,0,fpschar,19,0xffff);
			switch(Current_Mode)
			{
				case MODE_GAME:ui.SUIMainShow();break;
				case MODE_NORMAL:ui.NUIMainShow();break;
				case MODE_MUSIC:ui.MUIMainShow();break;
				case MODE_DATE:switch(TimeTHEME) 
				{
					case 0:ui.TUIMainShow();break;
					case 1:ui.T1UIMainShow();break;
					case 2:ui.T2UIMainShow();break;
					default:Device_Cmd.commandtimetheme=0;break;
	//				case 2:ui.T2UIMainShow();break;
				}
				break;
				case MODE_SHOW:ui.HUIMainShow();break;
				case MODE_CHROME:ui.GAMEUIMainShow();break;
				case MODE_OFFLINE:break;
	//			default:ui.SUIMainShow();break;
			}
			oled.Refrash_Screen();
		}
		HAL_Delay(5);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


uint8_t prt = 2;		
STA systemstatus = offline;
u16 offlinecount = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static u16 TimeRun = 0;
	static u16 TimeRun1 = 0;
	if (htim->Instance == htim4.Instance)
	{
		if(key1_fall_flag==1)//发生按键按下事件
		{
			if(!HAL_GPIO_ReadPin(KEY_L_GPIO_Port,KEY_L_Pin))//按键持续按下
			{         
				if(key1_holdon_ms <= 200)  
				{
					key1_holdon_ms++; 
				} 
				else //按键按下到2000ms就判断长按时间成立，生成长按标志 
				{ 
					key1_holdon_ms = 0; 
					short_key1_flag = 0;//清短按键标志
					key1_long_down = 1;//长按键标志置位
					key1_fall_flag = 0;//清按键按下标志 
				} 
			} 
			else //按键抬起
			{ 
				if(key1_holdon_ms>5)//按下时间大于50ms，生成单击标志
				{  
					key1_holdon_ms=0;
					short_key1_flag=1;
					key1_long_down =0;
					key1_fall_flag=0;

					//距离上次单击时间在100~500ms之间，则认为发生连击事件
//					if(key1upCnt>100 && key1upCnt<500)
//					{ 
//						doubleClick = True;
//						short_key1_flag=0;
//					} 
					key1UpFlag = True;//单击抬起按键后，生成按键抬起标志 
				} 
				else  //按键持续时间小于50ms，忽略 
				{    
					key1_holdon_ms=0; 
					short_key1_flag=0;
					long_key1_flag=0;
					key1_fall_flag=0;
				} 
			}
		}
		
		if(key1UpFlag)//单击抬起后，启动计数，计数到500ms  
			key1upCnt++;
		if(key1upCnt>50)
		{ 
			key1upCnt = 0;
			key1UpFlag = False;
		}
		
		
		if(key2_fall_flag==1)//发生按键按下事件
		{
			if(!HAL_GPIO_ReadPin(KEY_R_GPIO_Port,KEY_R_Pin))//按键持续按下
			{         
				if(key2_holdon_ms <= 200)  
				{
					key2_holdon_ms++; 
				} 
				else //按键按下到2000ms就判断长按时间成立，生成长按标志 
				{ 
					key2_holdon_ms = 0; 
					short_key2_flag=0;//清短按键标志
					key2_long_down = 1;//长按键标志置位
					key2_fall_flag = 0;//清按键按下标志 
				} 
			} 
			else //按键抬起
			{ 
				if(key2_holdon_ms>5)//按下时间大于50ms，生成单击标志
				{  
					key2_holdon_ms=0;
					short_key2_flag=1;
					key2_long_down =0;
					key2_fall_flag=0;

					//距离上次单击时间在100~500ms之间，则认为发生连击事件
//					if(key2upCnt>100 && key2upCnt<500)
//					{ 
//						doubleClick = True;
//						short_key2_flag=0;
//					} 
					key2UpFlag = True;//单击抬起按键后，生成按键抬起标志 
				} 
				else  //按键持续时间小于50ms，忽略 
				{    
					key2_holdon_ms=0; 
					short_key2_flag=0;
					long_key2_flag=0;
					key2_fall_flag=0;
				} 
			}
		}
		
		if(key2UpFlag)//单击抬起后，启动计数，计数到500ms  
			key2upCnt++;
		if(key2upCnt>50)
		{ 
			key2upCnt = 0;
			key2UpFlag = False;
		}
			DampAutoPos(0);
			MainSysRun();
		
	}
	if (htim->Instance == htim5.Instance)
	{
		oled.Set_Wheel(TimeRun++%96);
		oled.Calc_Color();
//		HAL_GPIO_TogglePin(SYSLED_GPIO_Port, SYSLED_Pin);
		Flag_Refrash = True;
	}
	if (htim->Instance == htim9.Instance)
	{
	  if(Device_Cmd.commandgametype<=8)
		{
			DataDisType = Device_Cmd.commandgametype;
			pit[DAMPTYP].current = 30;
			TimeRun1 = 0;
		}
		else
		{
			if(TimeRun1++>40)
			{
				TimeRun1 = 0;
				{
					DataDisType++;
					pit[DAMPTYP].current = 30;
					if(DataDisType>=8)
						DataDisType=0;
				}
			}
		}
		if(offlinecount<3)
		{
			offlinecount++;
		}
		else if(systemstatus==online)
		{
			systemstatus=offline;
			if(Display_Mode!=MODE_DATE&&Display_Mode!=MODE_MUSIC)
				Display_Mode=MODE_DATE;
		}
		Time_Handle();
		sprintf(fpschar,"%d",fps);
		fps = 0;
		
//		HAL_RTC_GetTime(&hrtc,&timenow,RTC_FORMAT_BIN);
//		printf("Time:%02d:%02d:%02d\r\n",timenow.Hours,timenow.Minutes,timenow.Seconds); 
//		HAL_RTC_GetDate(&hrtc,&datenow,RTC_FORMAT_BIN);
//		printf("Date:20%02d-%02d-%02d\r\n",datenow.Year,datenow.Month,datenow.Date); 
//		printf("Week:%d\r\n",datenow.WeekDay); 
	}
//	if (htim->Instance == htim6.Instance)
//	{
//		if(ContinueCount++>20)
//		{
//			ContinueCount = 0;
//			HAL_TIM_Base_Stop_IT(&htim6);
//		}
//	}
//	if (htim->Instance == htim7.Instance)
//	{
//		oled.Set_Wheel(TimeRun%96);
////		if(TimeRun++>20)
////		{
////			TimeRun = 0;
////		}
//		
//		if(SleepCount++>SLEEPTIME*2)
//		{
//				Flag_Sleep = True;
//				SleepCount = 0;
//		}
//	}
}

void Delayms(u16 ms)
{
  u16 i,j;
  u8 k;
  for(i=0;i<ms;i++)
    for(j=0;j<0x0500;j++) k++;
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	Delayms(20);
	switch(GPIO_Pin)
	{
		case KEY_L_Pin:
		if(HAL_GPIO_ReadPin(KEY_L_GPIO_Port,KEY_L_Pin)==GPIO_PIN_RESET)  //LED1翻转
		{
				if(sleepflag)
					module_pwr_exit_sleep_mode();
				else
					key1_fall_flag = 1;//开启按键标志
				drache_printf("KeyL Pressed!\r\n"); 
		}
		break;
		case KEY_R_Pin:
		if(HAL_GPIO_ReadPin(KEY_R_GPIO_Port,KEY_R_Pin)==GPIO_PIN_RESET)  //LED1翻转
		{
				if(sleepflag)
					module_pwr_exit_sleep_mode();
				else
					key2_fall_flag = 1;//开启按键标志
				drache_printf("Key2 Pressed!\r\n");  
		}
		break;
		case DS_INT_Pin:
		if(HAL_GPIO_ReadPin(DS_INT_GPIO_Port,DS_INT_Pin)==GPIO_PIN_RESET)  //LED1翻转
		{
			Time_Handle();
//			drache_printf("Time:%s\r\n",ds3231.Time); 
		}
		break;
	}
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	int i;
//	HAL_ADC_Stop_DMA(&hadc1);	//停止ADC的DMA传输
	FFT_Stop();
	FFT_Pro();	
	if(systemstatus==offline)
	{
		for(i=0;i<200;i++)
		{
			if(lBufMagArray[i]/prt>20)
				Device_Msg.fft[i] = lBufMagArray[i]/prt;
			else
				Device_Msg.fft[i] = lBufMagArray[i]/prt/4;
		}
		Device_Msg.leftvol=Device_Msg.fft[1];
	}
  FFT_Start();
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
