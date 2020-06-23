/***************************************************************
程序功能：  驱动 AT24C02EEPROM芯片，实现对其的读和写。让其保存计时器的输出sec值。
			并可以保证在系统掉电后，重新加电时，可以读出上次掉电瞬间所保存的值。
演示方法：  下载程序后，数码管开始显示自加数值。片刻后，关闭学习板
			电源，然后打开，会发现数码管上显示的数值是断电瞬间所显
			示的数值。（也即由AT24C02EEPROM芯片所记忆的数值。）
***************************************************************/
//试验AT24C02EEPROM芯片程序
#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
unsigned char sec,num;        //定义计数值，每过1秒，sec加1
unsigned int times;        //定时中断次数
bit  write=0;             //写24C02的标志；
sbit sda=P2^0;                          //IO口定义
sbit scl=P2^1;
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char j,k;

void delay(unsigned char i)		//延时程序
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,				//数码管编码
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void nop()
{
	_nop_();
	_nop_();
}
void display(uchar a,uchar b)			//显示程序
{
   dula=0;
   P0=table[a];		//显示百位
   dula=1;
   dula=0;

   wela=0;
   P0=0x7e;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[b];		//显示十位
   dula=1;
   dula=0;

   wela=0;
   P0=0x7d;
   wela=1;
   wela=0;
   delay(5);

}

/////////24C02读写驱动程序////////////////////
void delay1(unsigned char m)
{	unsigned int n;
  	for(n=0;n<m;n++);
}



void init()  //24c02初始化子程序
{
	scl=1;
	nop();
	sda=1;
	nop();
}

void start()        //启动I2C总线
{
	sda=1;
	nop();
	scl=1;
	nop();
	sda=0;
	nop();
	scl=0;
	nop();
}

void stop()         //停止I2C总线
{
	sda=0;
	nop();
	scl=1;
	nop();
	sda=1;
	nop();
}

void writebyte(unsigned char j)  //写一个字节
{
	unsigned char i,temp;
   	temp=j;
   	for (i=0;i<8;i++)
   {
	   temp=temp<<1;
	   scl=0;
	   nop();
	   sda=CY;		//temp左移时，移出的值放入了CY中
	   nop();
	   scl=1;		//待sda线上的数据稳定后，将scl拉高
	   nop();
   }
   scl=0;
   nop();
   sda=1;
   nop();
}

unsigned char readbyte()   //读一个字节
{
   unsigned char i,j,k=0;
   scl=0; nop(); sda=1;
   for (i=0;i<8;i++)
   {  
		nop(); scl=1; nop();
      	if(sda==1) 
		j=1;
      	else
		j=0;
      	k=(k<<1)|j;
	  	scl=0;
	}
   	nop();
	return(k);
}

void clock()         //I2C总线时钟
{
   unsigned char i=0;
   scl=1;
   nop();
   while((sda==1)&&(i<255))
   	  i++;
   scl=0;
   nop();
}

////////从24c02的地址address中读取一个字节数据/////
unsigned char read24c02(unsigned char address)
{
   unsigned char i;
   start();
   writebyte(0xa0);
   clock();
   writebyte(address);
   clock();
   start();
   writebyte(0xa1);
   clock();
   i=readbyte();
   stop();
   delay1(10);
   return(i);
}

//////向24c02的address地址中写入一字节数据info/////
void write24c02(unsigned char address,unsigned char info)
{
   EA=0;
   start();
   writebyte(0xa0);
   clock();
   writebyte(address);
   clock();
   writebyte(info);
   clock();
   stop();
   EA=1;
   delay1(50);
}

void main()
{
unsigned char i;
TMOD=0x01;  //定时器工作在方式1
ET0=1;
EA=1;
TH0=0x4c; //对TH0 TL0赋值
TL0=0x00; //使定时器0.05秒中断一次
TR0=1;           //开始计时

init();        //初始化24C02
sec=read24c02(1);//每次重新开机时，读出保存的数据赋于sec

while(1)
  {
      i=20;
	  //num=read24c02(1);//读出保存的数据赋于num
      while(i--)
      {
 	  	display(sec/10,sec%10);
      }
	  if(write==1) //判断计时器是否计时一秒
       {
	 	write=0;              //清零
        write24c02(1,sec);  //在24c02的地址1中写入数据sec
	   }
   } 
}

void timer0(void) interrupt 1 using 0  //定时中断服务函数
{
TH0=0x4c; //对TH0 TL0赋值 ,每隔0.05s发出一次中断。
TL0=0x00; //重装计数初值
times++;        //每过250ust tcnt加一
if(times==20)  //计满20次（1秒）时
  {
    times=0;   //重新再计
    sec++;
    write=1;  //1秒写一次24C08
    if(sec==60) //定时100秒，在从零开始计时
      {sec=0;}
  }
}
