/*
proj:多机通信（主机）程序
desc:基于80c51通过串口实现多机通信，主机具备校验返回信息功能。主机接收信息使用定时器中断完成。
    主机利用中断服务扫描矩阵键盘获取子机序列，子机数量可拓展。
author:mrskye
website:https://www.mrskye.cn/
encoding:utf-8
*/

#include <reg51.h>
#define uchar unsigned char
#define NODE0_ADDR 0        //0#主机地址
#define NODE1_ADDR 1  		//1#子机地址

/*测试引脚*/
sbit P1_4=P1^4;
sbit P1_7=P1^7;

/*矩阵键盘全局变量*/
unsigned char flag_int0 = 0;
//行列式键盘键值
unsigned char key_buf[] = {0xee, 0xde, 0xbe, 0x7e,0xed, 0xdd, 0xbd, 0x7d,0xeb, 0xdb, 0xbb, 0x7b,0xe7, 0xd7, 0xb7, 0x77};

/*led显示全局变量*/
sbit DULA = P1^0;
sbit WELA = P1^1;
//7led显示字形码(阴极)
unsigned char wordlist[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};
//led灯位
unsigned char ledlist[] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
//默认显示列表
unsigned char display_list[] = {0x40,0x40};


/*串口通信全局变量*/
uchar KeyValue=0;			                //键值
uchar send_str[]="HELLO";                   //发送字符存储列表
uchar recv_str[]="AAAAA";                   //接受字符存储列表
uchar flag_sms = 0;                         //收发信息正确标志
uchar pointer_recv = 0;		                //主机当前接收字符指针
uchar flag_recv_done = 0;                   //接收完成标志

void delay(unsigned int i)                  //可控时间延迟函数
{
    unsigned int j;
    unsigned char k;
    for(j=i;j>0;j--)
    {
        for(k=125;k>0;k--);
    }
}

void display(unsigned char display_list[],unsigned int time)//led显示函数
{
    unsigned char i;
    for(i=0;*(display_list+i);i++)
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

void service_int0(void) interrupt 0         //外部中断0：启动键盘扫描
{
    flag_int0 = 1;
}

unsigned char getKey()                      //矩阵键盘取值函数
{	unsigned char key_scan[] = {0xef, 0xdf, 0xbf, 0x7f};	   //键扫描码
    unsigned char i = 0, j = 0;
    for (i = 0;key_scan[i]; i++) 
    {	
        P2 = key_scan[i];  						   //P2送出键扫描码
        if ((P2 & 0x0f) != 0x0f) 				   //取P2低四位判断有无键闭合
        {	
            delay(100);//消抖
            if ((P2 & 0x0f) != 0x0f)               //再次判断键闭合
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

void receive(void) interrupt 4              //定时器中断1：串口接收
{    
    RI=0;
    if(RB8==1)
    {
        if(SBUF==NODE0_ADDR)
        {
            SM2=0;
        }
        return;
    }
    //存储到列表
    *(recv_str+pointer_recv)=SBUF;
    if(++pointer_recv>4)
    {
        flag_recv_done=1;
        pointer_recv=0;
    }
    P1_4=!P1_4;
    SM2=1;
}

void proc_key(uchar node_number,uchar str[])//发送程序
{
    switch(node_number)                     //切换子机
    {
        case 1: 
        {
            uchar pointer_1;		        //子机当前发送字符指针
            for(pointer_1=0;*(str+pointer_1);pointer_1++)
            {
                
                TB8=1;	   			        //发送地址帧
                SBUF=node_number;           //子机地址
                while(TI==0);		        //等待地址帧发送结束
                TI=0;				        //清TI标志
                TB8=0;				        //准备发送数据帧
                SBUF=*(str+pointer_1);	    //1#子机字符帧
                while(TI==0);		        //等待地址帧发送结束
                TI=0;				        //清TI标志
                delay(1200);                //发送延时
            }
        }
        default: break;
    }
}

void main()
{
    /*中断服务初始化*/
    IE = 0x81;                              //10000001|总中断、外部中断0
    TCON = 0x00;                            //00000000|中断触发方式
    /*串口服务初始化*/
    SCON=0xf0;			                    //串口方式3、多机通信、允许接收、中断标志清零
    TMOD=0x20;			                    //T1定时方式2
    TH1=TL1=0xfd;		                    //9600bps
    TR1=1;				                    //启动T1
    ES=1;EA=1;		                        //开中断
    /*其余服务初始化*/
    P1=0xff;                                //复位关闭全部LED
    P2=0x0f;                                //键盘待触发状态
    while (1)
    {
        if(flag_int0)
        {
            unsigned char key;
            key = getKey();                 //获取子机序号
            switch(key)                     //切换子机
            {			  
                case 0x00: proc_key(NODE1_ADDR,send_str);break;
                default: break;
            }
            P2=0x0f;                        //键盘待触发状态
            flag_int0 = 0;                  //flag_int0复位
        }
        if(flag_recv_done)
        {
            unsigned char i;                //校验指针
            for(i=0;send_str[i];i++)
            {
                if(send_str[i]!=recv_str[i])//收发不一致
                {
                    display_list[0] = 0x54;
                    display_list[1] = 0x5c;
                    flag_recv_done = 0;     //标志位置零
                    break;
                }
            }
            display_list[0] = 0x76;         //收发一致
            display_list[1] = 0x06;
            flag_recv_done = 0;             //标志位置零
        }
        display(display_list,20);
    }    
}
