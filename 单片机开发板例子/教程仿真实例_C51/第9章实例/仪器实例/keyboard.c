#include <reg51.h>

#define CHECK_KEY_DOWN 	0	   //处在检测按键压下阶段标志
#define CHECK_KEY_UP   	1	   //处在检测按键抬起阶段标志
#define KEY_UP			0	   //按键抬起标志
#define KEY_DOWN		1	   //按键压下标志

sbit _p32 = P3^2;


char _key_status = CHECK_KEY_DOWN; //按键检测状态（初值为检测压下阶段）
char _key_idx = 0;				   //按键序号

void serial(char byte);

char _check_key(unsigned char _key_idx) { //检查按键状态
	serial(~(0x01<<_key_idx));	  //将待查按键键码转换成扫描码后输出
	if (_p32 == 0) {					  //根据P32状态决定返回值
		return KEY_DOWN;
	} else {
		return KEY_UP;
	}
}

char get_key(void) {							
	char result = -1;							//无键按下时键值为-1
	if (_key_status == CHECK_KEY_DOWN) {		//如果当前处于检查压下阶段，进行以下操作
		if (_check_key(_key_idx) == KEY_DOWN) {	//判断当前扫描键的状态，若为压下标志则
			_key_status = CHECK_KEY_UP;			//将检查阶段标志设置为抬起
		} else {								//否则，将检查阶段标志设置为压下
			if (++_key_idx == 4) {				//判断是否4个按键已经轮流扫描一遍
				_key_idx = 0;					//是，则将待扫描按键号设为0
			}
		}
	} else if (_key_status == CHECK_KEY_UP) {	//如果当前处于检查抬起阶段，进行以下操作
		if (_check_key(_key_idx) == KEY_UP) {	//判断当前扫描键的状态，若为抬起标志则
			result = _key_idx;					//键值输出
			_key_status = CHECK_KEY_DOWN;		//按键检查阶段标志改为压下
			if (++_key_idx == 4) {				//判断是否4个按键已经轮流扫描一遍
				_key_idx = 0;					//是，则将待扫描按键号设为0
			}
		}
	}
	return result;
}