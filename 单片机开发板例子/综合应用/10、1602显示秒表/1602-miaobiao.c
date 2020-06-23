/***************************************************************

*******************************************************************
* 工作芯片：STC89C516      晶振频率：11.0592MHz                   	*
*                                                                 	*
* 作者： 郭天祥 (Email: txmcu@163.com)                            	*
* 网站： www.tx-power.com(天祥电子)                               	*
* 邮箱： txmcu@163.com                                           	*
*                                                                 	*
*【版权】Copyright(C)天祥电子 www.tx-power.com  All Rights Reserved     *
*【声明】此程序仅用于学习与参考，引用请注明版权和作者信息！             *
*                                                                 	*
*************************************************************************
*                                                                 	*
* 描述：                                                          	*
*                                                                 	*
*   上电后液晶屏先显示网站等信息，接着按下S4，定时开始，再次按下	*
*                                                                 	*
*   S4暂停，第3次按下显示累积计时，第4次按下暂停计时，任何时候按下S5*
*                                                                 	*
*   计数清零。                                             		    *
*									                                *
*                                                                 	*
************************************************************************/

#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

uchar KeyCount=0;

sbit  K3 = P3^6;
sbit  K4 = P3^7;

sbit BEEP = P2^3;          //蜂鸣器

uchar code  cdis1[ ] = {"  STOPWATCH  0  "};
uchar code  cdis2[ ] = {"www.tx-power.com"};
uchar code  cdis3[ ] = {"TIME            "};

uchar code  cdis4[ ] = {" BEGIN COUNT  1 "};
uchar code  cdis5[ ] = {" PAUSE COUNT  2 "};
uchar code  cdis6[ ] = {" BEGIN COUNT  3 "};
uchar code  cdis7[ ] = {" PAUSE COUNT  4 "};
uchar code  cdis8[ ] = {"                "};

sbit LCD_RS = P3^5;             
//sbit LCD_RW = P2^1;
sbit LCD_EN = P3^4;

#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

uchar display[] =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uchar display2[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*********************************************************
 延时函数
*********************************************************/

void Delay(uint num)//延时函数
{
  while( --num );
}

/*********************************************************
 蜂鸣器响一声                                           
**********************************************************/
void beep()
{
    unsigned char y;
    for (y=0;y<180;y++)
    {
      BEEP=!BEEP;                //BEEP取反
	  Delay(70);
    } 
    BEEP=1;                      //关闭蜂鸣器
}

/*********************************************************
 延时函数1
*********************************************************/
void delay1(int ms)
{
   unsigned char n;
   while(ms--)
   {
     for(n = 0; n<250; n++)
    {
      _nop_();
      _nop_();
      _nop_();
      _nop_();
    }
   }
}

/**********************************************************
*                                                         *
* 检查LCD忙状态                                           *
* lcd_busy为1时，忙，等待。                               *
* lcd-busy为0时,闲，可写指令与数据。                      *
*                                                         *
**********************************************************/ 
bit lcd_busy()
 {                          
    bit result;
    LCD_RS = 0;
 //   LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(P0&0x80);
    LCD_EN = 0;
    return(result); 
 }

/**********************************************************
*                                                         *
*写指令数据到LCD                                          *
*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                     *
*                                                         *
**********************************************************/
void lcd_wcmd(uchar cmd)
{                          
//   while(lcd_busy());
    LCD_RS = 0;
//    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
    Delay(10);
}

/**********************************************************
*                                                         *
*写显示数据到LCD                                          *
*RS=H，RW=L，E=高脉冲，D0-D7=数据。                       *
*                                                         *
**********************************************************/
void lcd_wdat(uchar dat)
{                          
//   while(lcd_busy());
    LCD_RS = 1;
//    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
    Delay(10);
}

/**********************************************************
*                                                         *
*  LCD初始化设定                                          *
*                                                         *
**********************************************************/
void lcd_init()
{ 
    delay1(15);   
    lcd_wcmd(0x01);      //清除LCD的显示内容            
    lcd_wcmd(0x38);      //16*2显示，5*7点阵，8位数据
    delay1(5);
    lcd_wcmd(0x38);         
    delay1(5);
    lcd_wcmd(0x38);         
    delay1(5);

    lcd_wcmd(0x0c);      //开显示，不显示光标  
    delay1(5);

    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay1(5);
}

/**********************************************************
*                                                         *
*  设定显示位置                                           *
*                                                         *
**********************************************************/

void lcd_pos(uchar pos)
{                          
  lcd_wcmd(pos | 0x80);  //数据指针=80+地址变量
}

/********************************************************
 显示函数
*********************************************************/
void play()
{
   uchar  i;
   for(i=0;i<6;i++)
   {
    display2[i]=display[i]+0x30;  
   }

   display2[7]=display[6]/10+0x30;   //时单位数据处理
   display2[6]=display[6]%10+0x30;
   
   lcd_pos(0x45);
   lcd_wdat(display2[7]);   //显示时
   lcd_wdat(display2[6]);
   lcd_wdat(0x3a);          //显示':'
  
   lcd_wdat(display2[5]);   //显示分
   lcd_wdat(display2[4]);
   lcd_wdat(0x3a);          //显示':'

   lcd_wdat(display2[3]);   //显示秒
   lcd_wdat(display2[2]);
   lcd_wdat(0x3a);          //显示':'

   lcd_wdat(display2[1]);   //显示毫秒
   lcd_wdat(display2[0]);  
}

/********************************************************
 主函数
*********************************************************/
main()
{
   uchar m;
  
   TMOD=0x01; 
   TH0=0xdc; TL0=0x00;      //50ms定时   
   EA=1; ET0=1;

   lcd_init();
   
   lcd_pos(0x00);             //设置显示位置为第一行
   for(m=0;m<16;m++) 
   lcd_wdat(cdis1[m]);        //显示字符 

   lcd_pos(0x40);             //设置显示位置为第二行
   for(m=0;m<16;m++)
   {
    lcd_wdat(cdis2[m]);        //显示字符
    delay1(60);
   }

   delay1(1000);

   lcd_pos(0x40);             //设置显示位置为第二行
   for(m=0;m<16;m++)
   lcd_wdat(cdis3[m]); 

   while(1)
   {   
     if(K3==0)
     {
	   KeyCount++;          //计数
       beep();
	   delay1(100);    
     
       switch (KeyCount)
       {
        case 1: 
        TR0=1;              //启动中断
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis4[m]);     
        break;
  
        case 2: 
        TR0=0;              //停止中断
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis5[m]);     
        break;
      
		case 3: 
        TR0=1;              //启动中断
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis6[m]);     
        break;

        case 4: 
        TR0=0;              //停止中断
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(cdis7[m]);     
        break;
     
        default:  
        TR0=0;                   //停止中断 
        break;
       }
     }
     if(K4==0)
     {
	   TR0=0;                     //停止中断
       KeyCount=0;
	   for(m=0;m<8;m++)
       display[m]=0x00;           //计时单元清零
       lcd_pos(0x00);             //设置显示位置为第一行
       for(m=0;m<16;m++)
       lcd_wdat(cdis1[m]); 
       beep();
       delay1(100);              
     } 

     play(); 
   }
}

/*********************************************************
*                                                        *
* Time0中断函数                                          *
*                                                        *
**********************************************************/
void Time0(void) interrupt 1 using 0
{
   TH0=0xdc;               //10ms定时
   TL0=0x00;

   display[0]++;         //0.01S  

   if(display[0]==10)
   {
    display[0]=0;
	display[1]++;         //0.1S
   }
   if(display[1]==10)
   {
    display[1]=0;
    display[2]++;         //秒个位
   }
   if(display[2]==10)
   {
    display[2]=0;
    display[3]++;         //秒十位
   }
   if(display[3]==6)
   {
    display[3]=0;
    display[4]++;         //分个位
   }
   if(display[4]==10)
   {
    display[4]=0;
    display[5]++;         //分十位
   }
   if(display[5]==6)
   {
    display[5]=0;
    display[6]++;        //时
   }
   if(display[6]==24)
   {
     display[6]=0;
   } 
}

/*********************************************************/
