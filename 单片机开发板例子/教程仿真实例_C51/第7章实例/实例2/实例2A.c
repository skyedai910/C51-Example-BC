/*发送程序*/
#include<reg51.h>
#define uchar unsigned char
char code map[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};//'0'~'9'

void delay(unsigned int time){ 
	unsigned int j = 0;
	for(;time>0;time--)
		for(j=0;j<125;j++);
}
void main(void){
   	uchar counter=0;		//定义计数器
   	TMOD=0x20;				//T1定时方式2
   	TH1 = TL1 = 0xf4;		//2400b/s
   	PCON=0;					//波特率不加倍
   	SCON = 0x50;			//串口方式1,TI和RI清零,允许接收;
	TR1=1;					//启动T1   
	while(1){   	
       SBUF = counter;		//发送联络信号
       while(TI==0);		//等待发送完成
       TI = 0;				//清TI标志位
       while(RI==0);		//等待乙机回答
       RI = 0;
	   if(SBUF ==counter){  //若返回值与发送值相同,组织新数据
	      P2 = map[counter];	    //显示已发送值
	      if(++counter>9) counter=0;//修正计数器值
	      delay(500);
		}
	}
}



