/***********************************************************************
程序功能：	本程序为用单片机制作的占空比均匀间隔可调PWM发生器。占空比可调间隔
			为0.1。即可取得占空比为0%，10%，20%……90%到100%的PWM波。
			该波的频率固定为1KH（周期1000微秒）。
			学习板上的操作键：
					S2----占空比加			S3----占空比减
**************************************************************************/

#include<reg52.h>

#define uint unsigned int 
uint pp;
char num=2,dis;
sbit pwm=P1^0;
sbit s2=P3^4;
sbit s3=P3^5;

sbit dula=P2^6;
sbit wela=P2^7;

unsigned char code table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void keyscan()
{
	if(s2==0)
	{
		while(!s2);
		num++;
		if(num==11)
			num=10;
		dis=num;
	}
	if(s3==0)
	{
		while(!s3);
		num--;
		if(num==-1)
			num=0;
		dis=num;
	}
}

void display(a)
{
   dula=0;
   P0=table[a];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;   
}

void main()
{
	TMOD=0x01;		 //模式设置，00000001，可见采用的是定时器0，工作与模式1（M1=0，M0=1）。
	TR0=1;			 //打开定时器
	TH0=0Xff;		 //定时器设置，每隔100微秒发起一次中断。
	TL0=0Xa4;
	ET0=1;			 //开定时器0中断
	EA=1;			 //开总中断
	while(1)
	{
		keyscan();
		if((num!=0)&&(num!=10))	//对于占空比为0和100%这两种极端情况，最好分离出，单独考虑
		{
			if(pp<=num)
				pwm=1;
			else
				pwm=0;
		}
		else if(num==0)	//当占空比为0
			pwm=0;
		else
			pwm=1;
		if(pp==10)		//当占空比为100%
			pp=0;
	display(num);//显示num当前值，占空比为num/10。
	}
}

void time0() interrupt 1
{	TH0=0Xff;
	TL0=0Xa4;
	pp++;
}
