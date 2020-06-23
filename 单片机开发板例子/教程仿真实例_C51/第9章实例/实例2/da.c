#include <reg51.h>

sbit din = P2^0;
sbit clk = P2^1;
sbit cs = P2^2;

void _del(){}

void da(unsigned int v){
	char i = 11;
	cs = 1;
	_del();
	cs = 0;
	for ( ; i >= 0 ; i--) {
	 	din = (v >> i) & 0x01;
		clk = 1;
		_del();
		clk = 0;
	}
	cs = 1;
	_del();
	cs = 0;
}