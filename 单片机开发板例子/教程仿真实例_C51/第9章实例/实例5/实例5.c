#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
sbit DS1302_SCLK = P3^6;					//1302引脚位变量定义
sbit DS1302_IO = P3^7;
sbit DS1302_RST = P3^5;
sbit LM1602_EN = P2^2;				   		//1602引脚位变量定义
sbit LM1602_RW = P2^1;
sbit LM1602_RS = P2^0;
sbit ACC_7 = ACC^7;						   	//ACC位变量定义
sbit ACC_0 = ACC^0;
uchar second,minute,hour,week,day,month,year;
uchar table[]="0123456789";   				//定义数字显示字符
uchar table1[]="Time:  ";
uchar table2[]="Date: ";
uchar t1302[]={0x14,0x7,0x17,0x04,0x21,0x30,0x00}; //1302初值:年,月,日,星期,时,分,秒

void delay(uint x){             			//延时函数
	uint i;
	for(i=x;i>0;i--);
}

uchar read_ds1302(uchar addr){		   		//DS1302读数据函数
   	uchar i;
   	DS1302_RST=0;
   	DS1302_RST=1;		  					//开放1302使能
	ACC = addr;								//ACC中装入待发地址
	for(i=8;i>0;i--){	  					
		DS1302_IO = ACC_0;					//最低位数据由端口输出
		ACC >>= 1;							//整体右移1位
		DS1302_SCLK = 0;					//时钟线拉低
		DS1302_SCLK = 1;	 				//时钟线拉高
	}
   	for (i=8; i>0; i-- ){
		ACC_7=DS1302_IO;					//位数据移入最高位
		ACC >>= 1;							//整体右移1位
		DS1302_SCLK = 1;					//时钟线拉高
		DS1302_SCLK = 0;					//时钟线拉低
	}
	DS1302_RST=0; 							//关闭1302使能	
	return(ACC);
}

void write_ds1302(uchar addr, uchar dat){ 	//DS1302写数据函数
	uchar i;
	DS1302_RST=0;
 	DS1302_RST=1;
	ACC = addr;								//ACC中装入待发地址
	for(i=8;i>0;i--){	  			  		//发送地址
		DS1302_IO = ACC_0;					//最低位数据由端口输出
		ACC >>= 1;							//整体右移1位
		DS1302_SCLK = 0;					//时钟线拉低
		DS1302_SCLK = 1;	 				//时钟线拉高
	}
	ACC = dat;								//ACC中装入待发数据
	for(i=8;i>0;i--){	         	  		
		DS1302_IO = ACC_0;					//最低位数据由端口输出
		ACC >>= 1;							//整体右移1位
		DS1302_SCLK = 0;					//时钟线拉低
		DS1302_SCLK = 1;	 				//时钟线拉高
	}
 	DS1302_RST=0;							//关闭1302使能
}

void read_1302time() {						//读取DS1302信息
	second = read_ds1302(0x81);	  			//读秒寄存器
	minute = read_ds1302(0x83);	  			//读分寄存器
	hour = read_ds1302(0x85);	  			//读时寄存器
	//week = read_ds1302(0x8b);	  			//读星期寄存器
	month = read_ds1302(0x89);	  			//读月寄存器
	day = read_ds1302(0x87);	  			//读日寄存器
	year = read_ds1302(0x8d);	  			//读年寄存器

}

void write_1602com(uchar com){ 	 	  		//LM1602写指令函数
	P0 = com;	   							//送出指令
	LM1602_RS=0; LM1602_RW=0; LM1602_EN=1;	//写指令时序
	delay(100);
	LM1602_EN=0; 
}
 
void write_1602dat(uchar dat){ 				//LM1602读数据函数
	P0 = dat;								//送出数据
	LM1602_RS=1; LM1602_RW=0; LM1602_EN=1;	//写数据时序
	delay(100);
	LM1602_EN=0;
}

void init_1302(){							//DS1302的初始化
	write_ds1302(0x8e,0x00);				//开写保护寄存器
	write_ds1302(0x8c,t1302[0]);			//年
	write_ds1302(0x88,t1302[1]);			//月
	write_ds1302(0x86,t1302[2]);			//日
	write_ds1302(0x8a,t1302[3]);			//星期
	write_ds1302(0x84,t1302[4]);			//时
	write_ds1302(0x82,t1302[5]);			//分
	write_ds1302(0x80,t1302[6]);			//秒
	write_ds1302(0x8e,0x80);				//锁写保护寄存器
}

void init_1602(){   						//1602初始化
	write_1602com(0x38); 					//设置16*2显示，5*7点阵
	write_1602com(0x0c); 					//开显示，但不显示光标
	write_1602com(0x06); 					//地址加1，写数据时光标右移1位
}

void display1602(void){			  			//1602显示函数
    uchar i;
	write_1602com(0x80);					//第1行信息
	for(i=0;i<6;i++) write_1602dat(table1[i]); 	//显示字符“Time：”
    write_1602dat(table[(hour/16)]);		//显示时、分、秒信息
    write_1602dat(table[(hour%16)]);
    write_1602dat(':');
    write_1602dat(table[minute/16]);
    write_1602dat(table[minute%16]);
    write_1602dat(':');
    write_1602dat(table[second/16]);        
    write_1602dat(table[second%16]);

    write_1602com(0x80+0x40);				//第2行信息
    for(i=0;i<6;i++) write_1602dat(table2[i]);	//显示字符“Date：”
    write_1602dat(table[day/16]);   		//显示日、月、年信息
    write_1602dat(table[day%16]);
    write_1602dat('-');
    write_1602dat(table[month/16]);
    write_1602dat(table[month%16]);
    write_1602dat('-');
    write_1602dat(table[(year/16)]);
    write_1602dat(table[(year%16)]);
}

int main(void){
	init_1302();							//初始化1302
	init_1602();				 			//初始化1602
    while (1){
       read_1302time();			 			//读1302日历时钟信息
       display1602();				 		//显示日历时钟信息
	}
}

