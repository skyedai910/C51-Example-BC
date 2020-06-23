#include <reg51.h>
sbit P2_0=P2^0;
void timer0 (void) interrupt 1 {
   P2_0 = !P2_0; 		//P1.0取反
   TH0 = 0xfc; 			//计数初值重装载
   TL0 = 0x18;
}
void main (void) {
   TMOD = 0x01;             //T0定时方式1
   P2_0=0;
   TH0=0xfc;         		//预置计数初值
   TL0=0x18;
   EA=1; 
   ET0=1; 
   TR0=1;
   do { } while (1);
}
