#include <stdio.h>

int main(void){
	char name[32] = { 0 }; /* defines a char array with 32 bytes memory and initializes it to zero */
	
	scanf("%32s", name);
	printf("hello %s", name);
	return 0;
}