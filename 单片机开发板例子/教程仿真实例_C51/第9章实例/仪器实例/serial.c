#include<reg51.h>
void serial(char byte) 
{
	SCON = 0;			//串口方式0
	SBUF = byte; 	//输出数据送入缓冲区
	while(TI!=1);	//等待移位结束
	TI=0;	   			//清理标志位
}
