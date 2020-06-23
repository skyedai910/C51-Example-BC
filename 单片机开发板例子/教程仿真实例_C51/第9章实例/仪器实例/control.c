#include <reg51.h>

sbit P36 = P3^6;
sbit P37 = P3^7;

unsigned char ad();
void print(char name, unsigned int value);
extern unsigned char param_value[2];
extern char menu_status;

void control_thread() {
	//第1步:A/D转换
	unsigned char value = ad();
	//第2步:根据采样值控制LED灯
	if (value > param_value[1]) {
		P36 = 0;
		P37 = 1;
	} else if (value <= param_value[1] && value >= param_value[0]) {
	 	P36 = 1;
		P37 = 1;
	} else {
		P36 = 1;
	 	P37 = 0;
	}
	//第3步:如果菜单是关闭的，显示采集到的数值
	if (menu_status == 1) {
		print(' ', value);
	}
}