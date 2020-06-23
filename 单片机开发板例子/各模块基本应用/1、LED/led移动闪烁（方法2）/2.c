/********************************************************************
				
程序功能：	使八位LED逐位移动并不断循环。
			该方法采用逐个查表赋值的方法

**********************************************************************/
#include <reg51.h>



unsigned char table[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

void delay(unsigned int i)
{
	unsigned int j;
	unsigned char k;
	for(j=i;j>0;j--)
		for(k=125;k>0;k--);
}

void main()
{
	while(1)
	{
		unsigned char m;
		for(m=0;m<8;m++)
		{
			P1=table[m];
			delay(2000);
		}
	}
}