#define Switches (volatile int *) 0x5000
#define LEDs 	 (int *) 0x5010

void main()
{
	while (1){
		*LEDs = *Switches;
	}
}


