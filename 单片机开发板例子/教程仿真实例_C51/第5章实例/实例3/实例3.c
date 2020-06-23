#include "reg51.h"
char led[]={0xfe,0xfd,0xfb,0xf7};//LED花样数据
bit dir=0,run=0;     			//全局变量
void delay(unsigned int time);	 
key() interrupt 0{				//键控中断函数
	switch (P0 & 0x0f){			//修改标志位状态
		case 0x0e:run=1;break;
		case 0x0d:run=0,dir=0;break;
		case 0x0b:dir=1;break;
		case 0x07:dir=0;break;
}} 
void main(){ 
	char i;
	IT0=1;EX0=1;EA=1;//边沿触发、INT0允许、总中断允许
	while(1){ 
		if (run) 			  
			if(dir)			  //若run=dir=1,自上而下流动
				for(i=0;i<=3;i++){
					P2=led[i];
					delay(200);
				}
		    else			  //若run=1，dir=0,自下而上流动
				for(i=3;i>=0;i--){
					P2=led[i];
					delay(200);
				}
	    else P2=0xff;            //若run=0,灯全灭
}}
void delay(unsigned int time){ 
	unsigned int j = 0;
	for(;time>0;time--)
		for(j=0;j<125;j++);
}