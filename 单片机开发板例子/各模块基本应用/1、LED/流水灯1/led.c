#include<reg52.h>
#include<intrins.h>

void delay(unsigned char i)
{
	unsigned char m,n;
	for(m=i;m>0;m--)
	for(n=125;n>0;n--);
}

void main()
{	unsigned char k;
while(1)
{
	P1=0xfe;
	for(k=0;k<8;k++)
	{
	delay(2000);
	P1=_crol_(P1,1);
	}
}
}