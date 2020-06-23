 #include <reg51.h>
char bit_tab[]={0x20,0x10,0x08,0x04,0x02,0x01};	//显示位编码
char disp_tab[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//字形码

void delay(unsigned int time){	
	char j;
	for(;time>0;time--)	
		for(j=225;j>0;j--);
}	 

void main()	{  
	unsigned int count;						//定义T0计数值
	unsigned char led_point; 				//定义数码管指针
	TMOD = 0x0D;							//设置为T0计数方式1
	TR0 = 1;								//启动T0
	while(1){
		count = TH0 * 256 + TL0;			//获取T0计数值
		for(led_point = 0;led_point < 6; led_point++){ 
		   	P1 = bit_tab[5-led_point];		//控制显示位
			P2 = disp_tab[count%10];		//显示计数值
			count /= 10;					//消除计数值末位
			delay (500);
		}
	}
}