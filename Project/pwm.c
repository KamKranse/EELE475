/*
 * pwm.c
 *
 *  Created on: Nov 3, 2015
 *      Author: Kasy Treu, Kameron Kranse, Aziz
 */
#define PWM1_BASE_ADDRESS    0x01001180
#define GPS_SIGNAL          (int *) 0x01001000
#define GPS_READ_SIGNAL     (int *) 0x01001040
#define PWM1_CTRL           ((volatile int*) PWM1_BASE_ADDRESS)
#define PWM1_PERIOD         ((volatile int*) (PWM1_BASE_ADDRESS + 4))
#define PWM1_NEUTRAL        ((volatile int*)(PWM1_BASE_ADDRESS + 8))
#define PWM1_LARGEST        ((volatile int*)(PWM1_BASE_ADDRESS + 12))
#define PWM1_SMALLEST       ((volatile int*)(PWM1_BASE_ADDRESS + 16))
#define PWM1_ENABLE         ((volatile int*) (PWM1_BASE_ADDRESS + 20))

#define Switches            (volatile int *) 0x010011c0
#define LEDs 	            (int *) 0x01001250

#include<stdio.h>
#include <stdlib.h>

int main() {

	*PWM1_NEUTRAL = 0x000124f8;
	*PWM1_LARGEST = 0x000186a0;
	*PWM1_SMALLEST = 0x0000c350;
	*PWM1_ENABLE = 1;
	*PWM1_PERIOD = 0x0f4240;

	char c;
	char dataSet[80];
	while (1) {
		c = getchar();
		if (c == '$') {
			c = getchar();
			if (c == 'G') {
				dataSet[0] = c; //if the first character in the line after the $ is a G, store to array
				c = getchar();
				if (c == 'P') {
					dataSet[1] = c; //if the first character in the line after G is P, store to next array spot
					c = getchar();
					if (c == 'G') {
						dataSet[2] = c; //if the first character in the line after P is G, store to next array spot
						c = getchar();
						if (c == 'G') {
							dataSet[3] = c; //if the first character in the line after G is G, store to next array spot
							c = getchar();
							if (c == 'A') {
								dataSet[4] = c; //if the first character in the line after G is A, store to next array spot

								int i = 5;
								while (c != '*') {
									c = getchar(); // put all values in a char[] to XOR
									dataSet[i] = c;
									i++;
								}

								char myarray[2] = { dataSet[10], dataSet[11] }; // get seconds from GPS signal
								int time;

								sscanf(myarray, "%d", &time);
								// send seconds to GPS register to do the calculations
								*GPS_SIGNAL = time;
								// read back for another register and send it to control word
								*PWM1_CTRL = *GPS_READ_SIGNAL;

								*LEDs = time;
								printf(" %d\n", time);
								printf(" %d\n", *GPS_READ_SIGNAL);
							}
						}
					}
				}
			}
		}
	}

	return 0;
}
