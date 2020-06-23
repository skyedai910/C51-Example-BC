/***************************************************************
程序功能：  由于24c02是每个存储地址只对应8个bit的存储空间，即一个
			存储单元所存储的数据最大为255，该例程介绍了一种当所记忆
			数据大于255（而小于65536）的存储方法。即用两个存储单元来
			存储一个数据。
			下载程序后，数码管会显示所设定的num值（此处设为12345）。
***************************************************************/

#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
uint num,num1,num2;        //定义计数值，每过1秒，sec加1
sbit sda=P2^0;                          //IO口定义
sbit scl=P2^1;
sbit dula=P2^6;
sbit wela=P2^7;
sbit s1=P3^4;//计数按键
sbit s2=P3^5;//清零按键
unsigned char j,k;
char a,b,c,d,e;

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
void display(uchar a,b,c,d,e)			//显示程序
{
   dula=0;
   P0=table[a];		//显示万位
   dula=1;
   dula=0;

   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[b];		//显示千位
   dula=1;
   dula=0;

   wela=0;
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[c];		//显示百位
   dula=1;
   dula=0;

   wela=0;
   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[d];		//显示十位
   dula=1;
   dula=0;

   wela=0;
   P0=0xf7;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[e];		//显示个位
   dula=1;
   dula=0;

   wela=0;
   P0=0xef;
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
	num=12345;	   //num为小于等于65535的整数。
	num2=num/256;
	num1=num%256;
	write24c02(0,num1);
	write24c02(1,num2);
	num1=read24c02(0);	//用地址0单元存储num十六进制表示时的低两位
	num2=read24c02(1);	//用地址0单元存储num十六进制表示时的高两位
	num=num2*256+num1;
	a=num/10000;
	b=num%10000/1000;
	c=num%1000/100;
	d=num%100/10;
	e=num%10;
	while(1)
	display(a,b,c,d,e);
}

