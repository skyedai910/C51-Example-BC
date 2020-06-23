/********************************************************************
			

程序功能：	让学习板的第一个数码管动态地显示数字0到F。

**********************************************************************/
#include <reg51.h>

sbit dula=P2^6;		//段选信号的锁存器控制
sbit wela=P2^7;		//位选信号的锁存器控制

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
						//0-F的码表
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
		for(num=0;num<16;num++)
		{
			P0=table[num];	   
			dula=1;
			dula=0;

			P0=0xfe;	   //选中第一个数码管
			wela=1;
			wela=0;
			delay(1000);
		}
	}
}

