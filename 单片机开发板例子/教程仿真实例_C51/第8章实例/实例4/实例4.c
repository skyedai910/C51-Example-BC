		  #include<reg51.h>

unsigned char xdata CONW _at_ 0x7fff;	//控制字
unsigned char xdata PORTA _at_ 0x7cff;	//A口定义
unsigned char xdata PORTB _at_ 0x7dff;	//B口定义

void main( ){
	CONW=0x82;						//控制字寄存器赋值
	while(1){
		PORTA=PORTB;				//将B口读入值送入A口写出了
	}
}