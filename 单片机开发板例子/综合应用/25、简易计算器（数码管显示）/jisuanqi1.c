/******************************************************************
按键说明
	S6--S11  数字0--9的输入		S12	清零
				S13	等于			S14--S17加减乘除
程序功能：	本程序为简易计算器。可以算整数且正数类型的运算。
			但是暂时没有实现其连算功能，故在每次运算之后，请按下清零键。
******************************************************************/
#include<reg51.h>

#define uchar unsigned char

uchar a0=16,b0=16,c0=16,d0=16,e0=16,f0=16,wei,temp,key,i,j,k;
uchar jia,jian,cheng,chu,dengyu,jia0,jian0,cheng0,chu0,qingling;
uchar s0,s1,s2,s3,s4,s5; //参加运算的各个位
unsigned long qian,hou;//定义参于运算的第一个数和第二个数。
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
               qingling=1;
			   //key=10;
			   //wei++;
               break;

          case 0x7b:
               dengyu=1;
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
		  	   jia=1; 
               //key=12;
			  // wei++;
               break;

          case 0xd7:
               jian=1;
               break;

          case 0xb7:
               cheng=1;
               break;

          case 0x77:
               chu=1;
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

void display0()
{
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
					//wei=0;
					break;
	}
	key=20;
	}
	display(a0,b0,c0,d0,e0,f0);
	if(a0!=16) s5=a0;
	if(b0!=16) s4=b0;
	if(c0!=16) s3=c0;
	if(d0!=16) s2=d0;
	if(e0!=16) s1=e0;
	if(f0!=16) s0=f0;
}


void main()
{	uchar gongneng,yunsuan;
  	while(1)
	{
	  	keyscan();
		gongneng=jia|jian|cheng|chu|dengyu|qingling;
		if(gongneng==0)
		{
			display0();	
		}
		else
		{  	yunsuan=jia|jian|cheng|chu;
			if(yunsuan)
			{	
	
				a0=16;
				b0=16;
				c0=16;
				d0=16;
				e0=16;
				f0=16;
				//wei=0;
				if(jia)
				{jia=0;	jia0=1;jian0=0;cheng0=0;chu0=0;}
				if(jian)
				{jian=0;jia0=0;jian0=1;cheng0=0;chu0=0;}
				if(cheng)
				{cheng=0;jia0=0;jian0=0;cheng0=1;chu0=0;}
				if(chu)
				{chu=0;	jia0=0;jian0=0;cheng0=0;chu0=1;}
				switch(wei)
				{
					case 6:
					qian=s5*100000+s4*10000+s3*1000+s2*100+s1*10+s0;break;
					case 5:
					qian=s5*10000+s4*1000+s3*100+s2*10+s1;break;
					case 4:
					qian=s5*1000+s4*100+s3*10+s2;break;
					case 3:
					qian=s5*100+s4*10+s3;break;
					case 2:
					qian=s5*10+s4;break;
					case 1:
					qian=s5;break;
				}
				wei=0;
				s0=0;s1=0;s2=0;s3=0;s4=0;s5=0;
				display(a0,b0,c0,d0,e0,f0);
				//P1=0x55;
			}
			if(dengyu)
			{
				 dengyu=0;
				 //display(16,16,16,16,16,16);
				 switch(wei)
				{
					case 6:
					hou=s5*100000+s4*10000+s3*1000+s2*100+s1*10+s0;break;
					case 5:
					hou=s5*10000+s4*1000+s3*100+s2*10+s1;break;
					case 4:
					hou=s5*1000+s4*100+s3*10+s2;break;
					case 3:
					hou=s5*100+s4*10+s3;break;
					case 2:
					hou=s5*10+s4;break;
					case 1:
					hou=s5;break;
				}
				wei=0;
				s0=0;s1=0;s2=0;s3=0;s4=0;s5=0;
				 if(jia0)
				 {jia0=0;	hou=qian+hou;}
				 if(jian0)
				 {jian0=0;	hou=qian-hou;}
				 if(cheng0)
				 {cheng0=0;	hou=qian*hou;}
				 if(chu0)
				 {chu0=0;	hou=qian/hou;}
				 if(hou<10)
				 {
				 	a0=hou;b0=16;c0=16;d0=16;e0=16;f0=16;
				 }
				 else
				 {
				 	if(hou<100)
					{a0=hou/10;b0=hou%10;c0=16;d0=16;e0=16;f0=16;}
					else
					{
						if(hou<1000)
						{a0=hou/100;b0=hou%100/10;c0=hou%10;d0=16;e0=16;f0=16;}
						else
						{
							if(hou<10000)
							{a0=hou/1000;b0=hou%1000/100;c0=hou%100/10;d0=hou%10;e0=16;f0=16;}
							else
							{
								if(hou<100000)
								{a0=hou/10000;b0=hou%10000/1000;c0=hou%1000/100;d0=hou%100/10;e0=hou%10;f0=16;}
								else
								{
									if(hou<1000000)
									{a0=hou/100000;b0=hou%100000/10000;c0=hou%10000/1000;d0=hou%1000/100;e0=hou%100/10;f0=hou%10;}
								}
							}
						}
					}
				 }
				 display(a0,b0,c0,d0,e0,f0);
				 P1=hou;
	
			}
			if(qingling)
			{
				a0=16;b0=16;c0=16;d0=16;e0=16;f0=16;
				s0=0;s1=0;s2=0;s3=0;s4=0;s5=0;
				jia=0;jian=0;cheng=0;chu=0;
				jia0=0;jian0=0;cheng0=0;chu0=0;
				qingling=0;dengyu=0;
				qian=0;hou=0;
				display(a0,b0,c0,d0,e0,f0);
			}
			
		}

	}
}