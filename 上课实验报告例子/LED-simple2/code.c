/*
encoding:utf-8
name:led-simple2
function:
1.上电复位后，全部LED点亮。
2.当按下S2并弹起后，按D1-D7的顺序依次从上到下点亮LED。
3.当按下S3并弹起后，按D7-D1的顺序依次从下到上点亮LED。
4.当按下S4并弹起后，全部灯熄灭；再次当按下S4并弹起后，全部灯点亮。
5.(自加)当按下S4并弹起后，led进入跑马灯模式，再次按下时退出。
author : https://www.mrskye.cn
*/
#include<reg51.h>
#include<intrins.h>

sbit LE=P2^5;
unsigned char n;    //全局循环次数

void init(void)
{
    P1=0;   //复位点亮全部LED
}

void delay(unsigned int i)  //延迟函数
{
    unsigned int j;
    unsigned char k;
    for(j=i;j>0;j--)
    {
        for(k=125;k>0;k--);
    }
}

void main()
{
    init();
    while(1)
    {
        if(P3!=0xff)
        {
            delay(30);  //消抖
            if(P3!=0xff)
            {
                switch(P3)
                {
                    case 0xef:  //S2
                    {
                        while(P3==0xef); //判断按键状态
                        P1=0xfe;    //D0
                        delay(2500);    //D0亮时间
                        for(n=0;n<7;n++)    //循环8-1次
                        {
                            P1=_crol_(P1,1);
                            delay(2500);
                        }
                        init(); //重置led状态
                        break;
                    }
                    case 0xdf:  //S3
                    {
                        while(P3==0xdf); //判断按键状态
                        P1=0x7f;    //D7
                        delay(2500);    //D7亮时间
                        for(n=0;n<7;n++)    //循环8-1次
                        {
                            P1=_cror_(P1,1);
                            delay(2500);
                        }
                        init(); //重置led状态
                        break;
                    }
                    case 0xbf:  //S4
                    {
                        while(P3==0xbf); //判断按键状态
                        if(P1==0xff||P1==0x00)   //判断led状态避免未能全部关闭（开启）
                        {
                            P1=~P1; //取反
                        }
                        else
                        {
                            init(); //如果led状态全开（关），则默认按下按键重置led
                        }
                        break;
                    }
                    case 0x7f:  //S5
                    {
                        while(P3==0x7f); //判断按键状态
                        for(P1=0x0;;P1--)
                        {
                            if(P3==0x7f)break;  //再次按下退出跑马灯
                            delay(1000);
                        }                        
                        init();
                        break;
                    }
                }
            }
        }
        
    }
}