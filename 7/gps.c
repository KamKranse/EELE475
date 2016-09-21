/*
 * gps.c
 *
 *  Created on: Nov 3, 2015
 *      Author: v81t418
 */

#define Switches (volatile int *) 0x01001010
#define LEDs 	 (int *) 0x01001020

#include<stdio.h>
#include <stdlib.h>

int main() {

	char c;
	char dataSet[80];
	int calculatedChecksum = 0;
	int checkSumFromData;
	int ledWrite = 0;
	int sats[12];
	char checksum1;
	char checksum2;
	char str[5];
	char t[2];
	float latitude;
	float longitude;

	FILE *lcd;

	lcd = fopen("/dev/lcd_display", "w");

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
								//initialize the checkSum
								calculatedChecksum = 0;
								int k;
								//calculate the checkSum
								for (k = 0; k < i - 1; k++) {
									//xor each value within the packet before the checksum characters
									calculatedChecksum ^= (int) dataSet[k];
								}
								//check the checksum characters
								c = getchar();
								checksum1 = c;

								c = getchar();
								checksum2 = c;

								//build a string out of the read in checksum values into a hex value
								str[0] = '0';
								str[1] = 'x';
								str[2] = checksum1;
								str[3] = checksum2;
								str[4] = '\0';

								//convert this hex value into an integer for comparison to the calculated checksum
								sscanf(str, "%X", &checkSumFromData);
								//if the checksum is correct, continue
								if (calculatedChecksum == checkSumFromData) {
									//grab the hours portion of the time
									char myarray[2] = { dataSet[6], dataSet[7] };
									int time;
									//convert from universal correlated time into local time
									sscanf(myarray, "%d", &time);
									time = time - 7;
									//convert 24 hour time into 12 hour time with AM/PM
									if (time > 12) {
										time = time - 12;
										t[0] = 'P';
										t[1] = 'M';
									} else {
										t[0] = 'A';
										t[1] = 'M';
									}
									//update the time portions of data set.
									sprintf(myarray, "%d", time);
									dataSet[6] = myarray[0];
									dataSet[7] = myarray[1];
									//grab elevation portions of data set
									char Elevarray[6] = { dataSet[53],
											dataSet[54], dataSet[55],
											dataSet[56], dataSet[57],
											dataSet[58] };
									//convert the elevation values into feet
									float Elev;
									sscanf(Elevarray, "%f", &Elev);
									Elev = Elev * 3.2808;
									//update the elevation values of the data set
									sprintf(Elevarray, "%f", Elev);
									dataSet[53] = Elevarray[0];
									dataSet[54] = Elevarray[1];
									dataSet[55] = Elevarray[2];
									dataSet[56] = Elevarray[3];
									dataSet[57] = Elevarray[4];
									dataSet[58] = Elevarray[5];

									//case based around if the first switch if flipped or not
									switch (*Switches) {
									//if the first switch is not flipped write the elevation and time to the LCD
									case 0:
										fprintf(lcd, "Elev:%c%c%c%c%c%c FT\n",
												dataSet[53], dataSet[54],
												dataSet[55], dataSet[56],
												dataSet[57], dataSet[58]);
										fprintf(lcd,
												"TIME:%c%c:%c%c:%c%c %c%c\n",
												dataSet[6], dataSet[7],
												dataSet[8], dataSet[9],
												dataSet[10], dataSet[11], t[0],
												t[1]);
										break;
									//if the first switch is flipped write the latitude and longitude to the LCD
									case 1:
										fprintf(lcd,
												"Lat:  %c%c%c%c%c%c%c%c %c\n",
												dataSet[16], dataSet[17],
												dataSet[18], dataSet[19],
												dataSet[20], dataSet[21],
												dataSet[22], dataSet[23],
												dataSet[27]);
										latitude = (((dataSet[16] - 0x30) * 10)
												+ (dataSet[17] - 0x30))
												+ ((((dataSet[18] - 0x30) * 10)
														+ (dataSet[19] - 0x30))
														* .0167);
										printf("Latitude = %.2f\n", latitude);
										fprintf(lcd,
												"Long: %c%c%c%c%c%c%c  %c\n",
												dataSet[29], dataSet[30],
												dataSet[31], dataSet[32],
												dataSet[33], dataSet[34],
												dataSet[35], dataSet[41]);
										longitude =
												(((dataSet[29] - 0x30) * 100)
														+ ((dataSet[30] - 0x30)
																* 10)
														+ (dataSet[31] - 0x30))
														+ ((((dataSet[32] - 0x30)
																* 10)
																+ (dataSet[33]
																		- 0x30))
																* .0167);
										printf("Longitude = %.2f\n", longitude);
										break;
									}
								//if the checksum fails, print to the LCD that the GPS is not ready because data is invalid
								} else {
									fprintf(lcd,
											"GPS Unit not rdy.\nData is invalid\n");
								}
							}
						//check for satelite packet
						} else if (c == 'S') {
							dataSet[3] = c;
							c = getchar();
							if (c == 'A') {
								dataSet[4] = c;
								int l = 5;
								//read all characters before the checksum characters
								while (c != '*') {
									c = getchar();
									dataSet[l] = c;
									l++;
								}
								int m;
								calculatedChecksum = 0;
								//xor each value within the packet before the checksum characters
								for (m = 0; m < l - 1; m++) {
									calculatedChecksum ^= dataSet[m];
								}
								//get the first checksum character
								c = getchar();
								checksum1 = c;
								//get the second checksum characters
								c = getchar();
								checksum2 = c;
								// build a string out of the read in checksum characters into a hex value
								str[0] = '0';
								str[1] = 'x';
								str[2] = checksum1;
								str[3] = checksum2;
								str[4] = '\0';

								// then convert the hex string into an integer
								sscanf(str, "%x", &checkSumFromData);

								//check the two checksum values, if correct proceed
								if (calculatedChecksum == checkSumFromData) {
									//print out the satellites
									printf("Numbers of satellites: ");
									//use the commas as a counting system since they are always there
									int comma = 0;
									int place = 10;
									while (comma < 8) {
										//print out the satellite numbers and the commas but counting commas to know when to stop
										printf("%c", dataSet[place]); // always 8 commas
										if (dataSet[place] == ',') {
											comma++;
										}
										place++;
									}
									//print a new line character between each satellite reading
									printf("\n");
								} else {
									//if the checksum fails, print GPS is not ready to the LCD
									fprintf(lcd,
											"GPS Unit not rdy.\nData is invalid\n");
									printf("Sats incorrect, checksum failed\n");
								}
								//gather the satellite numbers to know which LEDs to turn on
								//builds an array of integers holding these values
								if(dataSet[10] == ','){
									sats[0] = 0;
								}else{
									sats[0] = (((dataSet[10]-0x30)*10) + (dataSet[11]-0x30));
								}
								if(dataSet[13] == ','){
									sats[1] = 0;
								}else{
									sats[1] = (((dataSet[13]-0x30)*10) + (dataSet[14]-0x30));
								}
								if(dataSet[16] == ','){
									sats[2] = 0;
								}else{
									sats[2] = (((dataSet[16]-0x30)*10) + (dataSet[17]-0x30));
								}
								if(dataSet[19] == ','){
									sats[3] = 0;
								}else{
									sats[3] = (((dataSet[19]-0x30)*10) + (dataSet[20]-0x30));
								}
								if(dataSet[22] == ','){
									sats[4] = 0;
								}else{
									sats[4] = (((dataSet[22]-0x30)*10) + (dataSet[23]-0x30));
								}
								if(dataSet[25] == ','){
									sats[5] = 0;
								}else{
									sats[5] = (((dataSet[25]-0x30)*10) + (dataSet[26]-0x30));
								}
								if(dataSet[28] == ','){
									sats[6] = 0;
								}else{
									sats[6] = (((dataSet[28]-0x30)*10) + (dataSet[29]-0x30));
								}
								if(dataSet[31] == ','){
									sats[7] = 0;
								}else{
									sats[7] = (((dataSet[31]-0x30)*10) + (dataSet[32]-0x30));
								}
								int p;
								//loops through the entire array of satellite values and builds an integer to turn on LEDs
								for(p=0; p<8; p++){
									if(sats[p]<19){
									ledWrite |= (1 << (sats[p]-1));
									}
								}
								//that value is written to the LEDs to turn the correct ones on.
								*LEDs = ledWrite;

							}
						}

					}

				}

			}

		} else {
			//if the char isn't $ move on the find the next character.
			c = getchar();
		}
	}
	return 0;
}
