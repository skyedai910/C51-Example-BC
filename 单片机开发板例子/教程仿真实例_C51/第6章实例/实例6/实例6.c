#include <reg51.h>
#define uchar unsigned char
uchar time;			 //中断次数
uchar period=250;	 //1个周期的次数
uchar high=50;		 //20%高电平的次数

timer0() interrupt 1{//T0中断函数
	TH0=0xd1;		 //重装载计数初值
	TL0=0x20;
	if (++time==high) P2=0;//高电平时间到，P2变低
	else if (time==period)
		{time=0;P2=1;}//周期时间到，P2变高
}

void main (){
   TMOD = 0x01; 	//T0定时方式1
   TH0 = 0xd1;		//首次装入计数初值
   TL0 = 0x20;		
   EA=ET0=1;		
   TR0 = 1;			//启动计数器
   do { }while (1);
}