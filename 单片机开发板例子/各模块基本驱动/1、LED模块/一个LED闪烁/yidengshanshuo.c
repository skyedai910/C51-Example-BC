/********************************************************************
			
程序功能：	让一LED闪烁十次后停止。

**********************************************************************/
#include <reg51.h>

sbit led=P2^3;

void delay(unsigned int i)
{
	unsigned int j;
	unsigned char k;
	for(j=i;j>0;j--)
		for(k=125;k>0;k--) ;
}

void main()
{
	unsigned char m;
	for(m=20;m>0;m--)
	{
		led=~led;	//！每隔一段时间即对输出取反。AA	55
		delay(3000); //闪烁间隔。
	}
	while(1); //闪烁十次后，到此不再闪烁。
}