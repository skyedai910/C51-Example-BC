#include <REG51.H>
#include <math.h>
sbit din = P3^0;   				//定义芯片引脚变量
sbit clk = P3^1;
sbit cs = P3^2;
#define  PI 3.1415

void da(unsigned int value);	

void main() {
	unsigned int num,value;
	while (1) {
		for (num = 180 ; num < 360 ; num++){	  //负半周正弦波形
			value = 2047 + 2047 * sin((float)num / 180 * PI);
			da(value); 
  		}
	}
}

void da(unsigned int v){		 //DA转换
	char i = 11;
	cs = 1;
	cs = 0;						//CS引脚置高电平
	for ( ; i >= 0 ; i--) {
	 	din = (v >> i) & 0x01;	//分解并行数据，串行送入DIN引脚
		clk = 1;				//发生时钟脉冲
		clk = 0;
	}
	cs = 1;	   					//发出第13个脉冲
	cs = 0;						//CS引脚置低电平
}


