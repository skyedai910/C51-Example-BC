/******************************************************************
程序功能：模拟交通灯显示。
		  其中，红灯与绿灯显示时间均为50s，中间的黄灯显示时间为15s。
		  绿灯用八位发光二极管流动闪烁作代表；
		  红灯用八位发光二极管全亮作代表；
		  黄灯用八位发光二极管全亮全灭闪烁作代表。
		  刚开始显示的为绿灯。
		  显示顺序：绿--黄--红--黄--绿--黄--红--………………

******************************************************************/


#include<reg51.h>

#define uchar unsigned char
#define uint unsigned int

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;

unsigned int pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
uchar j,k,l=255;
uchar a1,a0;
uchar shijian,deng=2;
void delay(unsigned int i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display(unsigned char sh_c,unsigned char g_c)
{
   dula=0;
   P0=table[sh_c];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[g_c];
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);
}

void main()
{
	TMOD=0x01;
	TR0=1;
	TH0=(65536-46080)/256;// 由于晶振为11.0592,故所记次数应为46080，计时器每隔50000微秒发起一次中断。
	TL0=(65536-46080)%256;//46080的来历，为50000*11.0592/12
	ET0=1;
	EA=1;
	while(1)
	{	
		if(shijian==0)
		{
			switch(deng)
			{
				case 0: shijian=50;		  //红灯
						deng=1;
						break;
				case 1: shijian=15;		  //黄灯
						deng=2;
						break;
				case 2: shijian=50;		  //绿灯
						deng=3;
						break;
				case 3: shijian=15;		  //黄灯
						deng=0;
						break;
			}
		}
		if(pp==20)
		{	pp=0;
			shijian--;
		}
		a0=shijian%10;
		a1=shijian/10;
		display(a1,a0);
		switch(deng)
		{
			case 1: P1=0X00;			 //红灯
					break;
			case 2: 			//黄灯
					if(shijian%2==0)
					P1=0x00;
					if(shijian%2==1)
					P1=0xff;
					break;
			case 3: 		//绿灯
					if(shijian%2==0)
					P1=0x55;
					if(shijian%2==1)
					P1=0xaa;
					break;
			case 0:	if(shijian%2==0)	  //黄灯
					P1=0x00;
					if(shijian%2==1)
					P1=0xff;
					break;	
		}
	}
}

void time0() interrupt 1
{	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	pp++;
}
