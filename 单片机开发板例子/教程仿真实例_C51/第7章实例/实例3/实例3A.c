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
   	uchar counter=0;	 	//定义计数器
   	PCON = 0x80;			//波特率加倍
   	SCON = 0x90;			//方式2,SM2=TI=RI=0,允许接收
   	while(1){   				
		ACC = counter;		//提取奇偶标志位值
		TB8 = P;			//组装奇偶标志
       	SBUF = counter;		//发送数据
       	while(TI==0);		//等待发送完成
       	TI = 0;				//清TI标志位
       	while(RI==0);		//等待乙机回答
       	RI = 0;
		if(RB8 ==0){		//判断RB8=0？
			P2 = map[counter];	//若为0，则显示已发送值
			if(++counter>9) counter=0;//刷新发送数据
			delay(500);		//调整程序节奏
		}
	}
}



