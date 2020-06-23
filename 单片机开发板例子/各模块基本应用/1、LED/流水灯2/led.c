#include<reg52.h>

void delay(unsigned char i)
{
	unsigned char m,n;
	for(m=i;m>0;m--)
	for(n=125;n>0;n--);
}

void main()
{	while(1)
{
	P1=0x7e;
	delay(2000);
	P1=0xbd;
	delay(2000);
	P1=0xdb;
	delay(2000);
	P1=0xe7;
	delay(2000);
	P1=0xdb;
	delay(2000);
	P1=0xbd;
	delay(2000);
	P1=0x7e;
	delay(2000);
}
}