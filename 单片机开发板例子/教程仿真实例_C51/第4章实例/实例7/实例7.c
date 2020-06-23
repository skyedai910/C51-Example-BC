#include <REG51.H>					//51头文件
char led_mod[] = {0x38,0x5B};		//LED字模“L2”
void delay(unsigned int time);	  	//delay函数声明

void main() {
	char led_point = 0;
   	while (1) { 
		P3 = 2 - led_point; 		//输出LED位码
		P2 = led_mod[led_point];	//输出字模	
		led_point = 1 - led_point;	//刷新LED位码
		delay(30);			
    }
}
void delay(unsigned int time){ 
	unsigned int j = 0;
	for(;time>0;time--)
		for(j=0;j<125;j++);
}

