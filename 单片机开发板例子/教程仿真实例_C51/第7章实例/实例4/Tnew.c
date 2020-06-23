//多机通信（主机）程序
#include <reg51.h>
#define uchar unsigned char
#define NODE1_ADDR 1  		//1#子机地址
#define NODE2_ADDR 2		//2#子机地址

uchar KeyValue=0;			//键值
uchar code str[]="0123456789ABCDEF";  //字符集
uchar pointer_1=0,pointer_2=0;		//子机当前发送字符指针

void delay(uchar time){				//延时
	uchar i,j;
	for(i=0;i<130;i++)
		for(j=0;j<time;j++);
} 

void proc_key(uchar node_number){	 //发送程序
	delay(200);
	SCON=0xc0;			//串口方式3、多机通信、允许接收、中断标志清零
	TMOD=0x20;			//T1定时方式2
	TH1=TL1=0xfd;		//9600bps
	TR1=1;				//启动T1		
	TB8=1;	   			//发送地址帧
	SBUF=node_number;
	while(TI==0);		//等待地址帧发送结束
	TI=0;				//清TI标志
	TB8=0;				//准备发送数据帧
	switch(node_number){//切换子机
		case 1: {
			SBUF=str[pointer_1++];	//1#子机字符帧
			if(pointer_1>=16) pointer_1=0;	 //修改发送指针
			break;
		}
		case 2: {
			SBUF=str[pointer_2++];	//2#子机字符帧
			if(pointer_2>=16) pointer_2=0;	 //修改发送指针
			break;
		}
		default: break;
	while(TI==0);		//等待数据帧发送结束
	TI=0;
}}

main(){
	while(1){						
		P1=0xff;
		while(P1==0xff);	  //检测按键
		switch(P1){			  //切换子机
			case 0xfe: proc_key(NODE1_ADDR);break;
			case 0xef: proc_key(NODE2_ADDR);break;
		}
	}
}  


