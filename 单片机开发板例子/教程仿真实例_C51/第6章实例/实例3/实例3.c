#include <reg51.h>
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,
                  0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char count=0;        //计数器赋初值

int0_srv () interrupt 1{	  //T0中断函数
	if(++count==100) count=0; //判断循环是否超限
	P0=table[count/10];	      //显示十位数
	P2=table[count%10];	      //显示个位数
}  

main(){
   P0=P2=table[0];			  //显示初值“00”
   TMOD=0x06;				  //T0计数方式2
   TH0=TL0=0xff;			  //计数初值
   ET0=1;					  //开中断
   EA=1;
   TR0=1;					  //启动T0
   while(1);
} 


