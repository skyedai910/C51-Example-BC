#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit rs=P3^5;
sbit lcden=P3^4;
sbit lcdrw=P3^7;

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

void write_com(uchar com)	 //些命令，RS=0
{
	lcdrw=0;
	P0=com;
	rs=0;
	lcden=0;
	delay(10);
	lcden=1;
	delay(10);
	lcden=0;
	
}

void write_data(uchar dat)	   //写数据，RS=1
{
        lcdrw=0;
	P0=dat;
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
	write_com(0x0f);   //显示模式设置
	delay(20);
	write_com(0x06);   //显示模式设置：光标右移，字符不移
	delay(20);
	write_com(0x01);   //清屏幕指令，将以前的显示内容清除
	delay(20);	
}
void main()
{
	uchar a;
	init();			  //1602初始化
	write_com(0x80);  //设置所要显示字符的位置为第一行第一列
	a=0;
	while(table1[a]!='\0')	//只要没有写到结束标志，就继续写
		{					//这是写字符的第一种方法，用while循环。
		write_data(table1[a]);
		a++;
		}
	write_com(0x80+0x40);  //设置第二行首字符显示的位置，为第二行，第一列
	a=0;
	for(a=0;a<13;a++)	   //总共要显示13个字符，故循环十三次。
	{					   //这是写字符的第二种方法，用for循环
	write_data(table2[a]);
	delay(10);		
	}
	while(1);
}