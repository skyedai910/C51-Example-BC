#include <reg51.h>
sbit P3_0=P3^0;
void main (){
   TMOD = 0x06; 	//设置为T0计数方式2
   TL0 = 0xff;		//初值0xff可使1个外来脉冲即产生溢出
   TR0 = 1;			//启动计数器
   while (1){
     while (!TF0);	//等待首次溢出
     TF0 = 0;		//清TF0溢出标志
     TMOD = 0x02; 	//设置为T0定时方式2
	 TL0=0x06;		//500微秒定时初值
     P3_0 = 0;
     while (!TF0);	//等待再次溢出
     TF0 = 0;		//清TF0溢出标志
     P3_0 = 1;
     TMOD = 0x06;	//设置为T0计数方式2
     TL0 = 0xff;	//1次溢出计数初值
   }
}