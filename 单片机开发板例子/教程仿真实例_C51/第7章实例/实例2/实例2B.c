/*接收程序*/
#include<reg51.h>
#define uchar unsigned char
char code map[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};//'0'~'9'

void main(void){
   	uchar receiv;		    //定义接收缓冲
   	TMOD=0x20;				//T1定时方式2
   	TH1=TL1=0xf4;			//2400b/s
   	PCON=0;					//波特率不加倍
   	SCON=0x50;				//串口方式1,TI和RI清零,允许接收;
	TR1=1;					//启动T1
   	while(1){  
		while(RI==1){		//等待接收完成
   		   RI = 0;			//清RI标志位
   		   receiv = SBUF;	//取得接收值
		   SBUF = receiv;	//结果返送主机
		   while(TI==0);	//等待发送结束
		   TI = 0;		   	//清TI标志位
		   P2 = map[receiv];//显示接收值
   	   }
    }
}



