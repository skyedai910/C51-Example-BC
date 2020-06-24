#include <reg52.h>
typedef unsigned char uchar;
sbit en = P3 ^ 4;
sbit rs = P3 ^ 5;
uchar code table1[] =
{
    "SMC1602 0x01..."
};
uchar code table2[] =
{
    "      Hugh Dong."
};
void Delay(uchar m)
{
    uchar i, j;
    for (i = m; i > 0; --i)
        for (j = 110; j > 0; --j);
}
/* 写数据 */
void write_date(uchar date)
{
    //参考写操作时序图
    rs = 1; //RS高电平,写数据
    P2 = date; //写入数据,对应时序图t_SP2数据建立
    Delay(5);
    en = 1; //打开使能,给E一个高脉冲,开始写入数据
    Delay(5);
    en = 0; //关闭使能,写入数据完成
}
/* 写指令 */
void write_com(uchar com)
{
    //基本原理同写数据
    rs = 0; //RS低电平,写指令
    P2 = com;
    Delay(5);
    en = 1;
    Delay(5);
    en = 0;
}
/* 初始化LCD */
void Init()
{
    en = 1;
    write_com(0x38); //00111000 设置16*2显示,5*7点阵,8位数据接口
    write_com(0x0E); //00001110 开显示,显示光标,不闪烁
    write_com(0x06); //00000110 读写字符后地址指针+1,光标+1,屏幕不移动
    write_com(0x01); //清屏
    write_com(0x80 + 0x01); //设置数据地址指针从第一个开始
}
int main()
{
    /* 初始化LCD */
    Init();
    Delay(100);
    /* 初始化中断 */
    EA = 1;
    EX0 = 1;
    EX1 = 1;
    /* 死循环 */
    while(1);
}
void Display()interrupt 0 using 1
{
    uchar i;
    for (i = 0; i < 16; ++i) //遍历逐个写入第一行数据
    {
        write_date(table1[i]);
        Delay(20);
    }
    write_com(0x02); //换行
    write_com(0x80 + 0x40); //数据地址指针指向第二行
    for(i = 0; i < 16; ++i) //逐个遍历写入第二行数据
    {
        write_date(table2[i]);
        Delay(20);
    }
}
void Clear() interrupt 1 using 1
{
    write_com(0x01);
}