#include <absacc.h>
#define U2 XBYTE [0x7fff]
#define U3 XBYTE [0xbfff]

void main(void){
	U2=0xaa;	 //U2หอ1010 1010b
	U3=0x0f; 	 //U3หอ0000 1111b   
    while(1);
}
