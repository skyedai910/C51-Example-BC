	#include <reg51.h> 	
	#define uchar unsigned char	
	bit ldelay=0; 			//长定时溢出标记
	uchar t=0; 				//定时溢出次数

    void main(void) {  	
		uchar code ledp[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 
		uchar ledi; 		//指示显示顺序
		TMOD=0x01;			//定义T0定时方式1
		TH0 =0x3c;TL0 =0xb0;//溢出20次=1秒（12M晶振) 
		TR0=1; 			
		EA=ET0=1; 		
		while(1){		
	  		if(ldelay) {	//发现有时间溢出标记，进入处理
				ldelay=0; 	//清除标记
				P2=ledp[ledi]; //读出一个值送到P2口
				ledi++; 	//指向下一个
				if(ledi==8)ledi=0;//到了最后一个灯就换到第一个
	}}}

	timer0() interrupt 1 {	//T0中断函数
		if(++t==20){t=0; ldelay=1;}//刷新长定时溢出标记
		TH0 =0x3c;TL0 =0xb0; //重置T0初值 
	} 	



