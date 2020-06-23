#include "reg51.h"
char led_mod[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,	  //led字模
				  0x7f,0x6f}; //显示字模	  
sbit D1=P3^0;
void delay(unsigned int time){//延时
	unsigned char j=255;	   
	for(;time>0;time--)
		for(;j>0;j--);
} 
key0() interrupt 0 {	      //K0中断函数
	unsigned char i;
	D1=!IE0;					  //IE0状态输出 
	for(i=0;i<=9;i++){    	  //字符0-9循环1圈
		P2=led_mod[i];
		delay(35000);	
	}P2=0x40; 				  //结束符“-” 
}  

key1() interrupt 2 {          //K1中断函数
	unsigned char i;
	for(i=0;i<=9;i++){    	  //字符0-9循环1圈
		D1=!IE0;				  //IE0状态输出
		P1=led_mod[i];
		delay(35000);	
	}P1=0x40; 				  //结束符“-” 
}

void main(){
	unsigned char i;
	TCON = 0x05;			  //脉冲触发方式
 	PX0=0;PX1=1;			  //INT1优先
	D1=1;P1=P2=0x40;		  //输出初值
	IE=0x85;				  //开中断
	while(1){
	    for(i=0;i<=9;i++){    //字符0-9无限循环
		   P0=led_mod[i];
		   delay(35000);	
		}
	     
	}
}