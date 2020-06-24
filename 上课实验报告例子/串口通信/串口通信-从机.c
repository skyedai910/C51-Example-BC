/*
proj:多机通信（1#子机）
desc:基于80c51通过串口实现多机通信，从机具备收发功能。通过定时器中断服务
     函数先接收信息，后调用发送函数返还接收信息给主机
author:mrskye
website:https://www.mrskye.cn/
encoding:utf-8
*/

#include <reg51.h>
#define NODE0_ADDR 0
#define NODE1_ADDR 1
#define uchar unsigned char

//测试引脚
sbit P3_7=P3^7;	
//delay全局变量
uchar i,j;	
//7led显示字形码(阳极)
uchar code table[16]={0x88,0x83,0xc6,0xa1,0x86,0x8e,0x00,0x89,0x00,0x00,0x00,0xc7,0x00,0x00,0xc0};


void delay(unsigned int i)  	//可控时间延迟函数
{
    unsigned int j;
    unsigned char k;
    for(j=i;j>0;j--)
    {
        for(k=125;k>0;k--);
    }
}

void display(uchar ch)
{
    if((ch>=65)&&(ch<=79)) P2=table[ch-65];
}		

void proc_key(uchar node_number,uchar str)//发送程序
{	 
    delay(200);	
    switch(node_number)//切换主机
    {
        case 0: 
        {
            uchar pointer_0;	//主机当前发送字符指针
            TB8=1;	   			//发送地址帧
            SBUF=node_number;
            while(TI==0);		//等待地址帧发送结束
            TI=0;				//清TI标志
            TB8=0;				//准备发送数据帧
            SBUF=str;			//0#主机字符帧
            while(TI==0);		//等待地址帧发送结束
            TI=0;				//清TI标志
        }
        default: break;
    }
}

void main()
{
    SCON=0xd0;				//串口方式3、多机通信、允许接收、中断标志清零
    TMOD=0x20;				//T1定时方式2
    TH1=TL1=0xfd;			//9600bps
    TR1=1;					//启动T1
    ES=1;EA=1;				//开中断
    while(1);
}

void receive(void) interrupt 4 
{
    uchar message;
    RI=0; 
    if(RB8==1)
    {
        if(SBUF==NODE1_ADDR)
        {
            SM2=0;
            P3_7=!P3_7;
        }
        return;
    }
    message = SBUF;
    display(SBUF);			//显示接收字符
    SM2=1;
    proc_key(NODE0_ADDR,message);//返回消息给主机
}
