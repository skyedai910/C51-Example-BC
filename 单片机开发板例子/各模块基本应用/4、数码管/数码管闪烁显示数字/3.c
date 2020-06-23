/********************************************************************
				

程序功能：	让学习板的第一位闪烁地显示数字3。

**********************************************************************/
#include <reg51.h>

sbit dula=P2^6;		//段选信号的锁存器控制
sbit wela=P2^7;		//位选信号的锁存器控制

void delay(unsigned int i)
{
	unsigned int m,n;
	for(m=i;m>0;m--)
		for(n=90;n>0;n--);
}

void main()
{
	while(1)
	{
			P0=0xfe;	   
			wela=1;
			wela=0;

			P0=0x4f;	  //数字“3”的代码
			dula=1;
			dula=0;
			delay(1000);

			P0=0x00;	  //什么都不显示的代码
			dula=1;
			dula=0;
			delay(1000);
	}
}

