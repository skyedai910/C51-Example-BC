#include <reg51.h>
sbit P24 = P2^4;
sbit P25 = P2^5;
sbit P26= P2^6;
sbit P27 = P2^7;
unsigned char ad() {
	P25 = 0;
	P25 = 1;
	P25 = 0;
	while(!P26);
	P27 = 1;
	return P1;
}
void ad_init() {
	TMOD = 0x02;
	TH0  = 0;
	TL0  = 0;	
	ET0  = 1;
	TR0  = 1;
	EA   = 1;
}
void _ad_clock(void) interrupt 1 {
	P24 = ~P24;
}
