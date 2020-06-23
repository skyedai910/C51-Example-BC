void ad_init();
void control_thread();
void menu_thread();

void main() {
	ad_init();
	while (1) {
		menu_thread();
		control_thread();
	}
}