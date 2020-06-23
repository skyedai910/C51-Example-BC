#include <reg51.H> 				//51头文件
sbit P3_7=P3^7;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char count;
void delay(unsigned int time) 	//延时
{	unsigned int j = 0;
	for(;time>0;time--)
		for(j=0;j<125;j++);
}
void main()	
{ count=0;                      //计数器赋初值
  P0=table[count/10];           //P0口显示初值
  P2=table[count%10];           //P2口显示初值
  while(1)                     	//进入无限循环
  {	 if(P3_7==0)              	//软件消抖,检测按键是否压下           
  	 {	delay(10);      
		if(P3_7==0)          	//若按键压下
		{	count++;          	//计数器增1
          	if(count==100)   	//判断循环是否超限
    			count=0; 
			P0=table[count/10];	//P0口输出显示
			P2=table[count%10];	//P2口输出显示
			while(P3_7==0);    	//等待按键松开，防止连续计数
   		}
	  }
   }
}






