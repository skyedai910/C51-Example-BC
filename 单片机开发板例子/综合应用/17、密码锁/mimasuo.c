/*******************************************************************************
						功能键
						S6---S15	数字键0-9
				S16---更改密码		S17---更改密码完毕后确认
				S18---重试密码、重新设定	S19---关闭密码锁
				初始密码：000000 	密码位数：6位
				注意：掉电后，所设密码会丢失，重新上点时，密码恢复为原始的000000
				与P1相连的8位发光LED点亮代表锁被打开；熄灭代表锁被锁上

程序功能：
		  1、开锁：
		  下载程序后，直接按六次S7（即代表数字1），8位LED亮，锁被打开，输入密码时，
		  六位数码管依次显示小横杠。
		  2、更改密码：
		  只有当开锁（LED亮）后，该功能方可使用。
		  首先按下更改密码键S16，然后设置相应密码，此时六位数码管会显示设置密码对应
		  的数字。最后设置完六位后，按下S17确认密码更改，此后新密码即生效。
		  3、重试密码：
		  当输入密码时，密码输错后按下键S18，可重新输入六位密码。
		  当设置密码时，设置中途想更改密码，也可按下此键重新设置。
		  4、关闭密码锁：
		  按下S19即可将打开的密码锁关闭。
推荐初级演示步骤：输入原始密码000000---按下更改密码按键S16---按0到9设置密码---按S17
			确认密码更改---按S18关闭密码锁---输入新的密码打开密码锁
*******************************************************************************/
#include<reg52.h>

#define uchar unsigned char
#define uint unsigned int

uchar old1,old2,old3,old4,old5,old6; //原始密码000000
uchar new1,new2,new3,new4,new5,new6;  //每次MCU采集到的密码输入
uchar a=16,b=16,c=16,d=16,e=16,f=16; //送入数码管显示的变量
uchar wei,key,temp;

bit allow,genggai,ok,wanbi,retry,close;	 //各个状态位

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;

unsigned char code table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00,0x40};

void delay(unsigned char i)
{
	uchar j,k;
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
               genggai=1;
			   wei=0;
               break;

          case 0x7b:
		  	   if(allow)
               ok=1;
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
		  	   retry=1;
               break;

          case 0xd7:
		  	   close=1;
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

void shumima()		//对按键采集来的数据进行分配
{
	if(!wanbi)
	{
	switch(wei)
	{
		case 1:new1=key; 
			    if(!allow)	a=17;
			   else a=key;	break;
		case 2:new2=key;
				if(a==17) b=17;
				else b=key;	break;
		case 3:new3=key; 
				if(a==17) c=17;
				else c=key;	break;
		case 4:new4=key;
				if(a==17) d=17;
				else d=key;	break;
		case 5:new5=key; 
				if(a==17) e=17;
				else e=key;	break;
		case 6:new6=key; 
				if(a==17) f=17;
				else f=key;
				wanbi=1;	break;
	}
	}
}

void yanzheng()	  //验证密码是否正确
{
	if(wanbi)	 //只有当六位密码均输入完毕后方进行验证
	{
	if((new1==old1)&(new2==old2)&(new3==old3)&(new4==old4)&(new5==old5)&(new6==old6))
		allow=1;	//当输入的密码正确，会得到allowe置一
	}
}

void main()
{
	while(1)
	{
		keyscan();
		shumima();
		yanzheng();
		if(allow)	 //验证完后，若allow为1，则开锁
		{
			P1=0x00;
			if(!genggai)
				wanbi=0;
		}
		if(genggai)	  //当S16更改密码键被按下，genggai会被置一
		{
			if(allow)	 //若已经把锁打开，才有更改密码的权限
			{
				while(!wanbi)	//当新的六位密码没有设定完，则一直在这里循环
				{
				 	keyscan();
					shumima();
					if(retry|close)	 //而当探测到重试键S18或者关闭密码锁键S19被按下时，则跳出
					{	wanbi=1;
						break;
					}
					display(a,b,c,d,e,f);
				}
			}
		}
		if(ok)	  //更改密码时，当所有六位新密码均被按下时，可以按下此键，结束密码更改
		{		  //其他时间按下此键无效
			ok=0; wei=0;
			genggai=0;
			old1=new1;old2=new2;old3=new3; //此时，旧的密码将被代替
			old4=new4;old5=new5;old6=new6;
			a=16;b=16;c=16;d=16;e=16;f=16;
		}
		if(retry)	//当重试按键S18被按下，retry会被置位
		{
		retry=0; wei=0;wanbi=0;
		a=16;b=16;c=16;d=16;e=16;f=16;
		new1=0;new2=0;new3=0;new4=0;new5=0;new6=0;		
		}
		if(close)  //当关闭密码锁按键被按下，close会被置位
		{
			close=0;genggai=0;//所有变量均被清零。
			wei=0;	wanbi=0;
			allow=0;
			P1=0xff;
			a=16;b=16;c=16;d=16;e=16;f=16;
			new1=0;new2=0;new3=0;new4=0;new5=0;new6=0;
		}
		display(a,b,c,d,e,f); //实时显示
	}
}