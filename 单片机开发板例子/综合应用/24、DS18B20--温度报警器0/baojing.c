/*****************************************************************************

*******************************************************************************
程序功能：实时采集当前环境温度值，并显示于数码管上。
		  同时，当温度高于某一值时（此处设为31摄氏度），蜂鸣器便会发出报警。
		  而当低于该值时，蜂鸣器自动停止报警。
		  试验时，可用手触摸DS18B20，使其温度升高到31度，观察现象。
*****************************************************************************/
#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit DS=P2^2;           //define interface
uint temp;             // variable of temperature
uchar flag1;            // sign of the result positive or negative
sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};

void delay(uint count)      //delay
{
  uint i;
  while(count)
  {
    i=200;
    while(i>0)
    i--;
    count--;
  }
}
void dsreset(void)       //send reset and initialization command
{
  uint i;
  DS=0;
  i=103;
  while(i>0)i--;
  DS=1;
  i=4;
  while(i>0)i--;
}

bit tmpreadbit(void)       //read a bit
{
   uint i;
   bit dat;
   DS=0;i++;          //i++ for delay
   DS=1;i++;i++;
   dat=DS;
   i=8;while(i>0)i--;
   return (dat);
}

uchar tmpread(void)   //read a byte date
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   //读出的数据最低位在最前面，这样刚好一个字节在DAT里
  }
  return(dat);
}

void tmpwritebyte(uchar dat)   //write a byte to ds18b20
{
  uint i;
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)
  {
    testb=dat&0x01;
    dat=dat>>1;
    if(testb)     //write 1
    {
      DS=0;
      i++;i++;
      DS=1;
      i=8;while(i>0)i--;
    }
    else
    {
      DS=0;       //write 0
      i=8;while(i>0)i--;
      DS=1;
      i++;i++;
    }

  }
}

void tmpchange(void)  //DS18B20 begin change
{
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);  // address all drivers on bus
  tmpwritebyte(0x44);  //  initiates a single temperature conversion
}

uint tmp()               //get the temperature
{
  float tt;
  uchar a,b;
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);
  tmpwritebyte(0xbe);
  a=tmpread();
  b=tmpread();
  temp=b;
  temp<<=8;             //two byte  compose a int variable
  temp=temp|a;
  tt=temp*0.0625;
  temp=tt*10+0.5;
  return temp;
}

void display(uint temp)			//显示程序
{
   uchar A1,A2,A2t,A3;
   A1=temp/100;
   A2t=temp%100;
   A2=A2t/10;
   A3=A2t%10;
   dula=0;
   P0=table[A1];		//显示百位
   dula=1;
   dula=0;

   wela=0;
   P0=0x7e;
   wela=1;
   wela=0;
   delay(1);

   dula=0;
   P0=table1[A2];		//显示十位
   dula=1;
   dula=0;

   wela=0;
   P0=0x7d;
   wela=1;
   wela=0;
   delay(1);

   P0=table[A3];		//显示个位
   dula=1;
   dula=0;

   P0=0x7b;
   wela=1;
   wela=0;
   delay(1);
}


void main()
{
 uchar a;
  do
  {
    tmpchange();
	for(a=10;a>0;a--)
  	{   
  		display(tmp());
  	}
	if(temp>=310)	 //当温度超过31度（仅作试验用，实际可设为其他更高的值），蜂鸣器便会报警。
		{
			P1=0x00;
			beep=0;
		}
		else
		{
			beep=1;
			P1=0xff;
		}
  } while(1);
}
