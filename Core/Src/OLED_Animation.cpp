#include "OLED_Animation.h"
#include "main.h"
#include "gpio.h"
#include "math.h"


#ifdef __cplusplus
extern "C"  {
#endif

u8 MindNum = 5;
u8 CircleNum = 1;
u8 SnowflakeNum = 5;
u8 MeteoNum = 5;
u8 PlanetNum = 5;
u8 TriangleNum = 2;

	
OLED_Animation::OLED_Animation(void) {
}

typedef struct
{
	float x;
	float y;	
	float dirx;
	float diry;
	float r;
	u16 color;
}MTMOVMIND;

MTMOVMIND mtmovmind[MINDMAX];
  

OLED_STATUS OLED_Animation::OLED_MovMind(u8 Index)
{
	if(mtmovmind[Index].x<=3)
	{	
		mtmovmind[Index].x = 4;
		return OLED_IDLE;
	}
	else if(mtmovmind[Index].x>=SCR_WIDTH-2)
	{
		mtmovmind[Index].x = SCR_WIDTH-3;
		return OLED_IDLE;
	}
	else if(mtmovmind[Index].y<=0)
	{
		mtmovmind[Index].y = 1;
		return OLED_IDLE;
	}
	else if(mtmovmind[Index].y>=SCR_HEIGHT-2)
	{
		mtmovmind[Index].y = SCR_HEIGHT-3;
		return OLED_IDLE;
	}
	else
	{
		mtmovmind[Index].x += mtmovmind[Index].dirx;
		mtmovmind[Index].y += mtmovmind[Index].diry;
	}
  return OLED_BUSY;
}

u16 GetMindDistanceSquare(u16 x0,u16 y0,u16 x1,u16 y1)
{
	return ((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
}

void OLED_Animation::Motion_MindInit(void)
{
	int i;
	for(i=0;i<MINDMAX;i++)
	{
		mtmovmind[i].color = oled.RandomColor();
		__ASM("NOP");
		mtmovmind[i].x = rand()%(SCR_WIDTH-4)+4;
		__ASM("NOP");
		mtmovmind[i].y = rand()%SCR_HEIGHT;
		
		mtmovmind[i].dirx = (rand()%30-15)*0.1f;
		mtmovmind[i].diry = (rand()%30-15)*0.1f;
		if(mtmovmind[i].dirx<0.2f&&mtmovmind[i].dirx>-0.2f)
			mtmovmind[i].dirx = 0.5f;
		if(mtmovmind[i].diry<0.2f&&mtmovmind[i].diry>-0.2f)
			mtmovmind[i].diry = 0.5f;
	}
//	MindNum = 5;
}


void OLED_Animation::Motion_Mind(void)
{
	int i,j;
	for(i=0;i<MindNum;i++)
	{
		if(OLED_MovMind(i) == OLED_IDLE)
		{
			mtmovmind[i].color = oled.RandomColor();
			mtmovmind[i].dirx = (rand()%30-15)*0.1f;
			mtmovmind[i].diry = (rand()%30-15)*0.1f;
			if(mtmovmind[i].dirx<0.2f&&mtmovmind[i].dirx>-0.2f)
				mtmovmind[i].dirx = 0.5f;
			if(mtmovmind[i].diry<0.2f&&mtmovmind[i].diry>-0.2f)
				mtmovmind[i].diry = 0.5f;
			
			if(MindNum<MINDMAX )
			{
				MindNum++;
			}
		}
	}
	
	for(i=0;i<MindNum;i++)
	{
		for(j=0;j<MindNum;j++)
		{
			if((mtmovmind[i].x-mtmovmind[j].x)*(mtmovmind[i].x-mtmovmind[j].x)+(mtmovmind[i].y-mtmovmind[j].y)*(mtmovmind[i].y-mtmovmind[j].y)<900)
			{
				oled.Draw_Line(mtmovmind[j].x,mtmovmind[j].y,mtmovmind[i].x,mtmovmind[i].y,color_half);
			}
		}
	}
	
	for(i=0;i<MindNum;i++)
		oled.Fill_Circle(mtmovmind[i].x,mtmovmind[i].y,2,mtmovmind[i].color);
}

typedef struct
{
	int x;
	int y;	
	float r;
	u16 color;
	float crt;
}MTCIRCLE;

MTCIRCLE mtcircle[CIRCLEMAX];

OLED_STATUS OLED_Animation::OLED_FucCircle(u8 Index)
{
	if(mtcircle[Index].crt<(mtcircle[Index].x%6+16))
	{
		if(mtcircle[Index].r-mtcircle[Index].crt<0)
		{
			mtcircle[Index].crt = 0;
			return OLED_IDLE;
		}
		oled.Fill_Circle(mtcircle[Index].x,mtcircle[Index].y,mtcircle[Index].r-mtcircle[Index].crt,mtcircle[Index].color);
		oled.Draw_Circle(mtcircle[Index].x,mtcircle[Index].y,mtcircle[Index].r+mtcircle[Index].crt,color_min);
		mtcircle[Index].crt+=0.4f;
		return OLED_BUSY;
	}
	else
	{
		mtcircle[Index].crt = 0;
		return OLED_IDLE;
	}
}
void OLED_Animation::Motion_CircleInit(void)
{
	int i;
	for(i=0;i<CIRCLEMAX;i++)
	{
			mtcircle[i].x = rand()%(SCR_WIDTH-12)+6;
			__ASM("NOP");
			mtcircle[i].y = rand()%SCR_HEIGHT;
			__ASM("NOP");
			mtcircle[i].r = rand()%10+2;
		__ASM("NOP");
			mtcircle[i].color = oled.RandomColor();

			if(CircleNum<CIRCLEMAX )
			{
				CircleNum++;
			}
	}
//	CircleNum=1;
}

void OLED_Animation::Motion_Circle(void)
{
	int i;
	for(i=0;i<CircleNum;i++)
	{
		if(OLED_FucCircle(i) == OLED_IDLE)
		{
			mtcircle[i].x = rand()%(SCR_WIDTH-12)+6;
			__ASM("NOP");
			mtcircle[i].y = rand()%SCR_HEIGHT;
			__ASM("NOP");
			mtcircle[i].r = rand()%10+2;
		__ASM("NOP");
			mtcircle[i].color = oled.RandomColor();

			if(CircleNum<CIRCLEMAX )
			{
				CircleNum++;
			}
		}
	}
}
typedef struct
{
	float x;
	float y;	
	float dirx;
	u16 color;
	u16 type;
}MTSNOWFLAKE;

MTSNOWFLAKE mtsnowflake[STARMAX];

OLED_STATUS OLED_Animation::OLED_MovSnowflake(u8 Index)
{
	mtsnowflake[Index].x+= mtsnowflake[Index].dirx;			// ��ȡ����֮��Ĳ�ֵ
	mtsnowflake[Index].y++;
	if(mtsnowflake[Index].y>SCR_HEIGHT||mtsnowflake[Index].x>SCR_WIDTH||mtsnowflake[Index].y<0||mtsnowflake[Index].x<0)
		return OLED_IDLE;
  return OLED_BUSY;
}

void OLED_Animation::Motion_SnowflakeInit(void)
{
	int i;
	for(i=0;i<STARMAX;i++)
	{
		mtsnowflake[i].x = rand()%(SCR_WIDTH-12)+6;
		mtsnowflake[i].y = 1;
		mtsnowflake[i].dirx = mtsnowflake[i].x+32;
		__ASM("NOP");
		mtsnowflake[i].type = rand()%5;
		mtsnowflake[i].color = oled.RandomColor();
		if(SnowflakeNum<STARMAX )
		{
			SnowflakeNum++;
		}
	}
//	SnowflakeNum = 5;
}

void OLED_Animation::Motion_Snowflake(void)
{
	int i,j;
//	oled.Draw_Line(MovSnowflake[0][0],mtsnowflake[i].y,mtsnowflake[i].dirx,95,15);
	for(i=0;i<SnowflakeNum;i++)
	{
		for(j=0;j<mtsnowflake[i].color%5+1;j++)
		{
			if(OLED_MovSnowflake(i) == OLED_IDLE)
			{
				mtsnowflake[i].x = rand()%(SCR_WIDTH-12)+6;
				mtsnowflake[i].y = 1;
				mtsnowflake[i].dirx = 32.0f/SCR_HEIGHT;
				__ASM("NOP");
				mtsnowflake[i].type = rand()%5;
				mtsnowflake[i].color = oled.RandomColor();
				if(SnowflakeNum<STARMAX )
				{
					SnowflakeNum++;
				}
			}
		}
		switch(mtsnowflake[i].type)
		{
			case 0:oled.Fill_Circle(mtsnowflake[i].x,mtsnowflake[i].y,1,mtsnowflake[i].color);break;
			case 1:oled.Fill_Circle(mtsnowflake[i].x,mtsnowflake[i].y,1,mtsnowflake[i].color);oled.Draw_Pixel(mtsnowflake[i].x,mtsnowflake[i].y,0);break;
			case 2:oled.Draw_Pixel(mtsnowflake[i].x,mtsnowflake[i].y,mtsnowflake[i].color);break;
			case 3:oled.Draw_Pixel(mtsnowflake[i].x,mtsnowflake[i].y,mtsnowflake[i].color);oled.Draw_Pixel(mtsnowflake[i].x+1,mtsnowflake[i].y+1,mtsnowflake[i].color);break;
			case 4:oled.Draw_Pixel(mtsnowflake[i].x,mtsnowflake[i].y,mtsnowflake[i].color);oled.Draw_Pixel(mtsnowflake[i].x,mtsnowflake[i].y+1,mtsnowflake[i].color);oled.Draw_Pixel(mtsnowflake[i].x+1,mtsnowflake[i].y,mtsnowflake[i].color);break;
		}
	}
}

typedef struct
{
	int start;
	u16 color;
	float spd;
	float x;
	float y;	
	int len;
	int type;
}MTMOVMETEOR;

MTMOVMETEOR mtmovmeteor[METEORMAX];
#define MOVMETEORDEF 60.0f
float movmeteorstep = MOVMETEORDEF / SCR_HEIGHT;

OLED_STATUS OLED_Animation::OLED_Movmeteor(u8 Index)
{
	mtmovmeteor[Index].y ++;
	mtmovmeteor[Index].x += movmeteorstep;
	if(mtmovmeteor[Index].y>SCR_HEIGHT)
		return OLED_IDLE;
	else
		return OLED_BUSY;
}

void OLED_Animation::Motion_MovmeteorInit(void)
{
	int i;
	for(i=0;i<METEORMAX;i++)
	{
		mtmovmeteor[i].start = rand()%SCR_WIDTH;
		mtmovmeteor[i].x = mtmovmeteor[i].start;
		mtmovmeteor[i].y = 0;
		mtmovmeteor[i].spd = rand()%5+1;//Speed
		__ASM("NOP");
		mtmovmeteor[i].type = rand()%4;//Type
		__ASM("NOP");
		mtmovmeteor[i].len = rand()%15;;//LineColor
		__ASM("NOP");
		mtmovmeteor[i].color = oled.RandomColor();
		__ASM("NOP");
		if(MeteoNum<METEORMAX )
		{
			MeteoNum++;
		}
	}
//	MeteoNum = 5;
}

void OLED_Animation::Motion_Movmeteor(void)
{
	int i,j;
	for(i=0;i<MeteoNum;i++)
	{
		for(j=0;j<mtmovmeteor[i].spd;j++)
		{
			if(OLED_Movmeteor(i) == OLED_IDLE)
			{
				mtmovmeteor[i].start = rand()%SCR_WIDTH;
				mtmovmeteor[i].x = mtmovmeteor[i].start;
				mtmovmeteor[i].y = 0;
				mtmovmeteor[i].spd = rand()%5+1;//Speed
				__ASM("NOP");
				mtmovmeteor[i].type = rand()%4;//Type
				__ASM("NOP");
				mtmovmeteor[i].len = rand()%15;;//LineColor
				__ASM("NOP");
				mtmovmeteor[i].color = oled.RandomColor();
				__ASM("NOP");
				if(MeteoNum<METEORMAX )
				{
					MeteoNum++;
				}
			}
		}
		if(mtmovmeteor[i].len>1)
		{
			mtmovmeteor[i].len--;
			oled.Draw_Line(mtmovmeteor[i].start,0,mtmovmeteor[i].x,mtmovmeteor[i].y,color_min);
		}
		else if((mtmovmeteor[i].y-(mtmovmeteor[i].spd*3))>0)
			oled.Draw_Line((mtmovmeteor[i].y-(mtmovmeteor[i].spd*3))*movmeteorstep+mtmovmeteor[i].start,mtmovmeteor[i].y-(mtmovmeteor[i].spd*3),mtmovmeteor[i].x,mtmovmeteor[i].y,color_half);
		switch(mtmovmeteor[i].type)
		{
			case 0:oled.Draw_Pixel(mtmovmeteor[i].x,mtmovmeteor[i].y,mtmovmeteor[i].color);break;
			case 1:oled.Draw_Pixel(mtmovmeteor[i].x+1,mtmovmeteor[i].y,mtmovmeteor[i].color);oled.Draw_Pixel(mtmovmeteor[i].x,mtmovmeteor[i].y+1,mtmovmeteor[i].color);break;
			case 2:oled.Fill_Circle(mtmovmeteor[i].x,mtmovmeteor[i].y,1,mtmovmeteor[i].color);break;
			case 3:oled.Draw_Circle(mtmovmeteor[i].x,mtmovmeteor[i].y,1,mtmovmeteor[i].color);oled.Draw_Pixel(mtmovmeteor[i].x,mtmovmeteor[i].y,0);break;
		}
	}
}

typedef struct
{
	int r;//���Ǵ�С
	u16 color;//������ɫ		
	float spd;//����ת��
	float orb;//�������й��
	float angle;//�������нǶ�
	float x;//����X
	float y;	//����Y
}MTPLANET;

MTPLANET mtplanet[PLANETMAX];

OLED_STATUS OLED_Animation::OLED_Planet(u8 Index){
	mtplanet[Index].angle+= mtplanet[Index].spd;
	if(mtplanet[Index].angle>=360)
	{
		mtplanet[Index].angle = 0;
		mtplanet[Index].orb++;
		if(mtplanet[Index].orb>=64)
			return OLED_IDLE;
	}
	return OLED_BUSY;
}

void OLED_Animation::Motion_PlanetInit(void){
	
	int i;
	for(i=0;i<PLANETMAX;i++)
	{
			mtplanet[i].r = rand()%4+1;	
				__ASM("NOP");
			mtplanet[i].color = oled.RandomColor();		
				__ASM("NOP");										
			mtplanet[i].spd = float(rand()%20+10)/5;	
				__ASM("NOP");
			mtplanet[i].orb = rand()%60+4;	
				__ASM("NOP");
			mtplanet[i].angle = 0;	
			
			mtplanet[i].x = 0;	
			mtplanet[i].y = 0;
			
			if(PlanetNum<PLANETMAX )
			{
				PlanetNum++;
			}
	}
//	PlanetNum = 5;
}


void OLED_Animation::Motion_Planet(void){
	
	int i,j;
	for(i=0;i<PlanetNum;i++)
	{
		if(OLED_Planet(i) == OLED_IDLE)
		{
			mtplanet[i].r = rand()%4+1;	//���Ǵ�С
				__ASM("NOP");
			mtplanet[i].color = oled.RandomColor();		//������ɫ		
				__ASM("NOP");										
			mtplanet[i].spd = float(rand()%20+10)/5;	//����ת��
				__ASM("NOP");
			mtplanet[i].orb = rand()%60+4;	//�������й��
				__ASM("NOP");
			mtplanet[i].angle = 0;	//�������нǶ�
			
			mtplanet[i].x = 0;	//����X
			mtplanet[i].y = 0;	//����Y
			
			if(PlanetNum<PLANETMAX )
			{
				PlanetNum++;
			}
		}
		mtplanet[i].x = OCX+double(mtplanet[i].orb*cos(mtplanet[i].angle*PI/180));
		mtplanet[i].y = OCY+double(mtplanet[i].orb*sin(mtplanet[i].angle*PI/180));
		
//		oled.Draw_Circle(OCX,OCY,mtplanet[i].orb,3);
		
	}
	
	
	for(i=0;i<PlanetNum;i++)
	{
		for(j=0;j<PlanetNum;j++)
		{
//			Temp = GetMindDistanceSquare(mtmovmind[i].x,mtmovmind[i].y,mtmovmind[j].x,mtmovmind[j].y);
			if((mtplanet[i].x-mtplanet[j].x)*(mtplanet[i].x-mtplanet[j].x)+(mtplanet[i].y-mtplanet[j].y)*(mtplanet[i].y-mtplanet[j].y)<1600)
			{
				if(mtplanet[i].r==1||mtplanet[j].r==1)
					;
				else
					oled.Draw_Line(mtplanet[j].x,mtplanet[j].y,mtplanet[i].x,mtplanet[i].y,color_half);
			}
		}
	}
	
//	for(i=0;i<PlanetNum-1;i++)
//	{
//		oled.Draw_Line(OCX+(mtplanet[i].orb*cos(mtplanet[i].angle*PI/180)),OCY+(mtplanet[i].orb*sin(mtplanet[i].angle*PI/180)),OCX+(MovPlanet[3][i+1]*cos(MovPlanet[4][i+1]*PI/180)),OCY+(MovPlanet[3][i+1]*sin(MovPlanet[4][i+1]*PI/180)),color_half);
//	}
//	oled.Draw_Line(OCX+(MovPlanet[3][0]*cos(MovPlanet[4][0]*PI/180)),OCY+(MovPlanet[3][0]*sin(MovPlanet[4][0]*PI/180)),OCX+(MovPlanet[3][PlanetNum-1]*cos(MovPlanet[4][PlanetNum-1]*PI/180)),OCY+(MovPlanet[3][PlanetNum-1]*sin(MovPlanet[4][PlanetNum-1]*PI/180)),color_half);
	
	for(i=0;i<PlanetNum;i++)
	{
		oled.Fill_Circle(mtplanet[i].x,mtplanet[i].y,mtplanet[i].r,mtplanet[i].color);
	}
	
}

typedef struct
{
	int r;//����Բ�뾶
	u16 color;//��������ɫ		
	float spd;//������ת��
	float angle;//�����νǶ�
	float x;  //����X
	float y;	//����Y
	float dirx;
	float diry;
	float delt1;
	float delt2;
}MTTRIANGLE;

MTTRIANGLE mttriangle[TRIANGLEMAX];

OLED_STATUS OLED_Animation::OLED_Triangle(u8 Index){
	mttriangle[Index].angle+= mttriangle[Index].spd;
	
	mttriangle[Index].x+=mttriangle[Index].dirx;
	mttriangle[Index].y+=mttriangle[Index].diry;
	
	if(mttriangle[Index].x-mttriangle[Index].r>SCR_WIDTH||mttriangle[Index].y-mttriangle[Index].r>SCR_HEIGHT||mttriangle[Index].x+mttriangle[Index].r<0||mttriangle[Index].y+mttriangle[Index].r<0)
	{
		return OLED_IDLE;
	}
	return OLED_BUSY;
}

void OLED_Animation::Motion_TriangleInit(void){
	
	int i;
	for(i=0;i<FULLTRIANGLEMAX;i++)
	{
		mttriangle[i].r=rand()%10+5;
			__ASM("NOP");
		mttriangle[i].color = oled.RandomColor();		
			__ASM("NOP");										
		mttriangle[i].spd = float(rand()%20+10)/15;
			__ASM("NOP");
		mttriangle[i].angle = rand()%60;	//���нǶ�30 150 270
		
		mttriangle[i].x = rand()%SCR_WIDTH;	
		mttriangle[i].y = rand()%SCR_HEIGHT;	
		
		mttriangle[i].dirx = rand()%20*0.1f-1.0f;
		if(mttriangle[i].dirx<0.2f&&mttriangle[i].dirx>-0.2f)
			mttriangle[i].dirx = 0.5f;
		mttriangle[i].diry = rand()%20*0.1f-1.0f;
		if(mttriangle[i].diry<0.2f&&mttriangle[i].diry>-0.2f)
			mttriangle[i].diry = 0.5f;
		mttriangle[i].delt1 = rand()%60+90;
		mttriangle[i].delt2 = rand()%60+210;
		
		if(TriangleNum<TRIANGLEMAX )
		{
			TriangleNum++;
		}
	}
//	TriangleNum = 2;
}


void OLED_Animation::Motion_Triangle(void){
	
	int i;
	for(i=0;i<TriangleNum;i++)
	{
		if(OLED_Triangle(i) == OLED_IDLE)
		{
			mttriangle[i].r=rand()%10+5;
				__ASM("NOP");
			mttriangle[i].color = oled.RandomColor();		
				__ASM("NOP");										
			mttriangle[i].spd = float(rand()%20+10)/15;
				__ASM("NOP");
			mttriangle[i].angle = rand()%60;	//���нǶ�30 150 270
			
			mttriangle[i].x = rand()%SCR_WIDTH;	
			mttriangle[i].y = rand()%SCR_HEIGHT;	
			
			mttriangle[i].dirx = rand()%20*0.1f-1.0f;
			if(mttriangle[i].dirx<0.2f&&mttriangle[i].dirx>-0.2f)
				mttriangle[i].dirx = 0.5f;
			mttriangle[i].diry = rand()%20*0.1f-1.0f;
			if(mttriangle[i].diry<0.2f&&mttriangle[i].diry>-0.2f)
				mttriangle[i].diry = 0.5f;
			mttriangle[i].delt1 = rand()%60+90;
			mttriangle[i].delt2 = rand()%60+210;
			
			if(TriangleNum<TRIANGLEMAX )
			{
				TriangleNum++;
			}
			else if(TriangleNum>TRIANGLEMAX )
				TriangleNum=TRIANGLEMAX;
		}
	}
	
	
	for(i=0;i<TriangleNum;i++)
	{				
		oled.Draw_Triangle(mttriangle[i].x+(mttriangle[i].r*cos(mttriangle[i].angle*PI/180)),mttriangle[i].y+(mttriangle[i].r*sin(mttriangle[i].angle*PI/180)),mttriangle[i].x+(mttriangle[i].r*cos((mttriangle[i].angle+mttriangle[i].delt1)*PI/180)),mttriangle[i].y+(mttriangle[i].r*sin((mttriangle[i].angle+mttriangle[i].delt1)*PI/180)),mttriangle[i].x+(mttriangle[i].r*cos((mttriangle[i].angle+mttriangle[i].delt2)*PI/180)),mttriangle[i].y+(mttriangle[i].r*sin((mttriangle[i].angle+mttriangle[i].delt2)*PI/180)),mttriangle[i].color);
		if(mttriangle[i].r>5)
			oled.Draw_Triangle(mttriangle[i].x+((mttriangle[i].r-5)*cos(mttriangle[i].angle*PI/180)),mttriangle[i].y+((mttriangle[i].r-5)*sin(mttriangle[i].angle*PI/180)),mttriangle[i].x+((mttriangle[i].r-5)*cos((mttriangle[i].angle+mttriangle[i].delt1)*PI/180)),mttriangle[i].y+((mttriangle[i].r-5)*sin((mttriangle[i].angle+mttriangle[i].delt1)*PI/180)),mttriangle[i].x+((mttriangle[i].r-5)*cos((mttriangle[i].angle+mttriangle[i].delt2)*PI/180)),mttriangle[i].y+((mttriangle[i].r-5)*sin((mttriangle[i].angle+mttriangle[i].delt2)*PI/180)),color_min);
		}
}


void OLED_Animation::Motion_TriangleF(void){
	
	int i;
	for(i=0;i<TriangleNum;i++)
	{
		if(OLED_Triangle(i) == OLED_IDLE)
		{
			mttriangle[i].r=rand()%10+5;
				__ASM("NOP");
			mttriangle[i].color = oled.RandomColor();		
				__ASM("NOP");										
			mttriangle[i].spd = float(rand()%20+10)/15;
				__ASM("NOP");
			mttriangle[i].angle = rand()%60;	//���нǶ�30 150 270
			
			mttriangle[i].x = rand()%SCR_WIDTH;	
			mttriangle[i].y = rand()%SCR_HEIGHT;	
			
			mttriangle[i].dirx = rand()%20*0.1f-1.0f;
			if(mttriangle[i].dirx<0.2f&&mttriangle[i].dirx>-0.2f)
				mttriangle[i].dirx = 0.5f;
			mttriangle[i].diry = rand()%20*0.1f-1.0f;
			if(mttriangle[i].diry<0.2f&&mttriangle[i].diry>-0.2f)
				mttriangle[i].diry = 0.5f;
			mttriangle[i].delt1 = rand()%60+90;
			mttriangle[i].delt2 = rand()%60+210;
			
			if(TriangleNum<FULLTRIANGLEMAX )
			{
				TriangleNum++;
			}
		}
	}
	
	
	for(i=0;i<TriangleNum;i++)
	{				
		oled.Fill_Triangle(mttriangle[i].x+((mttriangle[i].r)*cos(mttriangle[i].angle*PI/180)),mttriangle[i].y+((mttriangle[i].r)*sin(mttriangle[i].angle*PI/180)),mttriangle[i].x+((mttriangle[i].r)*cos((mttriangle[i].angle+mttriangle[i].delt1)*PI/180)),mttriangle[i].y+((mttriangle[i].r)*sin((mttriangle[i].angle+mttriangle[i].delt1)*PI/180)),mttriangle[i].x+((mttriangle[i].r)*cos((mttriangle[i].angle+mttriangle[i].delt2)*PI/180)),mttriangle[i].y+((mttriangle[i].r)*sin((mttriangle[i].angle+mttriangle[i].delt2)*PI/180)),mttriangle[i].color);

		oled.Draw_Triangle(mttriangle[i].x+((mttriangle[i].r+1)*cos(mttriangle[i].angle*PI/180)),mttriangle[i].y+((mttriangle[i].r+1)*sin(mttriangle[i].angle*PI/180)),mttriangle[i].x+((mttriangle[i].r+1)*cos((mttriangle[i].angle+mttriangle[i].delt1)*PI/180)),mttriangle[i].y+((mttriangle[i].r+1)*sin((mttriangle[i].angle+mttriangle[i].delt1)*PI/180)),mttriangle[i].x+((mttriangle[i].r+1)*cos((mttriangle[i].angle+mttriangle[i].delt2)*PI/180)),mttriangle[i].y+((mttriangle[i].r+1)*sin((mttriangle[i].angle+mttriangle[i].delt2)*PI/180)),color_half);
//		if(mttriangle[i].r>5)
	}
}

void OLED_Animation::OLED_AllMotion_Init(void)
{
	Motion_MindInit();
	Motion_CircleInit();
	Motion_SnowflakeInit();
	Motion_MovmeteorInit();
	Motion_PlanetInit();
	Motion_TriangleInit();
}

void OLED_Animation::OLED_CustormMotion(u8 Channel)
{
	if(Display_Mode==MODE_DATE&&Channel==5)
		Motion_TriangleF();
	else switch(Channel)
	{
	case 0:Motion_Mind();break;
	case 1:Motion_Movmeteor();break;
	case 2:Motion_Snowflake();break;
	case 3:Motion_Circle();break;
	case 4:Motion_Planet();break;
	case 5:Motion_Triangle();break;
	case 6:Motion_TriangleF();break;
	}
}
void OLED_Animation::OLED_AllMotion(uint8_t Type,u16 Speed)
{
	static long Count = 0;
	static uint8_t NowMotion = 5;
	static u16 SpeedSave = 0;
//	OLED_CustormMotion(4);return;
	if(SpeedSave!=Speed)
	{
		SpeedSave = Speed;Count = 0;
	}
	if((Type&0x3f) == 0)
	{
		Count = 0;
		NowMotion=6;
		return;
	}
	Count++;
	
	switch(Count/(5000-Speed*40))
	{
		case 0:
		if(Type&8){

 CircleNum = 1;
 SnowflakeNum = 5;
 MeteoNum = 5;
 PlanetNum = 5;
 TriangleNum = 2;

NowMotion=0; }else {Count=(5000-Speed*40);}   break;
		case 1:
		if(Type&4){ 
			
 MindNum = 5;
 CircleNum = 1;
 SnowflakeNum = 5;
 PlanetNum = 5;
 TriangleNum = 2;

NowMotion=1;}else {Count=(5000-Speed*40)*2;}   break;
		case 2:
		if(Type&2){

 MindNum = 5;
 CircleNum = 1;
 MeteoNum = 5;
 PlanetNum = 5;
 TriangleNum = 2;

NowMotion=2;}else {Count=(5000-Speed*40)*3;}   break;
		case 3:
		if(Type&1){
			
 MindNum = 5;
 SnowflakeNum = 5;
 MeteoNum = 5;
 PlanetNum = 5;
 TriangleNum = 2;

NowMotion=3;}else {Count=(5000-Speed*40)*4;}   break;
		case 4:
		if(Type&0x20){
			
 MindNum = 5;
 CircleNum = 1;
 SnowflakeNum = 5;
 MeteoNum = 5;
 TriangleNum = 2;

NowMotion=4;}else {Count=(5000-Speed*40)*5;}   break;
		case 5:
		if(Type&0x10){
			
 MindNum = 5;
 CircleNum = 1;
 SnowflakeNum = 5;
 MeteoNum = 5;
 PlanetNum = 5;

NowMotion=5;}else {Count=0;}   break;
		default:Count=0;break;
	}
	OLED_CustormMotion(NowMotion);
}



#ifdef __cplusplus
}
#endif


