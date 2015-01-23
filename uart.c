#include "MKL46Z4.h"

void UART_Init()
{
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	// PTA1 -> ALT3 = UART2_RX 
	// PTA2 -> ALT3 = UART2_TX
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTA->PCR[1] |= PORT_PCR_MUX(2); 
	PORTA->PCR[2] |= PORT_PCR_MUX(2);

	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);
	// Block Receiver and transmitter
	UART0->C2 = 0;
	UART0->C2 &= ~UART_C2_RE_MASK;
	UART0->C2 &= ~UART_C2_TE_MASK;
	UART0->C4 = 0;
	UART0->C4 |= UART0_C4_OSR(0x1F); // Set Over Sampling Ratio to 31
	// SBR = BusClock/(32*BR)
	// BusClock = 8MHz, BR = 9600bit/s
	// SBR = 26
	UART0->BDH = UART_BDH_SBR(0);
	UART0->BDL = UART_BDL_SBR(0x1A); // = 26
	
	UART0->BDH &= ~UART_BDH_SBNS_MASK; // One stop bit.
	UART0->C1 = 0;
	UART0->C1 &= ~UART_C1_M_MASK; // 8-bit data
	UART0->C1 &= ~UART_C1_PE_MASK; // No hardware parity generation or checking
	
	// Enable Receiver and transmitter
	UART0->C2 |= UART_C2_RE_MASK;
	UART0->C2 |= UART_C2_TE_MASK;
}

void Send(uint8_t message)
{
	// Send message
	UART0->D = message; 
	// Wait for transfer complete
	while(!(UART0->S1 & UART_S1_TDRE_MASK)){}
}

