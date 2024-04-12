#include "DS2438.h"
#include "delay.h"

float A, B, C, D, E, F;
float I, J;

u8 ds2438_reset(void)  //由主机发送复位信号，然后用presence去接2438给的应答信号，如果presence接到了0，说明2438有应答
{
	u8 presence;
	
	PB12L;
	delay_us(500);
	PB12H;
	delay_us(30);

	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) {
		presence = 1;
	} else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0) {
		presence = 0;
	}

	delay_us(300);
	return presence;
}
 
void ds2438_wr_byte(u8 cmd)
{  
	u8 i,j;

	for (j = 0; j < 8; j++) {
		i = cmd & 0x01;
		if (i == 0x01) {
			PB12L;
			delay_us(5);
			PB12H;
			delay_us(80);
		} else {
			PB12L;
			delay_us(60);
			PB12H;
			delay_us(25);
		}
		
		cmd = cmd >> 1;
	}
}
 
 
u8 ds2438_rd_byte(void)
{     
	u8 loop, result=0,tmp;

	for (loop = 0; loop < 8; loop++) {

		PB12L;
		delay_us(3);
		PB12H;

		delay_us(5);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) {
			tmp = 1;
		} else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0) {
			tmp = 0;
		}

		if (tmp) {
			tmp = tmp << loop;
			result |= tmp;
		}
		delay_us(58);
		delay_us(30);
	}
	return result;
}
 
void ds2438_initcommand(void)
{
	u8 r;

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	PB12H;

        r = ds2438_reset();
        while (r == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0x4E); // Write Scratchpad
        ds2438_wr_byte(0x00); // Write Page
        ds2438_wr_byte(0x07); // Write Cortrol   1.电流ad和ica使能  2.CCA/DCA 使能，数据将被存储并可从第 7 页，字节 4-7 检索 3.CCA/DCA 计数器数据将被映射到每次相应寄存器递增时 EEPROM
        r = ds2438_reset();
        while (r == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0xBE); // Read Scratchpad
        ds2438_wr_byte(0x00); // Read 0th Page
        r = ds2438_reset();
        while (r == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0x48); // Read Scratch Pad to Memory
        ds2438_wr_byte(0x00); // Copy 0th Page
        delay_ms(15);
}
 
float Read_Current(void)
{      
        u8 c[9];
        u8 i, j;
        float n;
        i = ds2438_reset();
        while(i == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0xB8); // Read Scratchpad
        ds2438_wr_byte(0x00); // Read 0th Page
        i = ds2438_reset();
        while(i == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0xBE); // Read Scratchpad
        ds2438_wr_byte(0x00); // Read 0th Page
        for (j = 0; j < 9; j++) {
            c[j] = ds2438_rd_byte();
        }
        n = ((c[6] & 0x03) * 256 + c[5]) / (4096 * R);         
        return (n);
}
 
float Read_Temperature(void)
{
	u8 g[9];
	u8 i,j;
	float u;
	i = ds2438_reset();
	while (i == 1);
	ds2438_wr_byte(0xCC); //Skip ROM
	ds2438_wr_byte(0x44); // Start Conversion

	delay_ms(15);
	i = ds2438_reset();
	while (i == 1);
	ds2438_wr_byte(0xCC); // Skip ROM
	ds2438_wr_byte(0xB8); //READ SCRATCHPAD
	ds2438_wr_byte(0x00); //READ 0TH PAGE
	i=ds2438_reset();
	while (i == 1);
	ds2438_wr_byte(0xCC); // Skip ROM
	ds2438_wr_byte(0xBE); // Read Scratch Pad
	ds2438_wr_byte(0x00);
	for (j = 0; j < 9; j++) {
		g[j] = ds2438_rd_byte();
	}
	u = ((g[2] & 0x7f) * 256 + g[1]) * .03125;
	return (u);
}
 
float Read_Voltage(void)
{      
	u8 b[9];
	u8 i,j;
	float m;
	i = ds2438_reset();
	while (i == 1);
	ds2438_wr_byte(0xCC); // Skip ROM
	ds2438_wr_byte(0xB4); //WRITE VOLTAGE CORTROL

	delay_ms(20);
	i = ds2438_reset();
	while (i == 1);
	ds2438_wr_byte(0xCC); // Skip ROM
	ds2438_wr_byte(0xB8); //READ SCRATCHPAD
	ds2438_wr_byte(0x00); //READ 0TH PAGE
	i=ds2438_reset();
	while (i == 1);         
	ds2438_wr_byte(0xCC); // Skip ROM
	ds2438_wr_byte(0xBE); //READ SCRATCHPAD
	ds2438_wr_byte(0x00); //READ 0TH PAGE
	for (j = 0; j < 9; j++) {
		b[j] = ds2438_rd_byte();
	}
	m = ((b[4] & 0x03) * 256 + b[3]) * 0.01;
	return (m);
}
 
float Read_ICAcurrent(void)
{      
        u8 d[9];
        u8 i,j;
        float p;
 
        i = ds2438_reset();
        while (i == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0xBE);
        ds2438_wr_byte(0x01);
        for (j = 0; j < 9; j++) {
		d[j] = ds2438_rd_byte();
        }
        p = d[4] / (2048 * R);
        return (p);
}
 
float Read_CCAcurrent(void)
{      
        u8 e[9];
        u8 i,j;
        float q;
        i = ds2438_reset();
        while (i == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0xB8); //READ SCRATCHPAD
        ds2438_wr_byte(0x07); //READ 07TH PAGE
        i = ds2438_reset();
        while (i == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0xBE);
        ds2438_wr_byte(0x07);
        for (j = 0; j < 9; j++) {
		e[j] = ds2438_rd_byte();
        }
        q = (e[5] * 256 + e[4]) * 0.015625;
        return (q);
}
 
float Read_DCAcurrent(void)
{      
        u8 f[9];
        u8 i,j;
        float t;
        i = ds2438_reset();
        while (i == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0xB8); //READ SCRATCHPAD
        ds2438_wr_byte(0x07); //READ 07TH PAGE
        i = ds2438_reset();
        while (i == 1);
        ds2438_wr_byte(0xCC); // Skip ROM
        ds2438_wr_byte(0xBE);
        ds2438_wr_byte(0x07);
        for (j = 0; j < 9; j++) {
		f[j] = ds2438_rd_byte();
        }
        t = (f[7] * 256 + f[6]) * 0.015625;
        return (t);
}
 
 
void ds2438_test(void)
{   
	C = Read_Current();     //读当前电流值
	A = Read_Temperature(); //读当前电池壳体的表面的温度
	B = Read_Voltage();     //读当前电池的电压值 
	D = Read_ICAcurrent();  //读当前电池的容量值
	E = Read_CCAcurrent();  //读当前充电电流的累计值 
	F = Read_DCAcurrent();  //读当前电池放电电流的累计值
	I = D / V_CC;           //当前电池的容量的百分比
	J = D / C;              //电池还能使用多长时间
}
