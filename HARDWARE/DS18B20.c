#include "ds18b20.h"
 


void DS18B20_Init(void)
{
		RCC->APB2ENR|=1<<3; //PB
		GPIOB->CRH&=0x0FFFFFFF;
		GPIOB->CRH|=0x30000000;
		GPIOB->ODR|=1<<15; 
}
 
 


u8 DS18B20_CheckDevice(void) 
{
		DS18B20_OUTPUT_MODE();
		DS18B20_OUT=0;        
		delay_us(750);        
		DS18B20_OUT=1;       
		delay_us(15);        
		if(DS18B20_CleckAck())
		{
				return 1;
		}
		return 0;
}

u8 DS18B20_CleckAck(void)
{
		u8 cnt=0;
		DS18B20_INPUT_MODE();
		while(DS18B20_IN&&cnt<200) 
		{
				delay_us(1);
				cnt++;
		}
		if(cnt>=200)return 1;
		
		cnt=0;
		while((!DS18B20_IN)&&cnt<240) 
		{
				delay_us(1);
				cnt++;
		}
		if(cnt>=240)return 1; 
		return 0;
}
 
 

void DS18B20_WriteByte(u8 cmd)
{
		u8 i;
		DS18B20_OUTPUT_MODE(); 
		for(i=0;i<8;i++)
	  {
				DS18B20_OUT=0; 
				delay_us(2);
				DS18B20_OUT=cmd&0x01; 
				delay_us(60);   
				DS18B20_OUT=1;  
				cmd>>=1;       
		}
}
 

u8 DS18B20_ReadByte(void)
{
	 u8 i,data=0;	
	 for(i=0;i<8;i++)
	 {
			DS18B20_OUTPUT_MODE(); 
			DS18B20_OUT=0; 
			delay_us(2);
			DS18B20_OUT=1;  
			DS18B20_INPUT_MODE(); 
			delay_us(8);   
			data>>=1;     
		  if(DS18B20_IN) data|=0x80;
			delay_us(60);
			DS18B20_OUT=1; 
	 }
	 return data;
}
 
 

u16 DS18B20_ReadTemp(void)
{
	  u16 temp=0;
		u8 temp_H,temp_L;
	  DS18B20_CheckDevice();  
		DS18B20_WriteByte(0xCC);
		DS18B20_WriteByte(0x44); 
		
	
		while(DS18B20_ReadByte()!=0xFF){}
		
		DS18B20_CheckDevice();  
		DS18B20_WriteByte(0xCC); 
		DS18B20_WriteByte(0xBE);
		
		temp_L=DS18B20_ReadByte(); 
		temp_H=DS18B20_ReadByte();
		temp=temp_L|(temp_H<<8); 
		return temp;
}

