#include "MKL46Z4.h"                    /* Device header */
#include "buttons.h"

#define SW1_PIN 3												
#define SW3_PIN 12

void Buttons_Init(void)
{	
	// Enable clock for port C
	SIM->SCGC5 |=  SIM_SCGC5_PORTC_MASK;
	// Set PTC3 and PTC12 as GPIO
	PORTC->PCR[SW1_PIN] |= PORT_PCR_MUX(1);      	
	PORTC->PCR[SW3_PIN] |= PORT_PCR_MUX(1); 
	// Enable pull up resistors and select pull up on PCR[3] and PCR[12]
	PORTC->PCR[SW1_PIN] |=  PORT_PCR_PE_MASK |		
							PORT_PCR_PS_MASK;		
	PORTC->PCR[SW3_PIN] |=  PORT_PCR_PE_MASK |	
							PORT_PCR_PS_MASK;		
}

int8_t SW1_State()
{
	if( ( FPTC->PDIR & ( 1 << SW1_PIN ) ) >> SW1_PIN == 0 )
		return 1;						
	else
		return 0;
}	

int8_t SW3_State()
{
	if( ( FPTC->PDIR & ( 1 << SW3_PIN ) ) >> SW3_PIN == 0 )
		return 1;						
	else
		return 0;
}	

void Switches_Read(uint8_t *switches)
{
	// First bit set to SW1 State, second bit set to SW3 State
	*switches = SW1_State() | (SW3_State() << 1);
}


