/********************************************************************
				
程序功能：	使用学习板键盘最下面一排左侧的S2键，控制一LED的亮灭
			按下后松开，led亮；按下后再松开，led灭。
			即使led像家里用的电灯一样受开关控制。
			本例子中，要掌握“输入输出”的概念，单片机作为一个控制芯片，
			必然要通过“输入输出”才可与外界相连。输入是感知外界状态，当
			将输入作一定处理后，再通过输出控制外部模块。
**********************************************************************/

#include<reg51.h>

sbit s2=P3^4;	 //采集按键状态，此时作为输入引脚
sbit led=P1^0;	 //此时作为输出引脚。

void delay(unsigned char p)
{
	unsigned char m,n;
	for(m=p;m>0;m--)
		for(n=125;n>0;n--);
}

void main()
{	
	while(1)
	{  
	   if(s2==0)  
		{	
			delay(300);	 //消抖
			if(s2==0)
			{	while(!s2);//等待按键松开。
				led=~led;
			}
		}
	}
}