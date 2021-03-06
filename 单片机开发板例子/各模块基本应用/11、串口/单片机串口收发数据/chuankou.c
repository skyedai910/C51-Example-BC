/*****************************************************************
 程序功能：	 计算机通过串口向单片机发送数据，单片机接受后会将接受到
 			 的数据重新发送给计算机。
			 本程序需用到串口调试助手软件，下载程序后，在发送区发送
			 数据后，会在上面的接受区显示单片机返回来的数据。
*****************************************************************/

#include <reg52.h>
	
#define uchar  unsigned char 
#define uint   unsigned int

uchar buf;

void senddata(uchar dat)
{
     SBUF =dat;
     while(!TI);
     TI = 0;
}

void main(void) 
{
    SCON=0x50;           //设定串口工作方式1
    PCON=0x00;           //波特率不倍增
			
    TMOD=0x20;           //定时器1工作于8位自动重载模式, 用于产生波特率
    EA=1;
    ES = 1;              //允许串口中断
    TL1=0xfd;
    TH1=0xfd;             //波特率9600
    TR1=1;
    while(1);        
}
/*********************************************************

  串行中断服务函数

*********************************************************/
void  serial() interrupt 4 
{
   ES = 0;                //关闭串行中断
   RI = 0;                //清除串行接受标志位
   buf = SBUF;            //从串口缓冲区取得数据
   senddata(buf);  
   ES = 1;    //允许串口中断
}
