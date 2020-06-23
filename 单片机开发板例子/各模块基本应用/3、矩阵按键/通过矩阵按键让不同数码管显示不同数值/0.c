/******************************************************************
				

程序功能：按下键盘矩阵后，在六个数码管上依次显示所按下的键所代表的数字。
		  按复位键可以清空显示为0
******************************************************************/
#include<reg51.h>

#define uchar unsigned char

uchar a0=16,b0=16,c0=16,d0=16,e0=16,f0=16,wei,temp,key,i,j,k;
sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void delay(uchar i)
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
			   
          case 0xbb:
               key=10;
			   wei++;
               break;

          case 0x7b:
               key=11;
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
	      P3=0xf7;
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
          case 0xe7:
               key=12;
			   wei++;
               break;

          case 0xd7:
               key=13;
			   wei++;
               break;

          case 0xb7:
               key=14;
			   wei++;
               break;

          case 0x77:
               key=15;
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
}
}
  void main()
  {
  	while(1)
	{
  	keyscan();
	if(key!=20)
	{
	switch(wei)
		{
			case 1: a0=key;
					b0=16;
					c0=16;
					d0=16;
					e0=16;
					f0=16;
					break;
			case 2: b0=key;
					c0=16;
					d0=16;
					e0=16;
					f0=16;
					break;
			case 3: c0=key;
					d0=16;
					e0=16;
					f0=16;
					break;
			case 4: d0=key;
					e0=16;
					f0=16;
					break;
			case 5: e0=key;
					f0=16; 
					break;
			case 6: f0=key;
					wei=0;
					break;
	}
	key=20;
	}
	display(a0,b0,c0,d0,e0,f0);
}
}