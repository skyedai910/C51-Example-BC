//实例2 独立按键识别
#include <REG51.H>	

void main() {
   char key = 0;
   while(1){
   		key = P0 & 0x0f;       	//读取按键状态
		if (key != 0x0f) P2 = key;	//显示到led
	}
}


   
  