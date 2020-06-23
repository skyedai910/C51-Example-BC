#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit rs=P3^5;
sbit lcden=P3^4;
sbit dula=P2^6;
sbit wela=P2^7;
uchar table1[]="TX-51STAR MCU";
uchar table2[]="WWW.TXMCU.COM";
void delay(uint x)
{
	uint a,b;
	for(a=x;a>0;a--)
		for(b=10;b>0;b--); 
}

void delay1(uint x)
{
	uint a,b;
	for(a=x;a>0;a--)
		for(b=100;b>0;b--); 
}

void write_com(uchar com)
{
	P0=com;
	rs=0;
	lcden=0;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
	
}

void write_date(uchar date)
{
	P0=date;
	rs=1;
	lcden=0;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
	
}

void init()
{
	dula=0;
	wela=0;
	write_com(0x38);   //显示模式设置：16×2显示，5×7点阵，8位数据接口
	delay(20);
	write_com(0x0f);   //00001111显示模式设置
	delay(20);
	write_com(0x06);   //00000110显示模式设置：光标右移，字符不移
	delay(20);
	write_com(0x01);   //清屏幕指令，将以前的显示内容清除
	delay(20);	
}

void main()
{
	uchar a;
	init();

	write_com(0x80+17);	 //将第一个字符写在向右偏移17个字符处，为后面的由右向左划入做准备。
	delay(20);
	for(a=0;a<13;a++)
	{
	write_date(table1[a]);
	delay(20);		
	}

	write_com(0xc0+17);
	delay(50);
	for(a=0;a<13;a++)
	{
	write_date(table2[a]);
	delay(40);		
	}

	for(a=0;a<16;a++)
	{
		write_com(0x18); //左移00011000
		delay1(600);
	}
	while(1);
}
