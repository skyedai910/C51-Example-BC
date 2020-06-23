/********************************************************************
				

程序功能：	让学习板的六位数码管同时显示数字0到5。

**********************************************************************/
#include <reg51.h>

sbit dula=P2^6;		//段选信号的锁存器控制
sbit wela=P2^7;		//位选信号的锁存器控制

unsigned char code wei[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};
						//数码管各位的码表
unsigned char code duan[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d};
						//0-5的码表
void delay(unsigned int i)
{
	unsigned int m,n;
	for(m=i;m>0;m--)
		for(n=90;n>0;n--);
}

void main()
{
	unsigned char num;
	while(1)
	{
		for(num=0;num<6;num++)
		{
			P0=wei[num];	   
			wela=1;
			wela=0;

			P0=duan[num];	  
			dula=1;
			dula=0;
			delay(2);	 //时间间隔短，这是关键（所谓的同时显示，只是间隔较短而已，利用人眼的余辉效应，觉得每个数码管都一直在亮）。
		 }
	}
}

