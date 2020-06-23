
/************************************************************************************************
					   
程序功能：模拟简易门铃；且有关闭门铃功能。
		  当按下S2键后，蜂鸣器会发出十次短暂的“嘟嘟”声作为呼叫。
		  S3用于关闭或者打开门铃。
		  当门铃处于关闭时，按下S2不会有呼叫效果。
******************************************************************************************************/

#include<reg52.h>

#define uchar unsigned char 
unsigned char k;
uchar ok=1;//默认开机后，门铃开。
unsigned int j;
sbit s2=P3^4;
sbit s3=P3^5;
sbit beep=P2^3;
 
void delay(unsigned int i)
{
	for(j=i;j>0;j--)
	for(k=114;k>0;k--);
}

void main()
{
	delay(1);
	while(1)
	{
		if(ok==1)
		{
			if(s3==0)
			{
				while(s3==0);
				ok=0;
			 }
		}
		else
		{
			if(s3==0)
			{
				while(s3==0);
				ok=1;
			 }
		}
		if((s2==0)&(ok==1))
		{	uchar i=5;
			for(i=5;i>0;i--)
			{
				beep=0;
				delay(800);
				beep=1;
				delay(800);
		//		beep=0;
		//		delay(1200);
		//		beep=1;
		//		delay(1300);
			}
		}
	}
}


