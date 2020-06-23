#include<reg51.h>

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
unsigned char j,k,a1,a0,s=60;
unsigned int m;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void delay(unsigned char i)
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
		if(m==20)
		{	m=0;
			s--;
		}
		a0=s%10;
		a1=s/10;
		display(a1,a0);
		if(s==0)
		{
			TR0=0;//时间到，定时器关闭
			beep=0;//蜂鸣器响，报时。
		}	
	}
}

void time0() interrupt 1
{TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	m++;
}
