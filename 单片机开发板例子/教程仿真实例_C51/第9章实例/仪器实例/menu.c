#define MENU_ON		0
#define MENU_OFF	1
#define MENU_NUM	2
#define MENU_MAX	240
#define MENU_MIN	10

unsigned char param_value[] = {100, 150};
unsigned char menu_status = MENU_OFF;
char _menu_name[] = {'L','H'};	 		 //参数名的符号
unsigned char _menu_value[] = {0,0};			 	 //供显示用的参数数组
unsigned char _menu_idx = 0;			 //参数序号

char get_key();
void print(char name, unsigned int value);

void menu_thread(void) {
	char i = 0;
	char key = get_key();		         
	if (menu_status == MENU_OFF) {		 
		//当前菜单为关闭状态时
		if (key == 0) {					 //若按键0已被按下
			menu_status = MENU_ON;		 //置当前菜单为打开状态
			_menu_idx = 0;				 //设置参数序号0
			//将所有参数当前值取出，送入供显示的参数数组中
			for (i = 0 ; i < MENU_NUM ; i++) {	  
				_menu_value[i] = param_value[i];	
			}
		}
	} else {							 
		//如果当前菜单为打开状态，则进行以下操作：
		if (key == 0) { 
			//若按键0按下，则不保存键值，仅切换到下一个参数
			if (++_menu_idx == MENU_NUM) {	  //判断是否所有参数都循环到了
				menu_status = MENU_OFF;	  //若已循环完成，设置菜单关闭状态
			}
		} else if (key == 1) { 
			//若按键1按下，则保存键值，并切换到下一个参数
			param_value[_menu_idx] = _menu_value[_menu_idx];
			if (++_menu_idx == MENU_NUM) {
				menu_status = MENU_OFF;
			}
		} else if (key == 2) { 
			//若按键2按下，则参数值加10
			_menu_value[_menu_idx] += 10;
			if (_menu_value[_menu_idx] >= MENU_MAX) {
				_menu_value[_menu_idx] = MENU_MAX;
			}
			
		} else if (key == 3) { 
			//若按键3按下，则参数值减10
			_menu_value[_menu_idx] -= 10;
			if (_menu_value[_menu_idx] <= MENU_MIN) {
				_menu_value[_menu_idx] = MENU_MIN;
			}
		}
	}

	if (menu_status == MENU_ON) {
		//菜单状态为开时,显示参数值
		print(_menu_name[_menu_idx], _menu_value[_menu_idx]); 
	}
}