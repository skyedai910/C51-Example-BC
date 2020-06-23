#include<reg51.h>
void delay() {	                        //延时
    unsigned int i;
    for (i=0; i<20000; i++) {}
}

void main() { 
    unsigned char index, LED;            //定义LED指针和显示字模
    SCON = 0;           	             //设置串行模块工作在方式0
    while (1) {      
	   LED=0x7f;
	   for (index=0; index < 8; index++) {
	        SBUF = LED;                  //控制L0灯点亮
	        do {} while(!TI);            //通过TI查询判别数据是否输出结束
            LED = ((LED>>1)|0x80);       //左移1位，末位置1
			TI=0;
			delay();  
 	   }
    }
}
