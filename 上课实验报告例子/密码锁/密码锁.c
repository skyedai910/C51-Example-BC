/*
proj:两位可修改密码密码锁
author:mrskye
website:https://www.mrskye.cn/
encoding:gbk
*/
#include<reg52.h>

sbit DULA = P3^0;
sbit WELA = P3^1;

//7led显示字形码
unsigned char wordlist[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};
//led灯位
unsigned char ledlist[] = {0xfe,0xfd};
//行列式键盘键值
unsigned char key_buf[] = {0xee, 0xde, 0xbe, 0x7e,0xed, 0xdd, 0xbd, 0x7d,0xeb, 0xdb, 0xbb, 0x7b,0xe7, 0xd7, 0xb7, 0x77};
//显示字符列表
unsigned char display_list[] = {0x00,0x00};
//锁状态(默认锁)
unsigned char lock = 1;
//密文(初始密码08)
unsigned char c[] = {0x3f,0x7f};
//初始化key
unsigned char key = 0;
//初始化led显示位
unsigned char led_set = 0;

void delay(unsigned int i)
{
    unsigned int m,n;
    for(m=i;m>0;m--)
		for(n=90;n>0;n--);
}

unsigned char getKey() 
{	unsigned char key_scan[] = {0xef, 0xdf, 0xbf, 0x7f};	   //键扫描码
	unsigned char i = 0, j = 0;
	for (i = 0; i < 4 ; i++) 
	{	P2 = key_scan[i];  						   //P2送出键扫描码
		if ((P2 & 0x0f) != 0x0f) 				   //判断有无键闭合
		{	
            delay(100);//消抖
            if ((P2 & 0x0f) != 0x0f)
            {
                for (j = 0 ; j < 16 ;j++) //低效而方便地遍历查找闭合键键号
			    {	
                    if (key_buf[j]==P2)
                    {
                        while ((P2 & 0x0f) != 0x0f);//松开按键
                        return j;
                    } 
			    }	
            }
		}
	}
    return -1;//无键闭合								   
}

void display(unsigned char display_list[],unsigned int time)
{
    unsigned char i;
    //for(i=0;*(display_list+i);i++)
    for(i=0;i<2;i++)//2对应led显示位数
    {
        //选择显示位
        P0=*(ledlist+i);
        WELA=1;
        WELA=0;
        //输出字符码
        P0=*(display_list+i);
        DULA=1;
        DULA=0;
        delay(time);
    }
}

void fini(unsigned char word)
{
    unsigned char i;
    for(i=0;i<2;i++)//2对应led显示位数
    {
        *(display_list+i)=word;
    }
}

void setDisplayList(unsigned char key)
{
    if(key!=-1)
    {
        *(display_list+led_set)=wordlist[key];
        ++led_set;
    }
}

void main(void) {
    fini(0x40);//初始化led
    while (1)
    {  
        key = getKey();//获取键值
        setDisplayList(key);//设定显示列表
        display(display_list,10);//驱动led显示
        if (led_set==2)//输入两位密码
        {
            if(lock)
            //有锁操作
            {
                if(display_list[0]==c[0] && display_list[1]==c[1])
                {
                    lock=0;
                    fini(0x73);
                    display(display_list,10);//驱动led显示
                }
            }
            led_set=0;//灯位初始化
        }
        //修改密码部分
        if(!lock)
        {
            do
            {
                display(display_list,10);//驱动led显示
            }while(getKey()!=15);
            fini(0x40);//初始化led
            while(led_set<2)
            {
                if((key = getKey())!=-1)//获取键值
                {
                    setDisplayList(key);//设定显示列表
                }
                display(display_list,10);//驱动led显示
            }
            do
            {
                display(display_list,10);//驱动led显示
            }while (getKey()!=14);//等待按下确定
            c[0]=display_list[0];//修改密码
            c[1]=display_list[1];
            lock=1;//上锁
            fini(0x40);//初始化led
        }
    }
}