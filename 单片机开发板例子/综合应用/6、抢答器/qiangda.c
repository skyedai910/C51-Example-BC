/		   按键功能简介
	 		S13 抢答开始	 S12 清零
	  		S6--S11   分别为1到6号选手按键
********************************************************************
功能简介：本例程为一抢答器；其中主持人操控S12与S13两个按键。选手共六位，分别操控S6--S11中的一个按键。
		  当主持人按下抢答开始按键S13后，倒计时开始，计时5s。此后最先按下按键的选手号码将显示与数码管上。
		  后来按下的将无显示。
		  若五秒计时结束后，再按下按键也不会显示。
		  若主持人没有按开始键，就有选手抢答，则视为犯规。此时犯规的选手号码将被显示于数码管上（最多显示五位犯规选手）
		  同时，蜂鸣器发出长笛声报警，数码管全亮。
		  而当主持人按下清零键S12后，一切状态均恢复，可以开始新一轮的抢答。	
***********************************************************************************************************************/


#include<reg51.h>

#define uchar unsigned char

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
uchar key,j,k,temp,daojishi=5,wei;
bit begin,end,clear,fangui;
uchar a0,b0=16;c0=16;d0=16;e0=16;f0=5;
unsigned int pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
void display(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f)
{
   dula=0;
   P0=table[a];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[b];
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   P0=table[c];
   dula=1;
   dula=0;
   
   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[d];
   dula=1;
   dula=0;
   
   P0=0xf7;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[e];
   dula=1;
   dula=0;
   
   P0=0xef;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[f];
   dula=1;
   dula=0;
   
   P0=0xdf;
   wela=1;
   wela=0;
   delay(5);
}

void keyscan()
{
	P3=0xfe;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xee:
               key=1;
			   wei++;
               break;

          case 0xde:
               key=2;
			   wei++;
               break;

          case 0xbe:
               key=3;
			   wei++;
               break;

          case 0x7e:
               key=4;
			   wei++;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
    }
    P3=0xfd;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xed:
               key=5;
			   wei++;
               break;

          case 0xdd:
               key=6;
			   wei++;
               break;

          case 0xbd:
               clear=1;
			   //key=7;
               break;

          case 0x7d:
		  	   begin=1;
               //key=8;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
      }
}

void main()
{
	TMOD=0x01;
	
	TH0=(65536-46080)/256;// 由于晶振为11.0592,故所记次数应为46080，计时器每隔50000微秒发起一次中断。
	TL0=(65536-46080)%256;//46080的来历，为50000*11.0592/12
	ET0=1;
	EA=1;
	while(1)
	{
		keyscan();
		if(begin)
		{
			if(!end)
			{
				TR0=1;
				//if(!daojishi)	daojishi=5;
				//begin=0;
				//daojishi=5;
				if(pp==20)
				{
					pp=0;
					daojishi--;
					
				}

			f0=daojishi;
			if(key!=0)
			a0=key;
		//	a1=a0;
			
			//if(a0!=0)	end=1;
			if(!daojishi|a0)
			{
				end=1;
				TR0=0;
				//daojishi=5;
			//	begin=0;
				pp=0;
			}
			}
			
	}
	else
	{	
			if(key!=0)
		{
			switch(wei)
			{
				case 1: a0=key;break;
				case 2: b0=key;break;
				case 3: c0=key;break;
				case 4: d0=key;break;
				case 5: e0=key;break;
			}
			//if(!fangui)
				fangui=1;
				beep=0;
		}
	}
	if(clear)
	{
		a0=0;key=0;begin=0;end=0;fangui=0;beep=1;daojishi=5;f0=5;clear=0;
		b0=16;c0=16;d0=16;e0=16;wei=0; P1=0xff;
	}
	display(a0,b0,c0,d0,e0,f0);
	if(fangui)	
	{
		beep=0;
		P1=0x00;;
	}

}
}

void time0() interrupt 1
{	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	pp++;
}
