/*
encoding:utf-8
name:led-simple2
function:
1.上电复位后，全部LED不亮。
2.当按下S2并弹起后，按D1-D7的顺序依次从上到下点亮LED。
3.当按下S3并弹起后，按D7-D1的顺序依次从下到上点亮LED。
4.当按下S4并弹起后，全部灯熄灭；再次当按下S4并弹起后，全部灯点亮。
author : https://www.mrskye.cn
*/
#include<reg51.h>
#include<intrins.h>

sbit LE=P2^5;
unsigned char n;    //全局循环次数
unsigned char flag = 0; //启动标志位

void delay(unsigned int i)  //延迟函数
{
    unsigned int j;
    unsigned char k;
    for(j=i;j>0;j--)
    {
        for(k=125;k>0;k--);
    }
}

void service_int0(void) interrupt 0
{
    flag = 1;
}

void main()
{
    //中断服务初始化
    IE = 0x81;   //开启中断服务
    TCON = 0x1; //设置中断触发方式
    P1=0xff;   //复位关闭全部LED
    while(1)
    {
        if(flag)
        {
            switch (P2&0xf)
            {
                case 0xe:  //S2
                {
                    P1=0xfe;    //D0
                    delay(2500);    //D0亮时间
                    for(n=0;n<7;n++)    //循环8-1次
                    {
                        P1=_crol_(P1,1);//循环左移
                        delay(2500);
                    }                    
                    P1=0xff;   //复位关闭全部LED
                    break;
                }
                case 0xd:  //S3
                {
                    P1=0x7f;    //D7
                    delay(2500);    //D7亮时间
                    for(n=0;n<7;n++)    //循环8-1次
                    {
                        P1=_cror_(P1,1);//循环右移
                        delay(2500);
                    }
                    P1=0xff;   //复位关闭全部LED
                    break;
                }
                case 0xb:  //S4
                {
                    P1=~P1; //取反
                    break;
                }
                /*
                case 0x7:  //S5
                {
                    while(P2&0xf==0x7f); //判断按键状态
                    for(P1=0x0;;P1--)
                    {
                        if(P2==0x7f)break;  //再次按下退出跑马灯
                        delay(1000);
                    }
                    break;
                }
                */
                default: break;
            }
            flag = 0;
        }        
    }
}