#include<absacc.h>
#define DAC0832 XBYTE[0xfeff]	   		//设置DAC0832的访问地址
unsigned char num;
void main() {
  while (1) {
    for (num = 0 ; num < 255 ; num++) 	//上升段波形
	  DAC0832=num;
   	for (num = 255 ; num > 0 ; num--) 	//下降段波形
	DAC0832=num;
										//DAC0832转换输出
  }
}		  

