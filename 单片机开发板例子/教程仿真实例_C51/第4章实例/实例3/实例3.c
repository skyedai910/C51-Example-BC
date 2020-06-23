#include "reg51.h"
unsigned char led[]={0xfe,0xfd,0xfb,0xf7};//LED灯的花样数据
void delay(unsigned char time){ 	     //延时函数
	unsigned int j=15000;
	for(;time>0;time--)
		for(;j>0;j--);
} 
void main(){ 
	bit dir=0,run=0;				    //标志位定义及初始化
	char i;
	while(1){ 
		switch (P0 & 0x0f){			 	//读取键值
			case 0x0e:run=1;break;		//K1动作，设run=1
			case 0x0d:run=0,dir=0;break;//K2动作，设run=dir=0
			case 0x0b:dir=1;break;		//K3动作，设dir=1
			case 0x07:dir=0;break;	    //K4动作，设dir=0
		}
		if (run) 						//若run=dir=1, 自上而下流动
			if(dir)
				for(i=0;i<=3;i++){
					P2=led[i];
					delay(200);
				}
		    else						//若run=1,dir=0, 自下而上流动
				for(i=3;i>=0;i--){
					P2=led[i]; 
					delay(200);
				}
	    else P2=0xff; 					//若run=0，灯全灭
	}
}