#include "MKL46Z4.h"
#include "buttons.h"
#include "sLCD.h"
#include "uart.h"
#include "MMA8451.h"
#include "cstdio"

int main()
{
	uint32_t counter;
	uint8_t switches; 
	uint8_t xy[2] = {0,0}; // We need only X and Y coordinates
	char message[8];
	uint8_t field;
	// Buttons initialize
	Buttons_Init();
	// LCD initialize
	sLCD_init();
	// UART0 initialize in order to communicate with PC
	UART_Init();
	// MMA8451Q accelerometer initialize
	MMA8451_Init();	
	while(1)
	{	
		// Read XY
		MMA8451_Read(&xy[0]);
		// Read switches values
		Switches_Read(&switches);
		// Create frame
		sprintf(message,":%03i%03i%01i",xy[0],xy[1],switches);
		// Send "frame"
		for( field = 0; field < 8; field++ )
			Send( message[field] );
		// Delay
		for( counter = 0; counter < 100000; counter++ );	
	}
}
