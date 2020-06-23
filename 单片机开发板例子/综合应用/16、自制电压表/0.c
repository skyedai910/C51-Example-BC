/****************************************************************************

*****************************************************************************
程序功能：	制作一量程为5V的电压表，且显示与数码管上。
			本电压表测量的是位于ADC与DAC芯片之间的滑动变阻器之间的电压，拧动
			滑动变阻器，可以发现数码管显示的电压值变化。
****************************************************************************/
#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit adrd=P3^7;                          //IO口定义
sbit adwr=P3^6;
sbit diola=P2^5;
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char j,k,adval,zheng,xiao1,xiao2,xiao3;
unsigned int value0;
float value;
void delay(unsigned char i)		//延时程序
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,				//数码管编码
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void display(uchar bai_c,uchar sh_c,uchar g_c)			//显示程序
{  
   P0=table[bai_c]|0x80;		//显示百位
   dula=1;
   dula=0;

   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);

   dula=0;
   P0=table[sh_c];		//显示十位
   dula=1;
   dula=0;

   wela=0;
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   P0=table[g_c];		//显示个位
   dula=1;
   dula=0;

   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);
}

void main()		//	主程序
{
	uchar a;
    while(1)
  {
    wela=1;
    P0=0;                       //选通ADCS
    adwr=0;			//AD写入（随便写个什么都行，主要是为了启动AD转换）
    _nop_();
    adwr=1;
    P0=0xff;                    //关闭ADCS
    delay(10);
    wela=0;                     //关闭有AD片选信号锁存器的锁存端以防止在操作数码管时使AD的片选发生变化

     wela=1;                   //重新打开有AD片选信号锁存器的锁存端
     P1=0xff;                    //读取P1口之前先给其写全1
     P0=0;                       //选通ADCS
     adrd=0;                     //AD读使能
     adval=P1;			//AD数据读取赋给P1口
     adrd=1;
    P0=0xff;                     //关闭ADCS
     adwr=0;
     P1=adval;                    //同时把AD的值送八个发光二极显示
	value=adval*5.0/255.0;		  //转换。

	value0=(unsigned int)(value*1000);//强制类型转换，而后取出各位的值。
	zheng=value0/1000;
	xiao1=value0/100%10;
	xiao2=value0/10%10;
	xiao3=value0%10;   //四舍五入的处理
	if(xiao3>=5)
		xiao2++;
	
	 for(a=20;a>0;a--)        //需要注意的是ADC0804在写和读之间的时间间隔要足够长否则无法读出数据
    {                           //这里把显示部分放这里的原因也是为了增加写读之间的时间间隔                                                                                                                                                                                                                           
    	display(zheng,xiao1,xiao2);
     }	//送去显示各位。
   }
}
