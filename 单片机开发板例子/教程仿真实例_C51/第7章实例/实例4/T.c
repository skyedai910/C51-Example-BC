//多机通信（主机）程序
#include <reg51.h>
#define uchar unsigned char
#define NODE1_ADDR 1
#define NODE2_ADDR 2

bit flag=0;
bit flag_send=0;
uchar KeyValue=0;		//键值
uchar code str[]="0123456789ABCDEF";
uchar pointer_1=0,pointer_2=0;

void delay(uchar time){
	uchar i,j;
	for(i=0;i<200;i++)
		for(j=0;j<time;j++);
} 

void proc_key(uchar node_number){
	delay(200);
	SCON=0xc0;			//初始化串行口和波特率发生器
	TMOD=0x20;
	TH1=TL1=0xfd;
	TR1=1;		//打开T1		
	ES=1;EA=1;	//允许串行口中断
	TB8=1;	   	//发送地址帧
	SBUF=node_number;
	while(flag_send==0);//发送完毕
	ES=0;EA=0;			//关中断
	flag_send=0;
	KeyValue=0;
}

main(){
	while(1){
		P1=0xff;
		while(P1==0xff);
		KeyValue=P1;
		switch(KeyValue){
			case 0xfe: proc_key(NODE1_ADDR);break;
 			case 0xef: proc_key(NODE2_ADDR);break;
			default: break;
}}}  

void send(void) interrupt 4 {	//发送数据帧
	TI=0;
	if(flag==1){
		flag_send=1;
		flag=0;
		return;
	}
	else{
		TB8=0;
		switch(KeyValue){
			case 0xfe: {
				SBUF=str[++pointer_1];
				if(pointer_1>=16) pointer_1=0;
				break;
			}
			case 0xef: {
				SBUF=str[++pointer_2];
				if(pointer_2>=16) pointer_2=0;
				break;
			}
			default: break;
		}
		flag=1;
}}
