/*********************************************************/
/*                    实例4                             */
/*********************************************************/
int value,key=1;//定义value和key
main(void)			//主函数
{
next:switch(key) 
	{	case 1:value=11;break;
		case 2:value=35;break;
		case 3:value=72;break;
		default:goto end;
	} 
	key++;
	goto next; 
end:key=0;		//结束程序
}