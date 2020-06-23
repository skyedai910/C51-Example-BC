
#include<reg51.h>
#include <math.h>
#define  PI 3.1415
unsigned int num;
void main() {
  while (1) {
    for (num = 0 ; num < 360 ; num++)
		P2= 127 +127 * sin((float)num / 180 * PI);
  }
}
