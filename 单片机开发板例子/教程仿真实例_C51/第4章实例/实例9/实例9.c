#include <reg51.h>		//51头文件
sbit lock=P3^0;	   		//定义端口变量
char key_buf[] = {0xee, 0xde, 0xbe, 0x7e,0xed, 0xdd, 0xbd, 0x7d,//定义键值
				  0xeb, 0xdb, 0xbb, 0x7b,0xe7, 0xd7, 0xb7, 0x77};
unsigned char init=0x7f,on=0x73,off=0x79,lock_on=0,lock_off=1; //定义操作变量
char getKey(void) 		//定义读键值函数
{	char key_scan[] = {0xef, 0xdf, 0xbf, 0x7f};	//键扫描码
	char i = 0, j = 0;
	for (i = 0; i < 4 ; i++) 
	{	P2 = key_scan[i];  						//P2送出键扫描码
		if ((P2 & 0x0f) != 0x0f)				//判断有无键闭合
			for(j=0;j<16;j++) if(key_buf[j]==P2) return j; //查找闭合键键号
	}
	return -1;				//无键闭合返回-1
}
void delay(void)	 		//定义延时函数
{	unsigned int i,j;
	for(i=1;i<=400;i++)
		for(j=1;j<=200;j++);
}

void action(char stat,char num)	//定义操控函数
{	unsigned char i;
	lock=num; 		//定义开锁状态变量
	for(i=1;i<=2;i++) //显示字符闪烁控制
	{	P0=stat;	
		delay();
		P0=0x0;
		delay(); 
	}
	P0=0x40; 		//显示待机字符“-”
	lock=1;			//上锁
}
void main(void)					//主函数 
{	char key = 0;				//键值初始值
	action(init,lock_off);		//初始化
	while(1){					//无限循环
		key = getKey();			//获得闭合键号
		if (key != -1){
		  	if (key != 7) action(off,lock_off); //密码不符，上锁
			else action(on,lock_on);	//密码符合，先开锁再上锁
		}
	}
}

