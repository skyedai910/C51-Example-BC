/********************************************************************
				

程序功能：	利用delay（）函数，通过单片机自身的“数数”来延时。
			这个方法不但占用了单片机的资源，计时不精确，而且在动态显示
			数码管时，也有个缺点，即数码管的第二位由于扫描间隔过大，出现闪动的
			现象。（故这个方法只是用于和用定时器的方法延时做比较。要求精
			密的场合，并不提倡。）

**********************************************************************/
#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int

uchar j,k,i,a,A1,A2,second;


sbit dula=P2^6;
sbit wela=P2^7;
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                    0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};


sbit beep=P2^3;
sbit kaiguan=P3^4;

void delay()		   //大概0.5ms左右的延时
{
	uchar a;
	for(a=450;a>0;a--)
	{
		_nop_();
	}
}

void display(uchar sh_c,uchar g_c)
{
   dula=0;
   P0=table[sh_c];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay();
   
   P0=table[g_c];
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay();
}

void main()
  {
     while(1)
       { unsigned int m;
		 for(m=0;m<2000;m++)
		 {
		 	delay();
		 }	
         second++;
         if(second==60)
         second=0;
         A1=second/10;
         A2=second%10;
         for(a=50;a>0;a--)
         { display(A1,A2);};
         }

    }
