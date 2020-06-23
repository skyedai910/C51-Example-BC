/******************************************************************
				

程序功能：通过键盘采集所按下的数据，并在按下确定键（S17）后，将按下的键
		  组合为一整数。
		  					按键功能
						S6--S15 数字键0--9
						 	S17 确定输入
******************************************************************/
#include<reg51.h>

#define uchar unsigned char

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;

uchar a=16,b=16,c=16,d=16,e=16,f=16;
uchar wei,temp,key,ok;
uchar s1,s2,s3,s4,s5,s6;
unsigned long shu;

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};


void delay(uchar i)
{
	uchar j,k;
	for(j=i;j>0;j--)
	for(k=125;k>0;k--);
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
               key=0;
			   wei++;
               break;

          case 0xde:
               key=1;
			   wei++;
               break;

          case 0xbe:
               key=2;
			   wei++;
               break;

          case 0x7e:
               key=3;
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
               key=4;
			   wei++;
               break;

          case 0xdd:
               key=5;
			   wei++;
               break;

          case 0xbd:
               key=6;
			   wei++;
               break;

          case 0x7d:
               key=7;
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
    P3=0xfb;
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
          case 0xeb:
               key=8;
			   wei++;
               break;

          case 0xdb:
               key=9;
			   wei++;
               break;
		   case 0x7b:
               ok=1;
			   //key=11;
			   //wei++;
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

void main()
{
	while(1)
	{
		keyscan();
		if(wei!=0)					 //实时显示
		{
			switch(wei)
			{
				case 1: a=key;s1=a; break;
				case 2: b=key;s2=b; break;
				case 3: c=key;s3=c; break;
				case 4: d=key;s4=d; break;
				case 5: e=key;s5=e; break;
				case 6: f=key;s6=f; break;
			}
		}
		if(ok)			 //将采集到的单个数据转化为一个整数。
		{	ok=0;
			switch(wei)
			{
				case 1: shu=s1; break;
				case 2: shu=s2+s1*10; break;
				case 3: shu=s3+s2*10+s1*100; break;
				case 4: shu=s4+s3*10+s2*100+s1*1000; break;
				case 5: shu=s5+s4*10+s3*100+s2*1000+s1*10000; break;
				case 6: shu=s6+s5*10+s4*100+s3*1000+s2*10000+s1*100000; break;
			}
			wei=0;
		//	f=16;e=16;d=16;c=16;b=16;a=16;
			s1=0;s2=0;s3=0;s4=0;s5=0;s6=0;
		}
		display(a,b,c,d,e,f);
	}
}


