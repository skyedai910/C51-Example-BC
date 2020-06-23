/*****************************************************************
程序功能：  向IIC总线器件24c02EEPROM中地址为3的存储单元中写入数据34
			并将其读出，显示于数码管上。
			主要为了练习IIC总线的驱动程序以及24C02的操作方法。
***************************************************************/
//试验AT24C02EEPROM芯片程序
#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
unsigned char num;        //定义计数值，每过1秒，sec加1
sbit sda=P2^0;                          //IO口定义
sbit scl=P2^1;
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char j,k;
char a,b;

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

	init();        //初始化24C02
	write24c02(3,34);//向24c02中的地址为3的存储单元写入十进制数字34
	num=read24c02(3);//读出保存的数据赋于num
	a=num/10;
	b=num%10;
	while(1)
	{
		display(a,b);
	}
}

