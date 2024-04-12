#include "oled.h"
#include "ds18b20.h"
#include "DS2438.h"
#include "led_buzz.h"

unsigned char temp=0;
int i;
unsigned char tem0;
unsigned char tem1;
unsigned char m;
unsigned char temputer;
unsigned char x=0;

u8 DS18B20_ROM[8];

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();  //延时函数初始化	  
	led_buzz_init();
	ds2438_initcommand();
	OLED_Init();   //初始化OLED  
	OLED_Clear(); 
	while (1) {
		//ds2438测试读取
		ds2438_test();

		DS18B20_CheckDevice();   
		DS18B20_WriteByte(0xCC); 
		DS18B20_WriteByte(0x44); 
		delay_ms(500); 
		DS18B20_CheckDevice();   
		DS18B20_WriteByte(0x55); 
		
		for (i=0;i<8;i++) {
			DS18B20_WriteByte(DS18B20_ROM[i]); 
		}
		
		DS18B20_WriteByte(0xBE);     
		temp = DS18B20_ReadByte();     
		temp = DS18B20_ReadByte() << 8;
		
		printf("temp1=%d.%d\n",temp >> 4, temp & 0xF);
		printf("temp2=%f\n", temp * 0.0625);
		
		tem0 = temp << 5;
		tem0 = tem0 >> 9;
		tem1 = temp << 12;
		tem1 = tem1 >> 12;
		while (tem0 != 0) {
			m=tem0%10;
			tem0/=10;
			temputer += m * pow(2,x);
			++x;
		}
		delay_ms(500);
		if (temputer >= 117) {
			LED=0;
			BUZZ=0;		  
			delay_ms(300);//延时300ms
			LED=1;	  
			BUZZ=1;  
			delay_ms(300);//延时300ms
			BUZZ = 1;
		}

	}

}
