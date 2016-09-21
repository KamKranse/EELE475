 #define Switches (volatile int *) 0x01001010
#define LEDs 	 (int *) 0x01001020
#include "board_diag.h"

int main()
{

  TestLCD();

  return(0);
}

static void TestLCD( void )
{
  FILE *lcd;
  
  lcd = fopen("/dev/lcd_display", "w");


  int x = -1;
  int oldX = -1;
  int switchLabel = -1;
  int oldSwitchLabel = -1;
  while(1){
  	/* Write some simple text to the LCD. */

  	switch(*Switches){
  		case 0:
  			x = -1;
  			break;
  		case 1:
  			x = 0;
  			break;
  		case 2:
  			x = 1;
  			break;
  		case 4:
  			x = 2;
  			break;
  		case 8:
  			x = 3;
  			break;
  	}

  	if(oldX != x){

  		oldX = x;
  		oldSwitchLabel = switchLabel;

  		//print first line
  		if(x < 0){
  			fprintf(lcd, "\n" );
  		}else {
  			switchLabel = x;
  			fprintf(lcd, "SW %d Active\n", switchLabel);
  		}

  		if(oldSwitchLabel >= 0){
  			fprintf(lcd, "Last SW %d\n", oldSwitchLabel);
  		} else {
  			fprintf(lcd, "\n" );
  		}
  	}
  }
}

