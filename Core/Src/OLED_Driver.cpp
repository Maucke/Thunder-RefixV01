#include "OLED_Driver.h"
#include "ASCII_Font.h"
#include "main.h"
#include "gpio.h"
#include "stdlib.h"


uint8_t color_byte[2],color_Damp_byte[2];

uint16_t wheel;
uint8_t OLED_GRAM[2*SCR_WIDTH*SCR_HEIGHT];
uint16_t color_now,color_half,color_min;

#ifdef __cplusplus
extern "C"  {
#endif


OLED_Driver::OLED_Driver(void) {
}
  
u16 OLED_Driver::ConvertColor(u8 red,u8 green,u8 blue)
{
	return red<<11|green<<6|blue;
}
u16 OLED_Driver::WheelP(u16 WheelPos)
{
	
	uint8_t red,green,blue;
	if(WheelPos < 32) 
	{
		red = 0;
		green = WheelPos;
		blue = (31 - WheelPos);
	}
	else if(WheelPos < (32*2)) 
	{          
		red = WheelPos-32;
		green = 32*2-1 - WheelPos;
		blue = 0;
	}
	else
	{
		red = (32*3)-1 - WheelPos;
		green = 0;
		blue = WheelPos-(32*2);
	}
	return (red<<11|green<<5|blue);
}	
u16 OLED_Driver::WheelPw(u16 WheelPos)
{
	
	uint8_t red,green,blue;
	if(WheelPos < 32) 
	{
		red = 31;
		green = WheelPos;
		blue = (31 - WheelPos);
	}
	else if(WheelPos < (32*2)) 
	{          
		red = WheelPos-32;
		green = 32*2-1 - WheelPos;
		blue = 31;
	}
	else
	{
		red = (32*3)-1 - WheelPos;
		green = 31;
		blue = WheelPos-(32*2);
	}
	return (red<<11|green<<5|blue);
}	


void OLED_Driver::Set_Wheel(u16 WheelPos)	{
	
	uint8_t red,green,blue;
	wheel = WheelPos;
	if(WheelPos < 32) 
	{
		red = 0;
		green = WheelPos;
		blue = (31 - WheelPos);
	}
	else if(WheelPos < (32*2)) 
	{          
		red = WheelPos-32;
		green = 32*2-1 - WheelPos;
		blue = 0;
	}
	else
	{
		red = (32*3)-1 - WheelPos;
		green = 0;
		blue = WheelPos-(32*2);
	}
	color_Damp_byte[0] = red<<3|(green>>2);
	color_Damp_byte[1] = green<<6|blue;
}
void OLED_Driver::Calc_Color(void){
				
	uint8_t red,green,blue;
	uint8_t red1,green1,blue1;
	red = color_byte[0]>>3;
	green = ((color_byte[0]&0x7)<<3)|color_byte[1]>>5;
	blue = color_byte[1]&0x1f;
	
	red1 = color_Damp_byte[0]>>3;
	green1 = ((color_Damp_byte[0]&0x7)<<3)|color_Damp_byte[1]>>5;
	blue1 = color_Damp_byte[1]&0x1f;
	
	if(red>red1)
	{
		red--;
	}
	else if(red<red1)
	{
		red++;
	}
	if(green>green1+1)
	{
		green-=2;
	}
	else if(green+1<green1)
	{
		green+=2;
	}
	if(blue>blue1)
	{
		blue--;
	}
	else if(blue<blue1)
	{
		blue++;
	}
	color_now = red<<11|green<<5|blue;
	color_byte[0] = color_now>>8;
	color_byte[1] = color_now&0xff;
	red >>=1;
	green >>=1;
	blue >>=1;
	color_half = red<<11|green<<5|blue;
	red >>=1;
	green >>=1;
	blue >>=1;
	color_min = red<<11|green<<5|blue;
	
}


uint16_t OLED_Driver::RandomColor(void){
	
	uint8_t red,green,blue;
	red = rand();
	__ASM("NOP");
	green = rand();
	__ASM("NOP");
	blue = rand();
	return (red<<11|green<<5|blue);
}

void OLED_Driver::Set_DampColor(uint16_t color)  {
  
  color_Damp_byte[0] = (uint8_t)(color >> 8);
  color_Damp_byte[1] = (uint8_t)(color & 0x00ff);
}




void OLED_Driver::Set_Color(uint16_t color)  {
  
  color_byte[0] = (uint8_t)(color >> 8);
  color_byte[1] = (uint8_t)(color & 0x00ff);
}


//void OLED_Driver::Set_FillColor(uint16_t color)  {
//  
//  color_fill_byte[0] = (uint8_t)(color >> 8);
//  color_fill_byte[1] = (uint8_t)(color & 0x00ff);
//}
//  

void OLED_Driver::Write_Command(uint8_t cmd)  {
  
  OLED_CS(GPIO_PIN_RESET);
  
#if  INTERFACE_4WIRE_SPI
  
  OLED_DC(GPIO_PIN_RESET);
  
  while(HAL_SPI_Transmit(&hspi1,&cmd,0x01,0x10) != HAL_OK);
  
  OLED_DC(GPIO_PIN_SET);
  
#elif INTERFACE_3WIRE_SPI
  
  uint8_t i;
	uint16_t hwData = 0;
	
  hwData = (uint16_t)cmd & ~0x0100;

	for(i = 0; i < 9; i ++) {
		OLED_SCK(GPIO_PIN_RESET);
    if(hwData & 0x0100) {
      OLED_DIN(GPIO_PIN_SET);
		}
    else  {
      OLED_DIN(GPIO_PIN_RESET);
		}
    OLED_SCK(GPIO_PIN_SET);
		hwData <<= 1;
	}

  
#endif
  
  OLED_CS(GPIO_PIN_SET);
}


void OLED_Driver::Write_Data(uint8_t dat) {
  
  OLED_CS(GPIO_PIN_RESET);
  OLED_DC(GPIO_PIN_SET);
  
  while(HAL_SPI_Transmit(&hspi1,&dat,0x01,0x10) != HAL_OK);
  
  OLED_DC(GPIO_PIN_RESET);
  OLED_CS(GPIO_PIN_SET);
  
}

uint8_t dmasendflag = 1;

void OLED_Driver::Write_Data(uint8_t* dat_p, long length) {

  OLED_CS(GPIO_PIN_RESET);
  OLED_DC(GPIO_PIN_SET);
  while(HAL_SPI_Transmit(&hspi1,dat_p,length,0xffff) != HAL_OK);
	
  OLED_DC(GPIO_PIN_RESET);
  OLED_CS(GPIO_PIN_SET);
  
}
void OLED_Driver::SCR_reg(int idx, int value)
{
  Write_Command(idx);
  Write_Data(value);
}


void OLED_Driver::RAM_Address(void)  {
  
  // draw region
  SCR_reg(0x17,0);
  SCR_reg(0x18,0x9f);
  SCR_reg(0x19,0);
  SCR_reg(0x1a,0x7f);
  
  // start position
//  SCR_reg(0x20,0);
//  SCR_reg(0x21,0);
}


void OLED_Driver::Refrash_Screen(void)  {
    
  RAM_Address();
  Write_Command(0x22);
//  for(i=0;i<;i++)  {
      Write_Data(OLED_GRAM,2*SCR_WIDTH*SCR_HEIGHT);//RAM data clear
//  }
}
  

void OLED_Driver::Clear_Screen(void)  {
  
  int i,j;
  for(i=0;i<SCR_HEIGHT;i++)  {
    for(j=0;j<SCR_WIDTH;j++)  {
      OLED_GRAM[2*j+i*SCR_WIDTH*2] = 0;
      OLED_GRAM[2*j+1+i*SCR_WIDTH*2] = 0;
    }
  }
}
  
//void OLED_Driver::Set_Coordinate(uint16_t x, uint16_t y)  {

//  if ((x >= SCR_WIDTH) || (y >= SCR_HEIGHT)) 
//    return;
//  //Set x and y coordinate
//  Write_Command(SCR_CMD_SETCOLUMN);
//  Write_Data(x);
//  Write_Data(SCR_WIDTH-1);
//  Write_Command(SCR_CMD_SETROW);
//  Write_Data(y);
//  Write_Data(SCR_HEIGHT-1);
//  Write_Command(SCR_CMD_WRITERAM);
//}
  
//void OLED_Driver::Set_Address(uint8_t column, uint8_t row)  {
//  
//  Write_Command(SCR_CMD_SETCOLUMN);  
//  Write_Data(column);	//X start 
//  Write_Data(column);	//X end 
//  Write_Command(SCR_CMD_SETROW); 
//  Write_Data(row);	//Y start 
//  Write_Data(row+7);	//Y end 
//  Write_Command(SCR_CMD_WRITERAM); 
//}
//  
//  
//void  OLED_Driver::Invert(bool v) {
//  
//  if (v)
//    Write_Command(SCR_CMD_INVERTDISPLAY);
//  else
//    Write_Command(SCR_CMD_NORMALDISPLAY);
//}

void OLED_Driver::Draw_Pixel(long x, long y)
{
  // Bounds check.
  if ((x >= SCR_WIDTH) || (y >= SCR_HEIGHT)) return;
  if ((x < 0) || (y < 0)) return;

	OLED_GRAM[2*x+y*SCR_WIDTH*2] = color_byte[0];
	OLED_GRAM[2*x+1+y*SCR_WIDTH*2] = color_byte[1];
  
}

void OLED_Driver::Draw_Pixel(long x, long y,uint16_t color)
{
  // Bounds check.
  if ((x >= SCR_WIDTH) || (y >= SCR_HEIGHT)) return;
  if ((x < 0) || (y < 0)) return;

	OLED_GRAM[2*x+y*SCR_WIDTH*2] = (uint8_t)(color >> 8);
	OLED_GRAM[2*x+1+y*SCR_WIDTH*2] = (uint8_t)(color & 0x00ff);
  
}
  
  
void OLED_Driver::Device_Init(void) {

  OLED_CS(GPIO_PIN_RESET);

  OLED_RST(GPIO_PIN_RESET);
  HAL_Delay(40);
  OLED_RST(GPIO_PIN_SET);
  HAL_Delay(20);
    
  // display off, analog reset
  SCR_reg(0x04, 0x01);
  HAL_Delay(1);
  // normal mode
  SCR_reg(0x04, 0x00);
  HAL_Delay(1);
  // display off
  SCR_reg(0x06, 0x00);
  // turn on internal oscillator using external resistor
  SCR_reg(0x02, 0x01);
  // 90 hz frame rate, divider 0
  SCR_reg(0x03, 0x30);
  // duty cycle 127
  SCR_reg(0x28, 0x7f);
  // start on line 0
  SCR_reg(0x29, 0x00);
  // rgb_if
  SCR_reg(0x14, 0x31);
  // memory write mode
  SCR_reg(0x16, 0x66);

  // driving current r g b (uA)
  SCR_reg(0x10, 0x45);
  SCR_reg(0x11, 0x34);
  SCR_reg(0x12, 0x33);

  // precharge time r g b
  SCR_reg(0x08, 0x04);
  SCR_reg(0x09, 0x05);
  SCR_reg(0x0a, 0x05);

  // precharge current r g b (uA)
  SCR_reg(0x0b, 0x9d);
  SCR_reg(0x0c, 0x8c);
  SCR_reg(0x0d, 0x57);

  SCR_reg(0x80, 0x00);

  // mode set
  SCR_reg(0x13, 0x00);
  
  Clear_Screen();
	Refrash_Screen();
	
  HAL_Delay(100);
  
  SCR_reg(0x06, 0x01);
}

  
// Draw a horizontal line ignoring any screen rotation.
void OLED_Driver::Draw_FastHLine(int16_t x, int16_t y, int16_t length) {
  // Bounds check
		int16_t x0=x;
    do
    {
        Draw_Pixel(x, y);   // �����ʾ�������ֱ��
        x++;
    }
    while(x0+length>=x);
}
// Draw a horizontal line ignoring any screen rotation.
void OLED_Driver::Draw_FastHLine(int16_t x, int16_t y, int16_t length,uint16_t color) {
  // Bounds check
		int16_t x0=x;
    do
    {
        Draw_Pixel(x, y,color);   // �����ʾ�������ֱ��
        x++;
    }
    while(x0+length>=x);
}
  
  // Draw a vertical line ignoring any screen rotation.
void OLED_Driver::Draw_FastVLine(int16_t x, int16_t y, int16_t length)  {
  // Bounds check
		int16_t y0=y;
    do
    {
        Draw_Pixel(x, y);   // �����ʾ�������ֱ��
        y++;
    }
    while(y0+length>=y);
}
  // Draw a vertical line ignoring any screen rotation.
void OLED_Driver::Draw_FastVLine(int16_t x, int16_t y, int16_t length,uint16_t color)  {
  // Bounds check
		int16_t y0=y;
    do
    {
        Draw_Pixel(x, y,color);   // �����ʾ�������ֱ��
        y++;
    }
    while(y0+length>=y);
}
void OLED_Driver::Display_bbmp(int x,int y,int w,int h,const uint8_t *ch) {
  
	u8 i,j,k;
	for(k=0;k<(h/8+1);k++)
		for(i=0;i<8;i++)
		{
			if(k*8+i>=h)
				return;
			for(j=0;j<w;j++)
					if(ch[k*w+j]&(1<<i))
						Draw_Pixel(x+j,y+k*8+i);
		}
}

void OLED_Driver::Display_bbmp(int x,int y,int w,int h,const uint8_t *ch,uint16_t color) {
  
	u8 i,j,k;
	for(k=0;k<(h/8+1);k++)
		for(i=0;i<8;i++)
		{
			if(k*8+i>=h)
				return;
			for(j=0;j<w;j++)
					if(ch[k*w+j]&(1<<i))
						Draw_Pixel(x+j,y+k*8+i,color);
		}
}
  
void OLED_Driver::Display_bmp(int x,int y,int w,int h,const uint8_t *ch) {
  int Temp;
  int i,j;
  for(i=y;i<y+h;i++)  {
    for(j=x;j<x+w;j++)  {
			Temp = (i-y)*w*2+2*(j-x);
			if((ch[Temp+1]<<8)|ch[Temp])
				Draw_Pixel(j,i,(ch[Temp+1]<<8)|ch[Temp]); 
    }
  }
} 

void OLED_Driver::Display_hbmp(int x,int y,int w,int h,const u8 *ch,uint16_t color)
{
	u16 i,j;
	u16 red,green,blue;
	u16 red1,green1,blue1;
	u16 Factor;
//	color = 0xFFFF;
	red = color>>11;
	green = (color&0x7E0)>>5;
	blue = color&0x1F;
	for(j=0;j<h;j++)
		for(i=0;i<(w+1)/2;i++)
		{
			Factor = (ch[j*((w+1)/2)+i]&0xF0)>>4;
			red1 = red*(Factor+16)/32;
			green1 = green*(Factor+16)/32;
			blue1=blue*(Factor+16)/32;
			
			if(Factor)
				Draw_Pixel(x+i*2+0,y+j,red1<<11|(green1<<5)|(blue1));
			
			Factor = (ch[j*((w+1)/2)+i]&0xF);
			red1 = red*(Factor+16)/32;
			green1 = green*(Factor+16)/32;
			blue1=blue*(Factor+16)/32;
			
			if(Factor)
				Draw_Pixel(x+i*2+1,y+j,red1<<11|(green1<<5)|(blue1));
		}
}	
  
void OLED_Driver::Display_hbmp(int x,int y,int w,int h,const u8 *ch,uint16_t color,uint8_t bk)
{
	u16 i,j;
	u16 red,green,blue;
	u16 red1,green1,blue1;
	u16 Factor;
//	color = 0xFFFF;
	red = color>>11;
	green = (color&0x7E0)>>5;
	blue = color&0x1F;
	for(j=0;j<h;j++)
		for(i=0;i<(w+1)/2;i++)
		{
			Factor = (ch[j*((w+1)/2)+i]&0xF0)>>4;
			red1 = red*(Factor+16)/32;
			green1 = green*(Factor+16)/32;
			blue1=blue*(Factor+16)/32;
			
			if(Factor)
				Draw_Pixel(x+i*2+0,y+j,red1<<11|(green1<<5)|(blue1));
			else
				Draw_Pixel(x+i*2+0,y+j,bk);
			
			Factor = (ch[j*((w+1)/2)+i]&0xF);
			red1 = red*(Factor+16)/32;
			green1 = green*(Factor+16)/32;
			blue1=blue*(Factor+16)/32;
			
			if(Factor)
				Draw_Pixel(x+i*2+1,y+j,red1<<11|(green1<<5)|(blue1));
			else
				Draw_Pixel(x+i*2+1,y+j,bk);
		}
}	

void OLED_Driver::OLED_HFAny(int x,int y,int w,int h,u8 Num,const unsigned char *ch,uint16_t color)
{
	u16 i,j;
	u16 red,green,blue;
	u16 red1,green1,blue1;
	u16 Factor;
//	color = 0xFFFF;
	red = color>>11;
	green = (color&0x7E0)>>5;
	blue = color&0x1F;
	for(j=0;j<h;j++)
		for(i=0;i<(w+1)/2;i++)
		{
			Factor = (ch[Num*h*((w+1)/2)+j*((w+1)/2)+i]&0xF0)>>4;
			red1 = red*(Factor+16)/32;
			green1 = green*(Factor+16)/32;
			blue1=blue*(Factor+16)/32;
			
			if(Factor)
				Draw_Pixel(x+i*2+0,y+j,red1<<11|(green1<<5)|(blue1));
			
			Factor = (ch[Num*h*((w+1)/2)+j*((w+1)/2)+i]&0xF);
			red1 = red*(Factor+16)/32;
			green1 = green*(Factor+16)/32;
			blue1=blue*(Factor+16)/32;
			
			if(Factor)
				Draw_Pixel(x+i*2+1,y+j,red1<<11|(green1<<5)|(blue1));
		}
}	

extern const unsigned char Defu_XF4x5[];
extern const unsigned char GeForce_10x13[];
extern const unsigned char GeForce_13x16[];
extern const unsigned char GeForce_19x23[];
extern const unsigned char GeForce_25x37[];
void OLED_Driver::OLED_SHFAny(int x,int y,char *ch,int w,uint16_t color)
{
	u8 c=0,j=0;
	while(ch[j]!='\0')
	{
		if(ch[j]=='-')
			c=10;
		else
			c=ch[j]-'0';
		
		switch(w)
		{
			case 4:OLED_HFAny(x,y,4,5,c,Defu_XF4x5,color);break;
			case 10:OLED_HFAny(x,y,10,13,c,GeForce_10x13,color);break;
			case 13:OLED_HFAny(x,y,13,16,c,GeForce_13x16,color);break;
			case 19:OLED_HFAny(x,y,19,23,c,GeForce_19x23,color);break;
			case 25:OLED_HFAny(x,y,25,37,c,GeForce_25x37,color);break;
			default:OLED_HFAny(x,y,10,13,c,GeForce_10x13,color);break;
		}
		x+=w; 
		j++;
	}
}

void OLED_Driver::OLED_BFAny(int x,int y,int w,int h,u8 Num,const unsigned char *ch,uint16_t color)
{
	uint16_t i,j,k;
	uint16_t bnum = ((h+7)/8)*w;
	
	for(k=0;k<(h+7)/8;k++)//8*k��
		for(j=0;j<w;j++)//w
			for(i=0;i<8;i++)
				if((ch[Num*bnum+w*k+j]>>i)&1)
					Draw_Pixel(x+j,y+i+k*8,color);
}

void OLED_Driver::OLED_BFAny(int x,int y,int w,int h,u8 Num,const unsigned char *ch,uint16_t color,uint16_t hui)
{
	uint16_t i,j,k;
	u16 red,green,blue;
	u16 red1,green1,blue1;
	
	red = color>>11;
	green = (color&0x7E0)>>5;
	blue = color&0x1F;
	uint16_t bnum = ((h+7)/8)*w;
	
	for(k=0;k<(h+7)/8;k++)//8*k��
		for(j=0;j<w;j++)//w
			for(i=0;i<8;i++)
				if((ch[Num*bnum+w*k+j]>>i)&1)
				{
					red1 = red*(hui-abs(j-w/2))/hui;
					green1 = green*(8-abs(j-w/2))/hui;
					blue1=blue*(hui-abs(j-w/2))/hui;
					
					Draw_Pixel(x+j,y+i+k*8,red1<<11|(green1<<5)|(blue1));
				}
}
	
	
extern const unsigned char OCRB_F8x16[];
extern const unsigned char Self_F9x16[];
extern const unsigned char OCR_F10x16[];
extern const unsigned char OCR_F12x16[];
extern const unsigned char OCRB_F12x16[];
extern const unsigned char Agency_F12x24[];
extern const unsigned char OCR_F16x24[];
extern const unsigned char OCRB_F16x24[];
extern const unsigned char Agency_F22x40[];
//extern const unsigned char Robot_F32x46[];
//extern const unsigned char RobotT_F32x46[];
//extern const unsigned char RobotB_F32x46[];	
	
void OLED_Driver::OLED_SBFAny(int x,int y,char *ch,int w,uint16_t color)
{
	u8 c=0,j=0;
	while(ch[j]!='\0')
	{
		c=ch[j]-' ';
		
		switch(w)
		{
			case 8:OLED_BFAny(x,y,8,16,c,OCRB_F8x16,color,8);break;
			case 9:if(c==0)c=1;else c+=' '-'.';OLED_BFAny(x,y,9,16,c,Self_F9x16,color);break;
			case 10:OLED_BFAny(x,y,10,16,c,OCR_F10x16,color);break;
			case 12:OLED_BFAny(x,y,12,16,c,OCR_F12x16,color);break;
			case 13:OLED_BFAny(x,y,12,24,c,Agency_F12x24,color);break;
//			case 12:OLED_BFAny(x,y,12,16,c,OCRB_F12x16,color);break;
			case 16:OLED_BFAny(x,y,16,24,c,OCR_F16x24,color);break;
			case 22:OLED_BFAny(x,y,22,40,c,Agency_F22x40,color);break;
//			case 16:OLED_BFAny(x,y,16,24,c,OCRB_F16x24,color);break;
			default:OLED_BFAny(x,y,8,16,c,OCRB_F8x16,color);break;
		}
		x+=w; 
		j++;
	}
}
const unsigned char Font_Default6x8[] =
{
	0x7E,0x7E,0x42,0x42,0x7E,0x00,
	0x00,0x00,0x7E,0x7E,0x00,0x00,
	0x7A,0x7A,0x4A,0x4A,0x4E,0x00,
	0x42,0x4A,0x4A,0x7E,0x7E,0x00,
	0x0E,0x0E,0x08,0x08,0x7E,0x00,
	0x4E,0x4E,0x4A,0x4A,0x7A,0x00,
	0x7E,0x7E,0x4A,0x4A,0x7A,0x00,
	0x02,0x02,0x02,0x7E,0x7E,0x00,
	0x7E,0x7E,0x4A,0x4A,0x7E,0x00,
	0x4E,0x4A,0x4A,0x7E,0x7E,0x00,
	0x00,0x0C,0x0C,0x00,0x00,0x00,// .
	0x02,0x00,0x0E,0x0A,0x0A,0x02,
	0x08,0x08,0x08,0x08,0x08,0x00,
	0x00,0x6C,0x6C,0x00,0x00,0x00,// :
};

void OLED_Driver::OLED_NF6x8(int x,int y,u8 Num,u8 Offset)
{
	u8 i,j;
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<3;j++)
		{
			if(!(((Font_Default6x8[(Num+Offset)*6+j*2]>>i)&1)<<1))
				Draw_Pixel(x+j*2,y+i);
			if(!((Font_Default6x8[(Num+Offset)*6+j*2+1]>>i)&1))
				Draw_Pixel(x+j*2+1,y+i);
		}
	}
}	


void OLED_Driver::OLED_SNF6x8(int x,int y,char *ch)
{
	u8 c=0,j=0;
	for(j=0;j<8;j++)
		Draw_Pixel(x-1,y+j);
	j = 0;
	while(ch[j]!='\0')
	{
		c=ch[j];
		if(x<253)
		{
			if(c=='$')
				OLED_NF6x8(x,y,11,0);
			else if(c=='.')
				OLED_NF6x8(x,y,10,0);
			else if(c==':')
				OLED_NF6x8(x,y,13,0);
			else if(c=='-')
				OLED_NF6x8(x,y,12,0);
			else
			{
				c=ch[j]-'0';
				OLED_NF6x8(x,y,c,0);
			}
		}
		x+=6;
		j++;
	}
	for(j=0;j<8;j++)
		Draw_Pixel(x,y+j);
}
#ifdef __cplusplus
}
#endif


