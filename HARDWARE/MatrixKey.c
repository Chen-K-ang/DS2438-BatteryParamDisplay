#include "stm32f10x_flash.h"
#include"MatrixKey.h"

// 4×4键盘矩阵扫描函数			    键盘分布(不同题目不一样)       键值
// S7  S11  S15  S19  -----P30	    1   2    3   12		   7E  BE  DE EE  (修改密码)
// S6  S10  S14  S18  -----P31		4	5	 6   13	  	   7D  BD  DD ED （显示密码键）
// S5  S9   S13  S17  -----P32		7	8	 9	 14	 	   7B  BB  DB EB （取消修改）
// S4  S8   S12  S16  -----P33 		10  0   11   15		   77  B7  D7 E7 （键盘初始化,设置的密码不变，不可以在5分钟惩罚时按）
// 	|	|	 |	  |										（10为确定）	（11为清除键）
//	|	|	 |	   --------P34
//	|	|	  -------------P35
//	|    ------------------P36
//   ----------------------P37
uchar MatrixKeyScan(void)
{
    uchar temp1,temp2,temp; 
	uchar j=0;             //键值  
    P2 = 0xf0;             //P3.0-P3.3发全0,准备读列状态
	if((P2 & 0xf0) != 0xf0)	 //如果P3.4-P3.7不全为1,有键按下
	{	   
		delay10ms();  	 //延时消抖
    	if((P2 & 0xf0) != 0xf0)	//确认有键按下
		{
			temp1=P2 & 0xf0;   //读高4位状态		 
    		P2 = 0x0f;    //准备读低4位状态	    

 		    temp2=P2 & 0x0f;      //P1.0-P1.3发为0,读P2低4位状态	
			temp= temp1+temp2;
			while(P2 != 0x0f); //等待松开
			switch(temp)
    		{								   
  				case 0x7e:return 1;break;  //此处赋值视不同的按键分布，可以赋给不同的值
  				case 0x7d:return 4;break;  //本题中没有用到S16-S19，因此没有对这些按键赋值
  				case 0x7b:return 7;break; 
  				case 0x77:return 10;break; 
  				case 0xbe:return 2;break; 
  				case 0xbd:return 5;break;
  				case 0xbb:return 8;break;
  				case 0xb7:return 0;break;//设置键
  				case 0xde:return 3;break;
  				case 0xdd:return 6;break;
  				case 0xdb:return 9;break;
  				case 0xee:return 12;break;
				case 0xed:return 13;break;
				case 0xeb:return 14;break;
				case 0xe7:return 15;break;	
  				default:return 0xff;break;//错误组合，返回0xff
  			}
		}
	}//有键按下判断结束	
	return(0xff);//无键按下返回0xff 
}   
void delay10ms(void)
{
	unsigned char i,j,k;
	for(i=5;i>0;i--)
		for(j=4;j>0;j--)
			for(k=248;k>0;k--);
}
