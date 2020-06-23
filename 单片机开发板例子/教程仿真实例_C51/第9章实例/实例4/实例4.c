#include<reg52.h> 	
unsigned char code table[]="MicroController";  //要显示的内容
unsigned char code table1[]="Proteus /Keil C";
sbit RS=P2^0; 				//寄存器选择引脚
sbit RW=P2^1; 				//读写引脚
sbit EN=P2^2;  				//片选引脚
void delay(unsigned int x){ //延时
	unsigned int i;
	for(i=x;i>0;i--);
}
void write_com(unsigned char com){ 	//写指令函数  
	P0 = com;	   			//送出指令
	RS=0;RW=0;EN=1;			//写指令时序
	delay(200);
	EN=0; 
}
void write_dat(unsigned char dat){ 	//写数据函数
	P0 = dat;				//送出数据
	RS=1;RW=0;EN=1;		 	//写数据时序
	delay(200);
	EN=0;
}
void init(){   				//初始化
	write_com(0x01); 		//清屏
	write_com(0x38); 		//设置16*2显示，5*7点阵
	write_com(0x0f); 		//开显示，显示光标且闪烁
	write_com(0x06); 		//地址加1，写入数据时光标右移1位
}
void main(){
	unsigned char i;   
	init();
	write_com(0x80); 		//起点为第一行第一个字符
	for(i=0;i<15;i++){    	//显示第一行字符
		write_dat(table[i]);
		delay(3000);    	//调节配合速度
	}
	write_com(0xC0);  		//起点为第二行第一个字符
	for(i=0;i<15;i++){	  	//显示第二行字符
		write_dat(table1[i]);
		delay(3000);
	}
	write_com(0x02);  		//光标复位
	while(1);
}