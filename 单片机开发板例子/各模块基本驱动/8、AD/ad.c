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
unsigned char j,k,adval;

void delay(unsigned char i)		//延时程序
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}


void main()		//	主程序
{

    while(1)
  {
    wela=1;
    P0=0;                       //选通ADCS
    adwr=0;						//AD写入
    _nop_();
    adwr=1;
    P0=0xff;                    //关闭ADCS
    delay(10);
    wela=0;                     //关闭有AD片选信号锁存器的锁存端以防止在操作数码管时使AD的片选发生变化
    delay(1);				  //500
	        				//需要注意的是ADC0804在写和读之间的时间间隔要足够长否则无法读出数据
     wela=1;                   //重新打开有AD片选信号锁存器的锁存端
     P0=0;                       //选通ADCS
     P1=0xff;                    //读取P1口之前先给其写全1
     adrd=0;                     //AD读使能
     adval=P1;			//AD数据读取赋给P1口
     adrd=1;
     P0=0xff;
	                      
     adwr=0;			   //关闭ADCS
     diola=1;			   //打开LED
	 P1=adval;             //同时把AD的值送八个发光二极显示
   }
}
