//可实现计时功能，最大计时时间为99小时。
//支持暂停功能，按键盘中左下角的S2键，可暂停计时;按下S3，可继续计时。
//注意晶振需为11.0592M。
//若为其他数值晶振，请改变TH0与TL0参数值，否则计时会有很大误差 。

#include<reg51.h>

#define uchar unsigned char

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
sbit lcden=P3^4;
sbit rs=P3^5;
unsigned char halt,j,k,a1,a0,b1,b0,c1,c0,s,f,m,n=255;
unsigned int pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display(uchar shi2,uchar shi1,uchar fen2,uchar fen1,uchar miao2,uchar miao1)
{
   dula=0;
   P0=table[shi2];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[shi1]|0x80;
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   P0=table[fen2];
   dula=1;
   dula=0;
   
   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[fen1]|0x80;
   dula=1;
   dula=0;
   
   P0=0xf7;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[miao2];
   dula=1;
   dula=0;
   
   P0=0xef;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[miao1];
   dula=1;
   dula=0;
   
   P0=0xdf;
   wela=1;
   wela=0;
   delay(5);
}

void keyscan()
{
	if(lcden==0)
	delay(10);
	if(lcden==0)
	{
		halt=1;
	}
	if(rs==0)
	halt=0;
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
	{	keyscan();
		if(halt==0)
		{
		TR0=1;
		if(pp==20)
		{	pp=0;
			m++;
			n--;
			P1=n;//闪烁灯
			if(m==60)
			{
				m=0;
				f++;
				if(f==60)
				{
					f=0;
					s++;
					if(s==99)
					{
						s=0;
					}
				}
			}
		}
		
		a0=s%10;
		a1=s/10;
		b0=f%10;
		b1=f/10;
		c0=m%10;
		c1=m/10;
		display(a1,a0,b1,b0,c1,c0);
	}
	else 
	TR0=0;
	display(a1,a0,b1,b0,c1,c0);
	}
}

void time0() interrupt 1
{TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	pp++;
}
