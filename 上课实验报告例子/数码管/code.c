/*
proj:dp-led
author:mrskye
encoding:gbk
*/
#include<reg52.h>

sbit DULA = P2^6;
sbit WELA = P2^7;

unsigned char wordlist[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};
unsigned char ledlist[] = {0xfe,0xfd,0xfb,0xf7,0xef,0x5f};

void delay(unsigned int i)
{
    unsigned int m,n;
    for(m=i;m>0;m--)
		for(n=90;n>0;n--);
}

void display(unsigned char word,unsigned char led,unsigned int time)
{
    //选中显示位
    P0 = led;
    WELA = 1;
    WELA = 0;
    //输出字符
    P0 = word;
    DULA = 1;
    DULA = 0;
    //延迟时间
    delay(time);
}

void main()
{
    while (1)
    {
        display(wordlist[14],ledlist[0],10);
        display(wordlist[6],ledlist[1],10);
        display(wordlist[0],ledlist[2],10);
        display(wordlist[1],ledlist[3],10);
        display(wordlist[3],ledlist[4],10);
        display(wordlist[2],ledlist[5],10);
    }
    
}