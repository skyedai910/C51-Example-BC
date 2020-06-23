#include <reg51.h>
sbit p2_0=P2^0;
sbit p3_2=P3^2;



main(){
   p2_0=1;
   while(1){
      if(p3_2==0) {
	 
	     p2_0=!p2_0;
		 
		 
		 
	  }
   }
}  