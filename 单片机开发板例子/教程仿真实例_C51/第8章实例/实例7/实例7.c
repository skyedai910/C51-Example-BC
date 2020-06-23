#include<absacc.h>
#include<reg51.h>
#define  DAC1  XBYTE[0xfeff]	//设置1#DAC0832输入锁存器的访问地址
#define  DAC2  XBYTE[0xfdff]	//设置2#DAC0832输入锁存器的访问地址
#define  DAOUT XBYTE[0xefff]	//两个DAC0832的DAC寄存器访问地址
void main (void){
     unsigned char num;			//需要转换的数据
     while(1){
		for(num =0; num <=255; num++){
			DAC1 = num;	   		//上锯齿送入1#DAC
			DAC2 = 255-num;		//下锯齿送入2#DAC
        	DAOUT = num;		//两路同时进行D/A转换输出
		}
     }
}
