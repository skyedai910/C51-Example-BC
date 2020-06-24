/*
encoding:utf-8
name:89c51四音电子琴
author:https://www.mrskye.cn
*/
#include<reg51.h>
#include<intrins.h>

//sbit button0 = P3^4;
//sbit button1 = P3^5;
//sbit button2 = P3^6;
//sbit button3 = P3^7;
sbit buzz = P2^3;
unsigned char m;    //持续时间变量

/*
void delay_buzz()
{
    unsigned int a;
    for(a=55;a>0;a--)
    {
        _nop_();
    }
}
*/

void delay(unsigned int i)  //消抖延迟函数
{
    unsigned int j;
    unsigned char k;
    for(j=i;j>0;j--)
    {
        for(k=125;k>0;k--);
    }
}

void delay130us(void)   //do:误差 -0.055500567537us
{
    unsigned char a,b;
    for(b=13;b>0;b--)
        for(a=3;a>0;a--);
}

void delay146us(void)   //re:误差 -0.820108967083us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=64;a>0;a--);
}

void delay164us(void)   //mi:误差 -0.277704880817us
{
    unsigned char a;
    for(a=74;a>0;a--);
}

void delay174us(void)   //fa:误差 -0.271167423383us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=77;a>0;a--);
}

void main() //switch 改造升级
{
    while(1)
    {
        if(P3 != 0xff)
        {
            delay(30);  //消抖
            if(P3 != 0xff)
            {
                switch(P3)
                {
                    case 0xef:
                    {
                        while(P3==0xef)
                        {
                            for(m=3823;m>0;m--)//0.130816ms*3823=0.5s
                            {
                                buzz =~buzz;
                                delay146us();
                            }
                        }
                        break;
                    }
                    case 0xdf:
                    {
                        while(P3==0xdf)
                        {
                            for(m=3405;m>0;m--)//0.146836ms*3405=0.5s
                            {
                                buzz =~buzz;
                                delay130us();
                            }
                        }
                        break;
                    }
                    case 0xbf:
                    {
                        while(P3==0xbf)
                        {
                            for(m=3037;m>0;m--)//0.164818ms*3037=0.5s
                            {
                                buzz =~buzz;
                                delay164us();
                            }
                        }
                        break;
                    }
                    case 0x7f:
                    {
                        while(P3==0x7f)
                        {
                            for(m=2863;m>0;m--)//0.1746185ms*2863=0.5s
                            {
                                buzz =~buzz;
                                delay174us();
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}

/*
void main()
{
    while(1)
    {
        if(!button0)    //do
        {
            delay(30);  //消抖
            if(!button0)
            {
                while(!button0)
                {
                    for(m=3823;m>0;m--)//0.130816ms*3823=0.5s
                    {
                    buzz =~buzz;
                    delay146us();
                    }
                }
            }
        }
        if(!button1)    //re
        {
            delay(30);  //消抖
            if(!button1)
            {
                while(!button1)
                {
                    for(m=3405;m>0;m--)//0.146836ms*3405=0.5s
                    {
                    buzz =~buzz;
                    delay130us();
                    }
                }
            }
        }
        if(!button2)    //mi
        {
            delay(30);  //消抖
            if(!button2)
            {
                while(!button2)
                {
                    for(m=3037;m>0;m--)//0.164818ms*3037=0.5s
                    {
                    buzz =~buzz;
                    delay164us();
                    }
                }
            }
        }
        if(!button3)    //fa
        {
            delay(30);  //消抖
            if(!button3)
            {
                while(!button3)
                {
                    for(m=2863;m>0;m--)//0.1746185ms*2863=0.5s
                    {
                    buzz =~buzz;
                    delay174us();
                    }
                }
            }
        }
    }
}
*/