//可观察到发光二极管由暗到亮，然后由亮到暗的渐变。
//若用示波器观察，可看到三角波波形。
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
	unsigned char m;
	for(m=0;m<255;m++)
	{
		P0=m;
		delay(20);
	}
	for(m=255;m>0;m--)
	{
		P0=m;
		delay(20);
	}
  }
}
