/************************************************************************************************
					   
程序功能： 生活中常见一个按键随着按下次数不同对应不同功能，
		   本程序以学习板中的s2键为例，模拟其具有2种功能，
		   一是点亮一个发光二极管，二是蜂鸣器发声
******************************************************************************************************/

#include<reg52.h>

#define uchar unsigned char 
uchar num;

sbit beep=P2^3;
sbit led=P1^0;
sbit s2=P3^4;

void delay(uchar i)
{
	unsigned int j,k;
	for(j=i;j>0;j--)
		for(k=114;k>0;k--);		  //11.0592MHz时k=114延时为1ms；12MHz时k=123,延时1ms
}


void main()
{	delay(1) ;
	while(1)
	{
		if(s2==0)
		{
			delay(20);		 //消抖
			if(s2==0)
			{
				while(!s2);	 //等待按键松开
				num++;
			}
			switch(num)
			{
				case 1: led=0;	beep=1; break;
				case 2: beep=0;	led=1;	num=0; break;
			}
		}
	}
}