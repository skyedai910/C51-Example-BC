#include <reg51.h>
sbit p2_0=P2^0;

int0_srv () interrupt 0{
   p2_0 = !p2_0; 
}  

main(){
   IT0=1;
   EX0=1;
   EA=1;
   while(1);
} 

