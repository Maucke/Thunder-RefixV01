#include "i2c.h"
#include "main.h"

void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = DS_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = DS_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);		
}

void delay_us(uint16_t us)
{
	uint16_t i,j;
	for(i=0;i<us;i++)
		for(j=0;j<20;j++)
	;
}
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_SET);	  	  
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);
	delay_us(4);
 	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_RESET);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_RESET);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_SET);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_SET);delay_us(1);	   
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);delay_us(1);	 
	while(HAL_GPIO_ReadPin(DS_SDA_GPIO_Port, DS_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);
	SDA_OUT();
	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_RESET);
	delay_us(2);
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);
	SDA_OUT();
	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);
}					

GPIO_PinState JudgePin(u8 in)
{
	if(in)
		return GPIO_PIN_SET;
	else
		return GPIO_PIN_RESET;
}
	
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	  SDA_OUT(); 	    
    HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
    HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, JudgePin((txd&0x80)>>7));
    txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);
		delay_us(2); 
		HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);	
		delay_us(2);
    }	 
} 

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET); 
        delay_us(2);
		HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);
        receive<<=1;
        if(HAL_GPIO_ReadPin(DS_SDA_GPIO_Port, DS_SDA_Pin))receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK 
    return receive;
}

uint8_t read_IIC(uint8_t slave,uint8_t ReadAddr)
{				  
	  uint8_t temp=0;		  	    																 
    IIC_Start();  
    IIC_Send_Byte(slave);   //����������ַ,д���� 	   
	  IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //���͵͵�ַ
	  IIC_Wait_Ack();	    
	  IIC_Start();  	 	   
	  IIC_Send_Byte(slave|0x01);           //�������ģʽ			   
	  IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	  return temp;
}

void write_IIC(uint8_t slave,uint8_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    IIC_Start();  
    IIC_Send_Byte(slave);   //����������ַ,д���� 	 
	  IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr);   //���͵͵�ַ
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	  IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	  HAL_Delay(10);	 
}
uint8_t IIC_Write_Byte(uint8_t dat)
{
	uint8_t i;
	uint8_t IIC_Ack=0;	

	SDA_OUT(); 	    

	for(i = 0;i < 8;i++)
	{
		if(dat & 0x80)	
			HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_RESET);
			
		delay_us(2);
		HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);
	  delay_us(2);
		HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);
		dat<<=1;
	}

	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_SET);	//�ͷ�������

	SDA_IN();	//���ó�����

	delay_us(2);
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);
	delay_us(2);
	
	IIC_Ack |= HAL_GPIO_ReadPin(DS_SDA_GPIO_Port, DS_SDA_Pin);	//����Ӧ��λ
	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);
	return IIC_Ack;	//����Ӧ���ź�
}

uint8_t IIC_Read_Byte1(void)
{
	uint8_t i;
	uint8_t x=0;

	HAL_GPIO_WritePin(DS_SDA_GPIO_Port, DS_SDA_Pin, GPIO_PIN_SET);	//������������Ϊ�ߵ�ƽ

	SDA_IN();	//���ó�����

	for(i = 0;i < 8;i++)
	{
		x <<= 1;	//�������ݣ���λ��ǰ

		delay_us(2);
		HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_SET);	//ͻ��
		delay_us(2);
		
		if(HAL_GPIO_ReadPin(DS_SDA_GPIO_Port, DS_SDA_Pin))	x |= 0x01;	//�յ��ߵ�ƽ

		HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);
		delay_us(2);
	}	//���ݽ������

	HAL_GPIO_WritePin(DS_SCL_GPIO_Port, DS_SCL_Pin, GPIO_PIN_RESET);

	return x;	//���ض�ȡ��������
}

