#include <reg51.h>
unsigned char xdata *PORT; //定义端口指针变量
void main(){
    unsigned char tmp;
	*PORT=0xff;				//启动后置黑屏
    while(1){
		tmp = *PORT; 		//从74244端口读取数据
		if(tmp!=0xff)*PORT=tmp;//若有键动作，键值送74273
    }                       
}
