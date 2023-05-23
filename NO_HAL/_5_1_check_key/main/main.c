#include "key.h"
#include "sys.h"
#include "led.h"
#include "beep.h"

#define OPEN_BEEP 1
#define OPEN_RED_LED 2
#define OPEN_BLUE_LED 3
#define OPEN_RED_BLUE_LED 4

int main(void)
{

	System_Clock_Init();
	KEY_Init();
	LED_Init();
	Beep_Init();
	
	while(1)
	{
		uint8_t result = Scan_Key();
		switch(result)
		{
			case OPEN_BEEP:
				BEEP = BEEP_ON;
				break;	
			case OPEN_RED_LED:
				LED_RED = ON;
				break;
			case OPEN_BLUE_LED:
				LED_BLUE = ON;
				break;
			case OPEN_RED_BLUE_LED:
				LED_RED = ON;
				LED_BLUE = ON;
				break;
			default:
				LED_RED = OFF;
				LED_BLUE = OFF;
				BEEP = BEEP_OFF;

		}
	}

}