

#define PWM1_BASE_ADDRESS    0x01001100
#define PWM1_CTRL           ((volatile int*) PWM1_BASE_ADDRESS)
#define PWM1_PERIOD         ((volatile int*) (PWM1_BASE_ADDRESS + 4))
#define PWM1_NEUTRAL        ((volatile int*)(PWM1_BASE_ADDRESS + 8))
#define PWM1_LARGEST        ((volatile int*)(PWM1_BASE_ADDRESS + 12))
#define PWM1_SMALLEST       ((volatile int*)(PWM1_BASE_ADDRESS + 16))
#define PWM1_ENABLE         ((volatile int*) (PWM1_BASE_ADDRESS + 20))

#define PWM1_READ_BASE_ADDRESS    0x01001140
#define PWM1_READ_CTRL           ((volatile int*) PWM1_READ_BASE_ADDRESS)
#define PWM1_READ_PERIOD         ((volatile int*) (PWM1_READ_BASE_ADDRESS + 4))
#define PWM1_READ_NEUTRAL        ((volatile int*)(PWM1_READ_BASE_ADDRESS + 8))
#define PWM1_READ_LARGEST        ((volatile int*)(PWM1_READ_BASE_ADDRESS + 12))
#define PWM1_READ_SMALLEST       ((volatile int*)(PWM1_READ_BASE_ADDRESS + 16))
#define PWM1_READ_ENABLE         ((volatile int*) (PWM1_READ_BASE_ADDRESS + 20))

#define Switches            (volatile int *) 0x010011c0
#define LEDs 	            (int *) 0x010011d0

#include<stdio.h>
#include <stdlib.h>

int main() {

	*PWM1_NEUTRAL = 0x000124f8;
	*PWM1_LARGEST = 0x000186a0;
	*PWM1_SMALLEST = 0x0000c350;
//	*PWM1_ENABLE = 1;
	*PWM1_PERIOD = 0x0f4240;

	while (1) {
		switch (*Switches) {

		case 1:
			*PWM1_ENABLE = 0x01;
			*PWM1_CTRL = -128;
			*LEDs = 1;
			break;

		case 2:
			*PWM1_ENABLE = 0x01;
			*PWM1_CTRL = 0;
			*LEDs = 2;
			break;

		case 4:
			*PWM1_ENABLE = 0x01;
			*PWM1_CTRL = 127;
			*LEDs = 4;
			break;
		default:
			*PWM1_ENABLE = 0x00;
			*LEDs = 0;
			break;
		}

		printf("CTRL: %d\n", (int)*PWM1_CTRL);
		printf("CTRL READ: %d\n", (int)*PWM1_READ_CTRL);
	}

	return 0;
}
