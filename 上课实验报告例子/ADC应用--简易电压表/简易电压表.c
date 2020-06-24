#include<reg52.h>        
#include <intrins.h> 
typedef unsigned char uchar;

#define AddWr 0x90        //PCF8591 地址

sbit en = P3 ^ 4;           //lcd1602写数据
sbit rs = P3 ^ 5;           //lcd1602写指令
sbit scl=P2^0;             //I2C    时钟 
sbit sda=P2^1;             //I2C    数据 
bit ack;                   //应答标志位

unsigned char date;
sbit C1=P2^6;//数码管1
sbit C2=P2^7;//数码管2
sbit Dp=P2^5;//小数点
uchar code table[] =
{
    "0123456789."
};
uchar code table1[] =
{
    "SMC1602 0x01..."
};
unsigned int data dis[3]={0x00,0x00,0x00}; 
unsigned int getData;
/*******************************************************************
 起动总线函数                             
函数原型: void    Start_I2c();    
功能:         启动I2C总线,即发送I2C起始条件.    
********************************************************************/
void Start_I2c()
{
    sda=1;                 /*发送起始条件的数据信号*/
    _nop_();
    scl=1;
    _nop_();                /*起始条件建立时间大于4.7us,延时*/
    _nop_();
    _nop_();
    _nop_();
    _nop_();        
    sda=0;                 /*发送起始信号*/
    _nop_();                /* 起始条件锁定时间大于4μs*/
    _nop_();
    _nop_();
    _nop_();
    _nop_();             
    scl=0;             /*钳住I2C总线，准备发送或接收数据 */
    _nop_();
    _nop_();
}

/*******************************************************************
结束总线函数                             
函数原型: void    Stop_I2c();    
功能:         结束I2C总线,即发送I2C结束条件.    
********************************************************************/
void Stop_I2c()
{
    sda=0;            /*发送结束条件的数据信号*/
    _nop_();             /*发送结束条件的时钟信号*/
    scl=1;            /*结束条件建立时间大于4μs*/
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    sda=1;            /*发送I2C总线结束信号*/
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

/*******************************************************************
 字节数据发送函数                             
函数原型: void    I2C_SendByte(UCHAR c);
功能:         将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
此状态位进行操作.(不应答或非应答都使ack=0)         
发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void I2C_SendByte(unsigned char    c)
{
    unsigned char    i;

    for(i=0;i<8;i++)    /*要传送的数据长度为8位*/
    {
        if((c<<i)&0x80)sda=1;     /*判断发送位*/
        else    sda=0;                                
        _nop_();
        scl=1;                             /*置时钟线为高，通知被控器开始接收数据位*/
        _nop_(); 
        _nop_();                         /*保证时钟高电平周期大于4μs*/
        _nop_();
        _nop_();
        _nop_();                 
        scl=0; 
    }
    _nop_();
    _nop_();
    sda=1;                                /*8位发送完后释放数据线，准备接收应答位*/
    _nop_();
    _nop_();     
    scl=1;
    _nop_();
    _nop_();
    _nop_();
    if(sda==1)ack=0;         
    else ack=1;                /*判断是否接收到应答信号*/
    scl=0;
    _nop_();
    _nop_();
}

/*******************************************************************
 字节数据接收函数                             
函数原型: UCHAR    I2C_RcvByte();
功能:                用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
发完后请用应答函数应答从机。    
********************************************************************/        
unsigned char I2C_RcvByte()
{
    unsigned char    retc=0,i; 
    sda=1;                                         /*置数据线为输入方式*/
    for(i=0;i<8;i++)
    {
        _nop_();                     
        scl=0;                                    /*置时钟线为低，准备接收数据位*/
        _nop_();
        _nop_();                                 /*时钟低电平周期大于4.7μs*/
        _nop_();
        _nop_();
        _nop_();
        scl=1;                                    /*置时钟线为高使数据线上数据有效*/
        _nop_();
        _nop_();
        retc=retc<<1;
        if(sda==1)retc=retc+1;    /*读数据位,接收的数据位放入retc中 */
        _nop_();
        _nop_(); 
    }
    scl=0;        
    _nop_();
    _nop_();
    return(retc);
}

/********************************************************************
 应答子函数
函数原型:    void Ack_I2c(bit a);
功能:            主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(bit a)
{    
    if(a==0)sda=0;                            /*在此发出应答或非应答信号 */
    else sda=1;				    /*0为发出应答，1为非应答信号 */
    _nop_();
    _nop_();
    _nop_();            
    scl=1;
    _nop_();
    _nop_();                                        /*时钟低电平周期大于4μs*/
    _nop_();
    _nop_();
    _nop_();    
    scl=0;                                         /*清时钟线，住I2C总线以便继续接收*/
    _nop_();
    _nop_();        
}

/************************************************************
* 函数名                : Pcf8591_DaConversion
* 函数功能            : PCF8591的输出端输出模拟量
* 输入                    : addr（器件地址），channel（转换通道），value（转换的数值）
* 输出                 	: 无
******************* *****************************************/
bit Pcf8591_DaConversion(unsigned char addr,unsigned char channel,unsigned char Val)
{
    Start_I2c();                            //启动总线
    I2C_SendByte(addr);                        //发送器件地址
    if(ack==0)return(0);
    I2C_SendByte(0x40|channel);                            //发送控制字节
    if(ack==0)return(0);
    I2C_SendByte(Val);                        //发送DAC的数值    
    if(ack==0)return(0);
    Stop_I2c();                             //结束总线
    return(1);
}

/************************************************************
* 函数名                : Pcf8591_SendByte
* 函数功能		        : 写入一个控制命令
* 输入                  : addr（器件地址），channel（转换通道）
* 输出                  : 无
************************************************************/
bit PCF8591_SendByte(unsigned char addr,unsigned char channel)
{
    Start_I2c();                            //启动总线
    I2C_SendByte(addr);                        //发送器件地址
    if(ack==0)return(0);
    I2C_SendByte(0x40|channel);                            //发送控制字节
    if(ack==0)return(0);
    Stop_I2c();                             //结束总线
    return(1);
}

/************************************************************
* 函数名             	: PCF8591_RcvByte
* 函数功能            	: 读取一个转换值
* 输入                    :
* 输出                    : dat
************************************************************/
unsigned char PCF8591_RcvByte(unsigned char addr)
{    
    unsigned char dat;

    Start_I2c();                    //启动总线
    I2C_SendByte(addr+1);            //发送器件地址
    if(ack==0)return(0);
    dat=I2C_RcvByte();                    //读取数据0

    Ack_I2c(1);                     //发送非应答信号
    Stop_I2c();                     //结束总线
    return(dat);
}
/*------------------------------------------------
串口初始化函数
------------------------------------------------*/
void init_com(void)
{
    EA=1;                //开总中断
    ES=1;                //允许串口中断
    ET1=1;
    TMOD=0x22;     //定时器T1，在方式2中断产生波特率
    PCON=0x00;     //SMOD=0
    SCON=0x50;     // 方式1 由定时器控制
    TH1=0xfd;        //波特率设置为9600
    TL1=0xfd;
    TR1=1;             //开定时器T1运行控制位
}
/*------------------------------------------------
延时函数
------------------------------------------------*/
void delay(unsigned char i)
{
    unsigned char j,k; 
    for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
/*------------------------------------------------
把读取值转换成一个一个的字符，给串口显示
------------------------------------------------*/
void To_ascii(unsigned char num)
{	
    SBUF=num/100+'0';		     	     
    delay(200);		    
    SBUF=num/10%10+'0';			     
    delay(200);	
    SBUF=num%10+'0';
    delay(200);
}
/* 写数据 */
void write_date(uchar date)
{
    //参考写操作时序图
    rs = 1; //RS高电平,写数据
    P0 = date; //写入数据,对应时序图t_SP2数据建立
    delay(5);
    en = 1; //打开使能,给E一个高脉冲,开始写入数据
    delay(5);
    en = 0; //关闭使能,写入数据完成
}
/* 写指令 */
void write_com(uchar com)
{
    //基本原理同写数据
    rs = 0; //RS低电平,写指令
    P0 = com;
    delay(5);
    en = 1;
    delay(5);
    en = 0;
}
/* 初始化LCD */
void init_lcd()
{
    en = 1;
    write_com(0x38); //00111000 设置16*2显示,5*7点阵,8位数据接口
    write_com(0x0E); //00001110 开显示,显示光标,不闪烁
    write_com(0x06); //00000110 读写字符后地址指针+1,光标+1,屏幕不移动
    write_com(0x01); //清屏
    write_com(0x80 + 0x01); //设置数据地址指针从第一个开始
}
void Display()
{
    write_date(table[dis[1]]);//整数位
    delay(20);
    write_date(table[10]);//小数点
    delay(20);
    write_date(table[dis[0]]);//小数位
    delay(20);
}
/*void Display()interrupt 0 using 1
{
    uchar i;
    for (i = 0; i < 16; ++i) //遍历逐个写入第一行数据
    {
        write_date(table1[i]);
        delay(20);
    }
}*/
void Clear()
{
    write_com(0x01);
}
/*------------------------------------------------
主函数
------------------------------------------------*/
int main()
{    
    /* 初始化LCD */
    init_lcd();
    /* 初始化中断 */
    EA = 1;
    EX0 = 1;
    delay(100);
    while(1)
    {
        PCF8591_SendByte(AddWr,0);	 //启动转换
        getData=PCF8591_RcvByte(AddWr);    //读转换完的数字信号

        dis[1]=getData/51;     //整数位
        dis[2]=getData%51;     //dis[2]位中间暂存数据位
        dis[2]=dis[2]*10;        
        dis[0]=dis[2]/51;        //计算输出电压的小数值	

        Clear();                //清屏准备显示新内容
        Display();              //显示内容
    }
}