/*********************************************************************

程序功能：	按下学习板上的S2按键时（一直按着），会发出频率为1000HZ和2000HZ交替
			的信号，通过蜂鸣器发出类似于救护车发出的报警声。

**********************************************************************/
#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int

sbit beep=P2^3;
sbit kaiguan=P3^4;

void delay()		   //大概0.5ms左右的延时
{
	uint a;
	for(a=55;a>0;a--)
	{
		_nop_();
	}
}
void main()
{	delay();
  	while(1)
	{
		if(!kaiguan)
		{
			uint m;
			for(m=800;m>0;m--)	  //持续时间0.5ms*800=0.4s
			{
				beep=~beep;		
				delay();		 //1000HZ的信号。
			}
			for(m=500;m>0;m--)	 //持续时间0.5ms*2*500 =0.5s
			{
				beep=~beep;
				delay();
				delay();		//500HZ的信号。
			}
		}
	}
}