#include <reg51.H>
#define E2PROM_ADDR 0x12			//定义EPROM地址
sbit P3_7=P3^7;						//定义计数器端口
unsigned char count =0;				//定义计数器
unsigned char code table[]=	{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
//声明e2prom的读写函数
void read_e2prom(unsigned char rom_addr, unsigned char ram_addr, unsigned char size);
void write_e2prom(unsigned char rom_addr, unsigned char ram_addr, unsigned char size);

void main(void)	{
	//从EPROM中读取计数保存值
	read_e2prom(E2PROM_ADDR, (unsigned char)&count, 1); 
	P0=table[count/10];				//显示count的十位
	P2=table[count%10];				//显示count的个位
  	while(1) {             		 
      	if(P3_7==0){      			//检测按键是否压下   
			count++;          		//计数器增1
			if(count==100) count=0; //判断循环是否超限
			P0=table[count/10];		//十位输出显示
			P2=table[count%10];		//个位输出显示
			//将当前计数值存入EPROM
			write_e2prom(E2PROM_ADDR,(unsigned char)&count,1); 
			while(P3_7==0);    		//等待按键抬起，防止连续计数
	   }
    }
}

