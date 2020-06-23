/**************************************************************************
程序功能：  该程序只适用于11.0592MHz的晶振。
			用于一些常用的小延时。
			但用的是软件延时的方法，占用CPU资源。
*******************************************************************************/

#include <reg51.h>

void delay100ms(void)   //延时100ms  误差 -0.000000000023us
{
    unsigned char a,b;
    for(b=221;b>0;b--)
        for(a=207;a>0;a--);
}

void delay10ms(void)   //延时10ms， 误差 -0.000000000002us
{
    unsigned char a,b;
    for(b=249;b>0;b--)
        for(a=17;a>0;a--);
}

void delay1ms(void)   //延时1ms，误差 -0.651041666667us
{
    unsigned char a,b;
    for(b=102;b>0;b--)
        for(a=3;a>0;a--);
}

void delay100us(void)   //延时100微秒，误差 -0.173611111111us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=43;a>0;a--);
}

void delay10us(void)   //延时10微秒 误差 -0.234375us
{
    unsigned char a;
    for(a=3;a>0;a--);
}

void main()
{
	char m;
	for(m=0;m<4;m++)	 //举例，延时4*10ms=40ms。
	{
		delay10ms();
	}
}