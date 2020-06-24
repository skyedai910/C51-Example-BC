/*
design for: at80c51 leds alternate open
author:https://www.mrskye.cn
*/
#include<reg51.h>

sbit led0 = P1^0;
sbit led1 = P1^1;
sbit led2 = P1^2;
sbit led3 = P1^3;
sbit led4 = P1^4;
sbit led5 = P1^5;
sbit led6 = P1^6;
sbit led7 = P1^7;

//自定义延迟函数
void delay(unsigned int i)
{
    unsigned int j;
    unsigned char k;
    for(j=i;j>0;j--)
    {
        for(k=125;k>0;k--);
    }
}

//switch led status
void switch_led()
{
    led0 = ~led0;
    led1 = ~led1;
    led2 = ~led2;
    led3 = ~led3;
    led4 = ~led4;
    led5 = ~led5;
    led6 = ~led6;
    led7 = ~led7;
}

void main()
{
    //init led status
    led0 = 1;
    led2 = 1;
    led4 = 1;
    led6 = 1;
	led1 = 0;
	led3 = 0;
	led5 = 0;
	led7 = 0;
    while(1)
    {
        delay(3000);
		switch_led();
    }
}