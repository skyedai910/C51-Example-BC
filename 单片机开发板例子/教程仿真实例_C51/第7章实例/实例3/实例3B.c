/*接收程序*/
#include<reg51.h>
#define uchar unsigned char
char code map[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};//'0'~'9'
void main(void){
   uchar receive;		    	//定义接收器
   PCON = 0x80;					//波特率加倍
   SCON = 0x90;					//串口方式2,TI和RI清零,允许接收
   while(1){  
		while(RI==1){			//等待接收完成
			RI = 0;				//清RI标志位
			receive = SBUF;		//取得接收值
			ACC = receive;		//提取奇偶标志位
			if (P == RB8) TB8=0;//将标志位值装入第9位
			  else TB8 = 1;
			SBUF = receive;		//接收的结果返回主机
			while(TI == 0);		//等待发送结束
			TI = 0;		   		//清TI标志位
			P2 = map[receive];  //显示接收值
		}
    }
}
