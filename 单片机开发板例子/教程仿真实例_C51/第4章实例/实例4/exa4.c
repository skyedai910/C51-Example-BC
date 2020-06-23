//实例4 流水灯
#include <reg51.h>	
#include <intrins.h> //包含右循环移位库函数的头文件

void delay(void)	 //定义延时函数
{	unsigned char i,j;
	for(i=1;i<=100;i++)
		for(j=1;j<=200;j++);
}

void main()
{	P2=0x7f;		//P2初值，即01111111B，对应于D8亮其余灭
	delay();		//延时
	while(1){ 		//无限循环
		P2 =_cror_(P2,1); //调用右循环移位库函数将P2右循环1位
		delay();	//延时
	}
}

   
  