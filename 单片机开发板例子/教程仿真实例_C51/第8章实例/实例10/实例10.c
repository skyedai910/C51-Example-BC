#include<reg51.h>
unsigned char delay_par=0x8;  				//闪灯次数初值
unsigned char light_code=0xf7;				//闪灯位置初值

void delay(void){	         				//可控硅触发维持时间
  	unsigned char i=5; 
  	if(i>=0)  i--;		
}

void main(){
  	TCON=0x01;	           					//中断设置为下降沿触发
  	EA=1;					   
  	EX0=1;				  					//开中断
  	while(1);				  
}

void INT0_srv(void) interrupt 0 {
	P1=light_code;							//触发可控硅(闪灯开始) 
	delay();								//延时20us
	P1=0xff;								//关断触发
	delay_par--;							//统计闪灯次数
	if(delay_par==0){						//切换闪灯位置
		switch(light_code){
			case 0xf7:	 light_code=0xfb;    break;
			case 0xfb:	 light_code=0xfd;	 break;
			case 0xfd:	 light_code=0xfe;	 break;
			case 0xfe:	 light_code=0xf7;	 break;
			default: 	 break;
		}
		delay_par=0x8;	 					//重置闪灯次数
	}  
}