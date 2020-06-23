/***************************************************************
*****************************************************************
程序功能：  向IIC总线器件24c02EEPROM中地址为0的存储单元中写入数据0xaa
			并将其读出并送入P1口，可观察到P1口相连的LED交替被点亮。
			主要为了练习IIC总线的驱动程序以及24C02的操作方法。
***************************************************************/
//试验AT24C02EEPROM芯片程序
#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit sda=P2^0;                          //IO口定义
sbit scl=P2^1;

//此为待写入24c02的数据。为了便于验证结果，数组的内容为周期重复的。
char code music[] = {
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,
0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0,0x55,0xaa,0x0f,0xf0
};	 //由于最后还要讲这些数据读出送到P1口，故可发现P1口相连的led有规律地闪烁15个周期

uchar data buffer[100]; //用于缓存从24c02中读取的数据。
				 
delay(unsigned int m)
{
	unsigned int n,p;
	for(n=m;n>0;n--)
		for(p=125;p>0;p--);
}
void nop()
{
	_nop_();
	_nop_();
}
/////////24C02读写驱动程序////////////////////
void delay1(unsigned int m)
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
   delay1(100);
   return(i);
}

//////向24c02的address地址中写入一字节数据info/////
void write24c02(unsigned char address,unsigned char info)
{
   start();
   writebyte(0xa0);
   clock();
   writebyte(address);
   clock();
   writebyte(info);
   clock();
   stop();
   delay1(5000); //这个延时一定要足够长，否则会出错。因为24c02在从sda上取得数据后，还需要一定时间的烧录过程。
}

void main()
{
	uchar add,i,j,m;
	init();        //初始化24C02
	P1=0X55;
	while(add!=sizeof(music))
	{
		write24c02(0x00+add,music[add]);
		add++;
		if(add%4==0)
		P1=~P1;
	}
	P1=0X00;
//到此为止，向24C02中写入数据的过程均已结束。下面的程序为附加的。
//将已写入的数据再读出，送到P1口，通过观察led的亮灭来检验写入的数据是否正确。
	while(m!=add)
	{
		buffer[i]=read24c02(m);
		i++;
		m++;
	}
	
	while(j!=add)
	{
		P1=buffer[j];
		j++;
		delay(1000);
	}
	while(1);
}

