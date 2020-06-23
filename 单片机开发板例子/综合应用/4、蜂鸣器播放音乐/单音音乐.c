#include <at89x52.h>
#define uchar unsigned char
bit flag; //标志音乐输出脚电平的高低
uchar ptr = 0x00; //取音符
uchar high; //计数器高位
uchar low; //计数器低位
// 本曲谱为 "新年好",前两个十六进制表示发声频率,后一个表示发声时间,0xFF,0xFF 表示休止符
// 0x00 表示结束
// 1 _ 1_ 1 .5
uchar code music[] = {
0xFC,0x44,0x7F, 0xFC,0x44,0x7F, 0xFC,0x44,0xFF, 0xFA,0x68,0xFF,
// 3 _ 3_ 3 1
0xFD,0x23,0x7F, 0xFD,0x23,0x7F, 0xFD,0x23,0xFF, 0xFC,0x44,0xFF,
// 1_ 3_ 5 5
0xFC,0x44,0x7F, 0xFD,0x23,0x7F, 0xFD,0x82,0xFF, 0xFD,0x82,0xFF,
// 4_ 3_ 2 -
0xFD,0x23,0x7F, 0xFD,0x23,0x7F, 0xFC,0xAC,0xFF, 0xFF,0xFF,0xFF,
// 2_ 3_ 4 4
0xFC,0xAC,0x7F, 0xFD,0x23,0x7F, 0xFD,0x34,0xFF, 0xFD,0x34,0xFF,
// 3_ 2_ 3 1
0xFD,0x23,0x7F, 0xFC,0xAC,0x7F, 0xFD,0x23,0xFF, 0xFC,0x44,0xFF,
// 1_ 3_ 2 .5
0xFC,0x44,0x7F, 0xFD,0x23,0x7F, 0xFC,0xAC,0xFF, 0xFA,0x68,0xFF,
 // .7_ 2_ 1 -
0xFC,0x0C,0x7F, 0xFC,0xAC,0x7F, 0xFC,0x44,0xFF, 0xFF,0xFF,0xFF,
0x00//结束
};

void Init(void); //初始化函数
void DelayMs(unsigned int time); //毫秒级延时函数

void main()
{
	uchar time;
	Init();
	TH0 = high;
	TL0 = low;
	while (1)
	{
		if (music[ptr] != 0xFF && music[ptr] != 0x00)//判断是否是正常音符
		{
			TR0 = 0;
			P2_3 = 1;
			DelayMs(10); //间歇
			TR0 = 1;
			high = music[ptr]; //取设置频率数值的高8 位
			low = music[ptr + 1]; //取设置频率数值的低8 位
			time = music[ptr + 2]; //取发声时间
			DelayMs(time);
			ptr += 3;
		}
		else if (music[ptr] == 0xFF) //判断是否是休止符
		{
			time = music[ptr + 2];
			DelayMs(time);
			ptr += 3;
		}
		
		else //结束符,停止2 秒后继续
		{
			TR0 = 0;
			P2_3 = 1;
			DelayMs(2000);
			ptr = 0;
		}
	}
}
/*********************************************************************************
* 名称：Count1(void) interrupt 1
* 功能：设置计时器0 溢出中断,每中断一次改变P2_3 引脚电平
*********************************************************************************/
void Count1(void) interrupt 1
{
	TH0 = high;
	TL0 = low;
	if (flag == 0) //改变P2_3 引脚电平
	{
		P2_3 = 0;
		flag = 1;
	}
	else
	{
		P2_3 = 1;
		flag = 0;
	}
}
/*********************************************************************************
* 名称：Init()
* 功能：设置计数器0 工作方式,16 位计数,溢出中断方式
**********************************************************************************/
void Init()
{
	TMOD = 0x01; //定时器0 处于计时方式,16 位
	EA = 1;
	ET0 = 1; //定时器0 溢出中断
}
/*********************************************************************************
* 名称：DelayMs(unsigned int time)
* 功能：延时time * 1ms 时间
*********************************************************************************/
void DelayMs(unsigned int time)
{
	unsigned int i;
	unsigned int j;
	for (j =0; j < time; j++) //每个循环 约 3ms
	{
		for (i =0; i < 363; i++)
		{;}
	}
}