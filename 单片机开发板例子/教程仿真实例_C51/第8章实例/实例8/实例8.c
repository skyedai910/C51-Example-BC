#include <reg51.h>
#include <absacc.h>        
#define	AD_IN7	XBYTE[0xfeff]	//IN7通道地址
sbit ad_busy=P3^3;              //定义检测单元变量

void main(void){
     while(1){
       	AD_IN7 = 0;  			//启动A/D信号                
		while(ad_busy==1); 		//等待A/D转换结束
		P1 = AD_IN7;			//转换数据显示
	}
}
