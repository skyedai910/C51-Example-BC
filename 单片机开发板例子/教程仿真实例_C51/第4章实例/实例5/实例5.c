//实例5 LED数码管显示
#include <reg51.h> 					// 51头文件
char led_mod[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; //LED显示字模
void delay(unsigned int time){ 
	unsigned int j = 0;
	for(;time>0;time--)				//采用传入的实参值作为time初值
		for(j=0;j<125;j++);
}
void main() 
{	char i = 0;
	while(1)		  	
	{	for(i=0;i<=9;i++) 
		{	P0=led_mod[i];			//提取字模输出到P0口
			delay(500); 
		}
	} 
}

