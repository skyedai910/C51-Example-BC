/********************************************************************
				
程序功能：	使八位LED逐位移动并不断循环。
			这种方法最简单。

**********************************************************************/
#include <reg51.h>
#include <intrins.h>  //这个头文件是这个方法的核心，它包含了逐步移位函数功能
					  //即程序中的_crol_(P1,y)

sbit led=P1^0;

void delay(unsigned int i)
{
	unsigned int j;
	unsigned char k;
	for(j=i;j>0;j--)		//约300ms
		for(k=144;k>0;k--);	 //(132,11.0592MHz).(144,12MHz)约0.3ms
}

void main()
{	P1=0xfe;
	delay(1000);
	while(1)
	{
		unsigned char m;
		P1=0xfe;
		delay(1000);
		for(m=7;m>0;m--)
		{
			P1=_crol_(P1,1);   //P1逐步左移一位。
			delay(1000);
		}
	}
}