#include<reg51.h>

void delay(unsigned char i)
{
	unsigned char m,n;
	for(m=i;m>0;m--)
	for(n=125;n>0;n--);
}

void main()
{
	unsigned char m=255;
	while(1)
	{
		P1=m;
		delay(2000);
		m--;
	}
}