/**************************************************************************
					
***************************************************************************
程序功能：	将一数字量不断自加后输给DA芯片，将数字量转化为模拟量，
			并通过小灯的亮灭程度显示出来。
			下载程序后可观察到DA芯片下面的D12发光二极管由暗变亮再熄灭过程，
*************************************************************************/
#include<reg51.h>
sbit wela=P2^7;
sbit dula=P2^6;
sbit dawr=P3^6;
sbit csda=P3^2;
unsigned char a,j,k;
void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
void main()
{
  wela=0;
  dula=0;
  csda=0;
  a=0;
  dawr=0;
  while(1)
  {

  P0=a;
  delay(50);
  a++;
  }
}
