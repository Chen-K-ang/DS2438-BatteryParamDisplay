#include "stm32f10x_flash.h"
#include"MatrixKey.h"

// 4��4���̾���ɨ�躯��			    ���̷ֲ�(��ͬ��Ŀ��һ��)       ��ֵ
// S7  S11  S15  S19  -----P30	    1   2    3   12		   7E  BE  DE EE  (�޸�����)
// S6  S10  S14  S18  -----P31		4	5	 6   13	  	   7D  BD  DD ED ����ʾ�������
// S5  S9   S13  S17  -----P32		7	8	 9	 14	 	   7B  BB  DB EB ��ȡ���޸ģ�
// S4  S8   S12  S16  -----P33 		10  0   11   15		   77  B7  D7 E7 �����̳�ʼ��,���õ����벻�䣬��������5���ӳͷ�ʱ����
// 	|	|	 |	  |										��10Ϊȷ����	��11Ϊ�������
//	|	|	 |	   --------P34
//	|	|	  -------------P35
//	|    ------------------P36
//   ----------------------P37
uchar MatrixKeyScan(void)
{
    uchar temp1,temp2,temp; 
	uchar j=0;             //��ֵ  
    P2 = 0xf0;             //P3.0-P3.3��ȫ0,׼������״̬
	if((P2 & 0xf0) != 0xf0)	 //���P3.4-P3.7��ȫΪ1,�м�����
	{	   
		delay10ms();  	 //��ʱ����
    	if((P2 & 0xf0) != 0xf0)	//ȷ���м�����
		{
			temp1=P2 & 0xf0;   //����4λ״̬		 
    		P2 = 0x0f;    //׼������4λ״̬	    

 		    temp2=P2 & 0x0f;      //P1.0-P1.3��Ϊ0,��P2��4λ״̬	
			temp= temp1+temp2;
			while(P2 != 0x0f); //�ȴ��ɿ�
			switch(temp)
    		{								   
  				case 0x7e:return 1;break;  //�˴���ֵ�Ӳ�ͬ�İ����ֲ������Ը�����ͬ��ֵ
  				case 0x7d:return 4;break;  //������û���õ�S16-S19�����û�ж���Щ������ֵ
  				case 0x7b:return 7;break; 
  				case 0x77:return 10;break; 
  				case 0xbe:return 2;break; 
  				case 0xbd:return 5;break;
  				case 0xbb:return 8;break;
  				case 0xb7:return 0;break;//���ü�
  				case 0xde:return 3;break;
  				case 0xdd:return 6;break;
  				case 0xdb:return 9;break;
  				case 0xee:return 12;break;
				case 0xed:return 13;break;
				case 0xeb:return 14;break;
				case 0xe7:return 15;break;	
  				default:return 0xff;break;//������ϣ�����0xff
  			}
		}
	}//�м������жϽ���	
	return(0xff);//�޼����·���0xff 
}   
void delay10ms(void)
{
	unsigned char i,j,k;
	for(i=5;i>0;i--)
		for(j=4;j>0;j--)
			for(k=248;k>0;k--);
}
