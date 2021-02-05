#include "OLED_UI.h"
#include "OLED_FFT.h"
#include "main.h"
#include "stdlib.h"
#include "ds3231.h"
#include "usart.h"
#include "math.h"
#include "image.h"

#ifdef __cplusplus
extern "C"  {
#endif


OLED_UI::OLED_UI(void) {
}
//uint16_t randcolor[100];
//u8 pointercolor=0;
void OLED_UI::OLED_LFPixel(int x,int y,int w,int h,int psize,int pinterval,u8 Num,const unsigned char *ch,uint16_t color)
{
	uint16_t i,j,k;
	uint16_t bnum = ((h+7)/8)*w;
	for(k=0;k<(h+7)/8;k++)//8*k层
		for(j=0;j<w;j++)//w
			for(i=0;i<8;i++)
				if((ch[Num*bnum+w*k+j]>>i)&1)
				{
					Fill_Rect(x+j*(psize+pinterval),y+i*(psize+pinterval)+k*8*(psize+pinterval),psize,psize,color);
				}
}
//	randcolor[(Num*bnum+w*k+j)*8+i]
void OLED_UI::OLED_SLFAny(int x,int y,int psize,int pinterval,char *ch,int w,uint16_t color)
{
	u8 c=0,j=0;
	while(ch[j]!='\0')
	{
		c=ch[j]-' ';
		
		switch(w)
		{
			case 8:OLED_LFPixel(x,y,8,16,psize,pinterval,c,OCRB_F8x16,color);break;
			case 9:OLED_LFPixel(x,y,9,16,psize,pinterval,c,Self_F9x16,color);break;
			case 10:OLED_LFPixel(x,y,10,16,psize,pinterval,c,OCR_F10x16,color);break;
			case 12:OLED_LFPixel(x,y,12,16,psize,pinterval,c,OCR_F12x16,color);break;
//			case 12:OLED_LFPixel(x,y,12,16,psize,pinterval,c,OCRB_F12x16,color);break;
			case 16:OLED_LFPixel(x,y,16,24,psize,pinterval,c,OCR_F16x24,color);break;
//			case 16:OLED_LFPixel(x,y,16,psize,pinterval,24,c,OCRB_F16x24,color);break;
			default:OLED_LFPixel(x,y,8,16,psize,pinterval,c,OCRB_F8x16,color);break;
		}
		x+=w*(psize+pinterval); 
		j++;
	}
}
	
void OLED_UI::TUI_In(){
	int i;
	for(i=40;i<50;i++)
		SetCurrent(i,0);
	SetTarget(40,128);
	SetTarget(41,-128);
	SetTarget(42,rand()%10-10);
}
void OLED_UI::TUI_Out(){
	int i;
	for(i=40;i<50;i++)
		SetTarget(i,0);
}
void OLED_UI::TUIDataPrss(){
	static int oldmin;
	if(oldmin!=BCD2HEX(DS3231_US_Buf[2]))
	{
		oldmin=BCD2HEX(DS3231_US_Buf[2]);
		SetTarget(40,rand()%5-5+128);
		SetTarget(41,rand()%10-128);
		SetTarget(42,rand()%10-10);
//		for(i=0;i<sizeof(randcolor);i++)
//			randcolor[i]=RandomColor();
	}
}
OLED_STATUS OLED_UI::TUIMainShow(){
	OLED_SLFAny(pit[40].current-128,-10+pit[42].current,4,2,ds3231.Hour,8,~color_now);
	OLED_SLFAny(128+pit[41].current+74,55+pit[42].current,3,2,ds3231.Min,8,color_now);
	return OLED_IDLE;}

void OLED_UI::T1UI_In(){
	int i;
	for(i=10;i<30;i++)
		SetCurrent(i,0);
	pit[RDHourColor].target = oled.WheelPw(rand()%96);
	pit[RDMinColor].target = oled.WheelPw(rand()%96);
	pit[RDSecColor].target = oled.WheelP(rand()%96);
	SetCurrent(RDRadius, Radius*2);
	SetCurrent(RDRadiusC, RadiusC*2);
	SetCurrent(RDRadiusB, RadiusB*2);
	SetTarget(RDRadius, Radius);
	SetTarget(RDRadiusC, RadiusC);
	SetTarget(RDRadiusB, RadiusB);
	SetTarget(RDHourRadius, HourRadius);
	SetTarget(RDMinRadius, MinRadius);
	SetTarget(RDSecRadius, SecRadius);
	SetTarget(RDRuleDelaut, 0);
	if(BCD2HEX(DS3231_US_Buf[2])%12<6)
	{
		SetTarget(PNTTIME,22+16);
		SetCurrent(PNTTIME, -60);
	}
	else
	{
		SetTarget(PNTTIME,22+76);
		if(pit[PNTTIME].target!=22+76)
			SetCurrent(PNTTIME,160+50);
	}
}
void OLED_UI::T1UI_Out(){
	int i;
	for(i=10;i<30;i++)
		SetTarget(i,0);
	SetTarget(RDRadius, Radius*2);
	SetTarget(RDRadiusC, RadiusC*2);
	SetTarget(RDRadiusB, RadiusB*2);
	SetTarget(RDRuleDelaut, 180);
//	if(Display_Mode != MODE_DATE_1)
	{
		SetTarget(RDHourRadius, 0);
		SetTarget(RDMinRadius, 0);
		SetTarget(RDSecRadius, 0);
		SetTarget(PNTHOUR,-90);
		SetTarget(PNTMIN,-90);
		SetTarget(PNTSEC,-90);
		if(pit[PNTTIME].current<60)
		{
			SetTarget(PNTTIME,-50);
		}
		else
		{
			SetTarget(PNTTIME,160+50);
		}
	}
}
void OLED_UI::T1UIDataPrss(){

	pit[PNTHOUR].target = (BCD2HEX(DS3231_US_Buf[2])%12-3)*30+BCD2HEX(DS3231_US_Buf[1])/2;
	pit[PNTMIN].target = (BCD2HEX(DS3231_US_Buf[1])-15)*6+BCD2HEX(DS3231_US_Buf[0])/2/5;
	pit[PNTSEC].target = (BCD2HEX(DS3231_US_Buf[0])-15)*6;
	if(pit[PNTHOUR].target == -90)
		pit[PNTHOUR].target = 270;
	else if(pit[PNTHOUR].current == 270)
		pit[PNTHOUR].current = -90;
	if(pit[PNTMIN].target == -90)
		pit[PNTMIN].target = 270;
	else if(pit[PNTMIN].current == 270)
		pit[PNTMIN].current = -90;
	if(pit[PNTSEC].target == -90)
		pit[PNTSEC].target = 270;
	else if(pit[PNTSEC].current == 270)
	{
		pit[RDHourColor].target = oled.WheelPw(rand()%96);
		pit[RDMinColor].target = oled.WheelPw(rand()%96);
		pit[RDSecColor].target = oled.WheelP(rand()%96);
		
		pit[PNTSEC].current = -90;
	}

	if(BCD2HEX(DS3231_US_Buf[2])%12<6)
	{
		SetTarget(PNTTIME,22+16);
	}
	else
	{
		SetTarget(PNTTIME,22+76);
	}
}
OLED_STATUS OLED_UI::T1UIMainShow(){
	int i;
	for(i=0;i<60;i++)
	{
		if(i%5==0)
			oled.Draw_Line((OCX+(pit[RDRadius].current*cos((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCY+(pit[RDRadius].current*sin((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCX+(pit[RDRadiusB].current*cos((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCY+(pit[RDRadiusB].current*sin((i*6+pit[RDRuleDelaut].current)*PI/180))),color_half);
		else
			oled.Draw_Line((OCX+(pit[RDRadius].current*cos((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCY+(pit[RDRadius].current*sin((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCX+(pit[RDRadiusC].current*cos((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCY+(pit[RDRadiusC].current*sin((i*6+pit[RDRuleDelaut].current)*PI/180))),color_min);
	}
	oled.Draw_Circle(OCX,OCY,pit[RDRadius].current+2,color_half);
	oled.Draw_Circle(OCX,OCY,pit[RDRadius].current);

	oled.OLED_SNF6x8(pit[PNTTIME].current,60,ds3231.Timens);
	
	oled.Draw_Line((OCX+(pit[RDHourRadius].current*cos(pit[PNTHOUR].current*PI/180))),(OCY+(pit[RDHourRadius].current*sin(pit[PNTHOUR].current*PI/180))),OCX,OCY,2,pit[RDHourColor].target);
	oled.Draw_Line((OCX+(pit[RDMinRadius].current*cos(pit[PNTMIN].current*PI/180))),(OCY+(pit[RDMinRadius].current*sin(pit[PNTMIN].current*PI/180))),OCX,OCY,pit[RDMinColor].target);
	oled.Draw_Line((OCX+(pit[RDSecRadius].current*cos(pit[PNTSEC].current*PI/180))),(OCY+(pit[RDSecRadius].current*sin(pit[PNTSEC].current*PI/180))),OCX,OCY,pit[RDSecColor].target);
//	oled.Draw_Line((OCX+(pit[RDSecRadius].current/5*cos((pit[PNTSEC].current+180)*PI/180))),(OCY+(pit[RDSecRadius].current/5*sin((pit[PNTSEC].current+180)*PI/180))),OCX,OCY,pit[RDSecColor].target);

	if(pit[RDHourRadius].current>5)
	{
		oled.Fill_Circle(OCX,OCY,3,0);
		oled.Draw_Circle(OCX,OCY,1);
		oled.Draw_Circle(OCX,OCY,3,color_half);
	}
	return OLED_IDLE;}

	
void OLED_UI::T2UI_In(){
	int i;
	for(i=10;i<30;i++)
		SetCurrent(i,0);
	pit[RDHourColor].target = oled.WheelPw(rand()%96);
	pit[RDMinColor].target = oled.WheelPw(rand()%96);
	pit[RDSecColor].target = oled.WheelP(rand()%96);
	SetCurrent(RDRadiusA, Radius);
	SetTarget(RDRadiusA, Radius/2);
	SetTarget(RDHourRadius, HourRadius);
	SetTarget(RDMinRadius, MinRadius);
	SetTarget(RDSecRadius, SecRadius);
	if(BCD2HEX(DS3231_US_Buf[2])%12<6)
	{
		SetTarget(PNTTIME,22);
		SetCurrent(PNTTIME, -60);
	}
	else
	{
		SetTarget(PNTTIME,22+76);
		if(pit[PNTTIME].target!=22+76)
			SetCurrent(PNTTIME,160+50);
	}
}
void OLED_UI::T2UI_Out(){
	int i;
	for(i=10;i<30;i++)
		SetTarget(i,0);
	SetTarget(RDRadiusA, Radius);
//	if(Display_Mode != MODE_DATE)
	{
		SetTarget(RDHourRadius, 0);
		SetTarget(RDMinRadius, 0);
		SetTarget(RDSecRadius, 0);
		SetTarget(PNTHOUR,-90);
		SetTarget(PNTMIN,-90);
		SetTarget(PNTSEC,-90);
		if(pit[PNTTIME].current<60)
		{
			SetTarget(PNTTIME,-50);
		}
		else
		{
			SetTarget(PNTTIME,160+50);
		}
	}
}
void OLED_UI::T2UIDataPrss(){

	pit[PNTHOUR].target = (BCD2HEX(DS3231_US_Buf[2])%12-3)*30+BCD2HEX(DS3231_US_Buf[1])/2;
	pit[PNTMIN].target = (BCD2HEX(DS3231_US_Buf[1])-15)*6+BCD2HEX(DS3231_US_Buf[0])/2/5;
	pit[PNTSEC].target = (BCD2HEX(DS3231_US_Buf[0])-15)*6;
	if(pit[PNTHOUR].target == -90)
		pit[PNTHOUR].target = 270;
	else if(pit[PNTHOUR].current == 270)
		pit[PNTHOUR].current = -90;
	if(pit[PNTMIN].target == -90)
		pit[PNTMIN].target = 270;
	else if(pit[PNTMIN].current == 270)
		pit[PNTMIN].current = -90;
	if(pit[PNTSEC].target == -90)
		pit[PNTSEC].target = 270;
	else if(pit[PNTSEC].current == 270)
	{
		pit[RDHourColor].target = oled.WheelPw(rand()%96);
		pit[RDMinColor].target = oled.WheelPw(rand()%96);
		pit[RDSecColor].target = oled.WheelP(rand()%96);
		
		pit[PNTSEC].current = -90;
	}

	if(BCD2HEX(DS3231_US_Buf[2])%12<6)
	{
		SetTarget(PNTTIME,22);
	}
	else
	{
		SetTarget(PNTTIME,22+76);
	}
}
OLED_STATUS OLED_UI::T2UIMainShow(){
	oled.Draw_Circle(OCX+RLV+14,OCY+RLV,pit[RDRadiusA].current,1,color_now);
	oled.Draw_Circle(OCX+RLV+14,OCY+RLV,pit[RDRadiusA].current+2,1,color_half); //右下
	
	oled.Draw_Circle(OCX-RLV-14,OCY+RLV,pit[RDRadiusA].current,2,color_now);
	oled.Draw_Circle(OCX-RLV-14,OCY+RLV,(pit[RDRadiusA].current+2),2,color_half);	//左下
	
	oled.Draw_Circle(OCX-RLV-14,OCY-RLV,pit[RDRadiusA].current,4,color_now);
	oled.Draw_Circle(OCX-RLV-14,OCY-RLV,(pit[RDRadiusA].current+2),4,color_half); //左上
	
	oled.Draw_Circle(OCX+RLV+14,OCY-RLV,pit[RDRadiusA].current,3,color_now);
	oled.Draw_Circle(OCX+RLV+14,OCY-RLV,(pit[RDRadiusA].current+2),3,color_half); //右上
	
	oled.Draw_Line(OCX+RLV+14+pit[RDRadiusA].current,OCY+RLV,OCX+RLV+pit[RDRadiusA].current+14,OCY-RLV,color_now);//右侧引导线
	oled.Draw_Line(OCX+RLV+14+(pit[RDRadiusA].current+2),OCY+RLV,OCX+RLV+(pit[RDRadiusA].current+2)+14,OCY-RLV,color_half);//右侧引导线
	
	oled.Draw_Line(OCX-RLV-14-pit[RDRadiusA].current,OCY+RLV,OCX-RLV-pit[RDRadiusA].current-14,OCY-RLV,color_now);//左侧引导线
	oled.Draw_Line(OCX-RLV-14-(pit[RDRadiusA].current+2),OCY+RLV,OCX-RLV-(pit[RDRadiusA].current+2)-14,OCY-RLV,color_half);//左侧引导线

	oled.OLED_SNF6x8(pit[PNTTIME].current,60,ds3231.Timens);
	oled.Draw_Line(pit[PNTTIME].current-1,60+9,pit[PNTTIME].current+30,60+9,color_half);
	
	oled.Draw_Line((OCX+(pit[RDHourRadius].current*cos(pit[PNTHOUR].current*PI/180))),(OCY+(pit[RDHourRadius].current*sin(pit[PNTHOUR].current*PI/180))),OCX,OCY,2,pit[RDHourColor].target);
	oled.Draw_Line((OCX+(pit[RDMinRadius].current*cos(pit[PNTMIN].current*PI/180))),(OCY+(pit[RDMinRadius].current*sin(pit[PNTMIN].current*PI/180))),OCX,OCY,pit[RDMinColor].target);
	oled.Draw_Line((OCX+(pit[RDSecRadius].current*cos(pit[PNTSEC].current*PI/180))),(OCY+(pit[RDSecRadius].current*sin(pit[PNTSEC].current*PI/180))),OCX,OCY,pit[RDSecColor].target);
//	oled.Draw_Line((OCX+(pit[RDSecRadius].current/5*cos((pit[PNTSEC].current+180)*PI/180))),(OCY+(pit[RDSecRadius].current/5*sin((pit[PNTSEC].current+180)*PI/180))),OCX,OCY,pit[RDSecColor].target);

	if(pit[RDHourRadius].current>5)
	{
		oled.Fill_Circle(OCX,OCY,3,0);
		oled.Draw_Circle(OCX,OCY,1);
		oled.Draw_Circle(OCX,OCY,3,color_half);
	}
	return OLED_IDLE;}

	
	
extern OLED_FFT fft;
void OLED_UI::MUI_In(){
	int i;
	ClearFFT();
	for(i=0;i<sizeof(fall_pot);i++)
	{
		fall_pot[i] = 160;
		flow_pot[i] = 160;
	}
}
void OLED_UI::MUI_Out(){
	ClearFFT();
}

void OLED_UI::MUIDataPrss(){

}

OLED_STATUS OLED_UI::MUIMainShow(){
  static uint8_t OTheme = 0;
	int i;
	if(OTheme!=Device_Cmd.commandstyle)
	{
		OTheme=Device_Cmd.commandstyle;
		
		for(i=0;i<sizeof(fall_pot);i++)
		{
			ClearFFT();
			fall_pot[i] = 160;
			flow_pot[i] = 160;
		}
	}
	switch(Device_Cmd.commandstyle)
	{
			case 1:fft.Display_Style1();break;
			case 2:fft.Display_Style2();break;
			case 3:fft.Display_Style3();break;
			case 4:fft.Display_Style4();break;
			case 5:fft.Display_Style5();break;
			case 6:fft.Display_Style6();break;
			default:fft.Display_Style1();break;
	}
	return OLED_IDLE;
}
void OLED_UI::HUI_In(){
	int i;
	for(i=50;i<60;i++)
		SetCurrent(i,0);
	SetTarget(50,160+16);
}
void OLED_UI::HUI_Out(){
	int i;
	for(i=50;i<60;i++)
		SetTarget(i,0);
}
void OLED_UI::HUIDataPrss(){
}
OLED_STATUS OLED_UI::HUIMainShow(){
	
	oled.Display_bmp(0-160+pit[50].current,0,128,128,Logo_amd);
	
	return OLED_IDLE;
}


void OLED_UI::SUI_In(){
	int i;
	for(i=60;i<70;i++)
		SetCurrent(i,0);
	SetTarget(SSLF,8) ;
	SetTarget(SSRT,66) ;
	SetTarget(SSMLF,8);
	SetTarget(SSMRT,98);
	SetTarget(SSLLF,40);
	SetTarget(SSLRT,88);
	SetTarget(SSUP,0);
	SetTarget(SSDN,112-7);
	SetTarget(SSRD,5);
	
	SetCurrent(SSLF,8-80) ;
	SetCurrent(SSRT,66+80) ;
	SetCurrent(SSMLF,8-120);
	SetCurrent(SSMRT,98+120);
	SetCurrent(SSLLF,40-20);
	SetCurrent(SSLRT,88+20);
	SetCurrent(SSUP,0-20);
	SetCurrent(SSDN,112-7+40);
	SetCurrent(SSRD,5-5);
}
void OLED_UI::SUI_Out(){
	int i;
	for(i=60;i<70;i++)
		SetTarget(i,0);
	SetTarget(SSLF,8-80) ;
	SetTarget(SSRT,66+80) ;
	SetTarget(SSMLF,8-88);
	SetTarget(SSMRT,98+88);
	SetTarget(SSLLF,40-20);
	SetTarget(SSLRT,88+20);
	SetTarget(SSUP,0-20);
	SetTarget(SSDN,112-7+40);
	SetTarget(SSRD,5-5);
}
void OLED_UI::SUIDataPrss(){
	
}

#define DT 16
extern OLED_Animation motion;
OLED_STATUS OLED_UI::SUIMainShow(){
	static int i,roun=0;
	static char tempstr[100];
	roun++;
	oled.Display_bbmp(DT+21,46,86,43,GeForce_B_Back,color_min);
	oled.Display_bbmp(DT+6+5,105,23,13,shadowleft,color_min);
	oled.Display_bbmp(DT+99-5,105,23,13,shadowright,color_min);

	motion.OLED_CustormMotion(Device_Cmd.commandmotion);
	oled.Display_hbmp(DT+pit[SSLF].current,22,54,9,userleft,color_now);
	oled.Display_hbmp(DT+pit[SSRT].current,22,54,9,userright,color_now);
	oled.Display_hbmp(DT+10,pit[SSDN].current,108,21,userbottom,color_now);
	
	
	oled.Display_hbmp(DT+35,pit[SSDN].current+3,58,3,usermgndbar,0xFFFF);
	oled.Display_hbmp(DT+pit[SSMLF].current,47,21,37,usermleft,color_half);
	oled.Display_hbmp(DT+pit[SSMRT].current,47,21,37,usermright,color_half);
	for(i=0;i<9;i++)
			oled.Draw_Pixel(DT+9+28+4+8+i*5,pit[SSUP].current+8,color_min);
	
	oled.Draw_Line(DT+0,0,DT+pit[SSRD].current,pit[SSRD].current,color_now);
	oled.Draw_Line(DT+127,0,DT+127-pit[SSRD].current,pit[SSRD].current,color_now);
	oled.Draw_Line(DT+pit[SSRD].current,127-pit[SSRD].current,DT+0,127,color_now);
	oled.Draw_Line(DT+127-pit[SSRD].current,127-pit[SSRD].current,DT+127,127,color_now);
	
	oled.Display_hbmp(DT+9-5+pit[SSRD].current,pit[SSRD].current,28,10,GeForce_LOAD,color_now);
	oled.Display_hbmp(DT+101+5-pit[SSRD].current,pit[SSRD].current,21,10,GeForce_SYS,color_now);
	oled.Display_hbmp(DT+27+1,33,74,10,GeForce_TEMP,color_now);
	oled.Display_hbmp(DT+48,88,33,13,GeForce_CENT,color_now);
	oled.Draw_Line(DT+20,96-2,DT+pit[SSLLF].current,96-2,0xFFFF);
	oled.Draw_Line(DT+pit[SSLRT].current,96-2,DT+108,96-2,0xFFFF);
	oled.Draw_Line(DT+27,33+10,DT+27+73,33+10,color_min);
	sprintf(tempstr,"%02d",Device_Msg.cputemp/10);
	oled.OLED_SHFAny(DT+38,48,tempstr,25,0xFFFF);
	sprintf(tempstr,"%03d",Device_Msg.cpuload/10);
	oled.OLED_SHFAny(DT+29-8+pit[SSLF].current,15,tempstr,10,0xFFFF);
	sprintf(tempstr,"%03d",Device_Msg.gputemp/10);
	oled.OLED_SHFAny(DT+69+1+8-pit[SSLF].current,15,tempstr,10,0xFFFF);
	return OLED_IDLE;
}





#ifdef __cplusplus
}
#endif


