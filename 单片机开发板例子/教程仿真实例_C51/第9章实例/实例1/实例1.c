#include <REG51.H>
char map[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
sbit lcs = P1^2;	   			//1241引脚定义
sbit sclk = P1^1;
sbit dout = P1^0;
void print(unsigned int value);	//显示函数
void delay();					//延时函数
unsigned int ad();				//AD转换函数
void main() {
   unsigned int value = 0;
   while (1) {
      value  = ad();			//得到AD转换结果
	  print(value);				//显示转换结果
}}
unsigned int ad() {				//AD转换函数
	unsigned int result = 0;
	unsigned char i = 0;
	lcs = 0;			   		//仿照转换时序开始AD转换
	while(dout == 0);			//等待转换结束
	for ( ; i < 12 ; i++) {		//提取转换结果
		sclk = 1;
		delay();
		sclk = 0;
		result <<= 1; 		
		result |= dout;
	}
	sclk = 1;					//发出第13个脉冲
	delay();
	sclk = 0;
	lcs = 1;					//结束AD转换
	return result;				//返回转换结果
}
void print(unsigned int value) {
	char p_buf[4] = "    ";
    char i,pos=0xf7;
	for (i=0; i < 4 ; i++) {   //拆解转换数据
		p_buf[i] = value % 10; //存入显示缓存
		value /= 10;
		if (value == 0) break;
	}
	for (i = 0 ;i < 4 ;i++) {   
	   P2 = P2 | 0x0f;		   //形成段码
	   P2 = P2 & pos;
	   P0 = map[p_buf[i]];	   //显示缓存内容
	   pos = (pos >> 1)| 0x80; //刷新段码
	   delay();
}}
void delay() {
    char i ;
	for (i = 0 ; i < 100 ; i++);
}