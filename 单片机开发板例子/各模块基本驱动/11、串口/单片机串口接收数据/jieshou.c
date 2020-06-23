/*****************************************************************************
程序功能：		用串口调试助手软件通过串口向单片机发送数据。
				当发送的数据为1到8时，分别对应点亮相应的发光二极管。
				发送其他数据，蜂鸣器会响。
****************************************************************************/
#include <reg52.h>
	
#define uchar  unsigned char 
#define uint   unsigned int

uchar buf;

sbit beep=P2^3;

void main(void) 
{
    SCON=0x50;           //设定串口工作方式
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
  switch(buf)
   {
      case 0x31:  P1=0xfe;beep=1;break;  //接受到1，第一个LED亮         
      case 0x32:  P1=0xfd;beep=1;break;  //接受到2，第二个LED亮        
      case 0x33:  P1=0xfb;beep=1;break;  //接受到3，第三个LED亮        
      case 0x34:  P1=0xf7;beep=1;break;  //接受到4，第四个LED亮       
      case 0x35:  P1=0xef;beep=1;break;  //接受到5，第五个LED亮            
      case 0x36:  P1=0xdf;beep=1;break;  //接受到5，第六个LED亮                   
      case 0x37:  P1=0xbf;beep=1;break;  //接受到5，第七个LED亮
	  case 0x38:  P1=0x7f;beep=1;break;  //接受到5，第八个LED亮
	  default:    beep=0;P1=0x00;break;  //接受到其它数据，蜂鸣器响         
   }
   ES = 1;    //允许串口中断
}
