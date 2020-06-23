//可观察到发光二极管由灭到亮，而后由亮到灭。
//若用示波器观察IOUT引脚电平，可看到正弦波波形。
#include<reg51.h>
#include <math.h>
sbit wela=P2^7;
sbit dula=P2^6;
sbit dawr=P3^6;
sbit csda=P3^2;
unsigned char j,k;
unsigned char  dat[90];

void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
} 
void main()
{
  unsigned char m;
  wela=0;
  dula=0;
  csda=0;
  dawr=0;
 for(m=0;m<90;m++)
 { unsigned char d;
  d=127*(sin(2*m*3.1415926/90)+1);	 //	90-->2pi,m-->2*pi*m/90
  dat[m]=d;	 	
  }

  while(1)
  {
	for(m=0;m<90;m++)
	{P0=dat[m];
	delay(10);}
  }	
}
