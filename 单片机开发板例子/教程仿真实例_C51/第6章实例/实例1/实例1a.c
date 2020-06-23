#include <reg51.h>
sbit P2_0=P2^0;
void main (void) {
   TMOD = 0x01;          //T0定时方式1
   TR0=1;                //启动T0
   for( ; ; ){
      TH0 = 0xfc; 		//装载计数初值
      TL0 = 0x18;
      do{ } while(!TF0); //查询等待TF0复位
      P2_0 =!P2_0;       //定时时间到P1.0反相
      TF0 = 0;           //软件清TF0
	}
}
