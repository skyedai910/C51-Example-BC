//拧动电位器，会在数码管的前三位显示0-255之间的数值。这就是把模拟电压转换成数字信号。
//有些拧动电位器后数码管数可能不会动态变化，请用户按下实验板上的S22复位按键可更新数据
//这种显象产生的原因是和AD芯片自身的一些特性有关。
#include<reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit adrd=P3^7;                          //IO口定义
sbit adwr=P3^6;
sbit diola=P2^5;
sbit dula=P2^6;
sbit wela=P2^7;
sbit CS=P0^7;
unsigned char j,k,adval;
void delay(unsigned char i)		//延时程序
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,				//数码管编码
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
void display(uchar bai_c,uchar sh_c,uchar g_c)			//显示程序
{  
   P0=table[bai_c];		//显示百位
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

  uchar a,A1,A2,A2t,A3;
    while(1)
  {
    wela=1;
    P0=0;                       //选通ADCS
    adwr=0;			//AD写入（随便写个什么都行，主要是为了启动AD转换）
    _nop_();
    adwr=1;
    P0=0xff;                    //关闭ADCS
    //CS=1;
	delay(10);
    wela=0;                     //关闭有AD片选信号锁存器的锁存端以防止在操作数码管时使AD的片选发生变化
	 adwr=0;

       for(a=20;a>0;a--)        //需要注意的是ADC0804在写和读之间的时间间隔要足够长否则无法读出数据
    {                           //这里把显示部分放这里的原因也是为了增加写读之间的时间间隔                                                                                                                                                                                                                           
        display(A1,A2,A3);
     }	//送去显示各位。
     wela=1;                   //重新打开有AD片选信号锁存器的锁存端
     P1=0xff;                    //读取P1口之前先给其写全1
     //P0=0;                       //选通ADCS
     CS=0;
	 adrd=0;                     //AD读使能
     adval=P1;			//AD数据读取赋给P1口
     adrd=1;
     //P0=0xff;                     //关闭ADCS
     CS=1;
     wela=0;     
	  
	 P1=adval;                    //同时把AD的值送八个发光二极显示
     A1=adval/100;		//分出百，十，和个位
     A2t=adval%100;
     A2=A2t/10;
     A3=A2t%10;
   };
}
