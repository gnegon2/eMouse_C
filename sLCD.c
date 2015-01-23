#include "MKL46Z4.h"
#include "sLCD.h"

// Zmienne tworzace dwie tablice
const static uint8_t LCD_Front_Pin[LCD_N_FRONT] = 
{
  LCD_FRONT0,
  LCD_FRONT1,
	LCD_FRONT2,
	LCD_FRONT3,
	LCD_FRONT4,
	LCD_FRONT5,
	LCD_FRONT6,
	LCD_FRONT7	
};


void sLCD_init(void) 
{
	// konfiguracja zegara dla portów
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_SLCD_MASK;
	// wstepne wylaczenie i reset controlera
	LCD->GCR |= LCD_GCR_PADSAFE_MASK; //Set PADSAFE to disable LCD while configuring
	LCD->GCR &= ~LCD_GCR_LCDEN_MASK; // Clear LCDEN w trakcie konfiguracji
	// konfiguracja multiplekserów do operacji portów jako kontroler LCD
	PORTD->PCR[0]  = PORT_PCR_MUX(0u);					//Set PTD0 to LCD_P40
	PORTE->PCR[4]  = PORT_PCR_MUX(0u);					//Set PTE4 to LCD_P52
	PORTB->PCR[23] = PORT_PCR_MUX(0u);					//Set PTB23 to LCD_P19
	PORTB->PCR[22] = PORT_PCR_MUX(0u);					//Set PTB22 to LCD_P18
	PORTC->PCR[17] = PORT_PCR_MUX(0u);					//Set PTC17 to LCD_P37
	PORTB->PCR[21] = PORT_PCR_MUX(0u);					//Set PTB21 to LCD_P17
	PORTB->PCR[7]  = PORT_PCR_MUX(0u);					//Set PTB7 to LCD_P7
	PORTB->PCR[8]  = PORT_PCR_MUX(0u);					//Set PTB8 to LCD_P8
	PORTE->PCR[5]  = PORT_PCR_MUX(0u);					//Set PTE5 to LCD_P53
	PORTC->PCR[18] = PORT_PCR_MUX(0u);					//Set PTC18 to LCD_P38
	PORTB->PCR[10] = PORT_PCR_MUX(0u);					//Set PTB10 to LCD_P10
	PORTB->PCR[11] = PORT_PCR_MUX(0u);					//Set PTB11 to LCD_P11
	// konfiguracja rejestrów LCD
	LCD->GCR = //LCD_GCR_RVEN_MASK |	  //Clear LCD_GCR_RVEN, disable voltage regulator.
	LCD_GCR_RVTRIM(0x00) | 
	LCD_GCR_CPSEL_MASK | 
	LCD_GCR_LADJ(0x03) | 
	LCD_GCR_VSUPPLY_MASK |        //Clear LCD_GCR_VSUPPLY, drive VLL3 externally.
	//LCD_GCR_PADSAFE_MASK |   //Set LCD_GCR_PADSAFE, leave enabled during configuration process.
	LCD_GCR_ALTDIV(0x00) |   //Set LCD_GCR_ALTDIV to 11, divide alternate clock by 512.  This is assuming an 8MHz External Crystal is used.
	LCD_GCR_SOURCE_MASK | //Set LCD_GCR_ALTSOURCE, Part of setting clock source to OSCERCLK, or external oscillator.
	//LCD_GCR_FFR_MASK |   	      //Set LCD_GCR_FFR, allow an LCD Frame Frequency of 46.6Hz to 146.2Hz.  Disable to change range to 23.3Hz to 73.1Hz.
	LCD_GCR_SOURCE_MASK |			  //Set LCD_GCR_SOURCE, Part of setting clock source to OSCERCLK, or external oscillator.
	LCD_GCR_LCLK(0x01) |        //Set LCD_GCR_LCLK to 111, LCD Clock prescaler where LCD controller frame frequency = LCD clock/((DUTY  |  1) x 8 x (4 | LCLK[2:0]) x Y), where Y = 2, 2, 3, 3, 4, 5, 8, 16 chosen by module duty cycle config
	LCD_GCR_DUTY(0x03);         //Set LCD_GCR_DUTY to 011, Have 4 backplane pins, so need a 1/4 duty cycle.
	// konfiguracja migania wyswietlacza
	LCD->AR = LCD_AR_BRATE(0x03); //Set LCD_SEG_AR_BRATE to 000.  Frequency of blink is determined by LCD clock/(2^(12 + BRATE))
	// konfiguracja rejestru FDCR (LCD Fault Detect Control Register)
	LCD->FDCR = 0x00000000; //Clear all bits in FDCR.  As this will not be covering use of fault detect, this register is cleared.
	// aktywowanie 12 pinów do kontroli wyswietlaczem (dwa rejestry po 32 bity)
	LCD->PEN[0] =  LCD_PEN_PEN(1u<<7u) |				//LCD_P7
								 LCD_PEN_PEN(1u<<8u) |				//LCD_P8
								 LCD_PEN_PEN(1u<<10u)|				//LCD_P10
								 LCD_PEN_PEN(1u<<11u)|				//LCD_P11
								 LCD_PEN_PEN(1u<<17u)|				//LCD_P17
								 LCD_PEN_PEN(1u<<18u)|				//LCD_P18
								 LCD_PEN_PEN(1u<<19u);				//LCD_P19
	LCD->PEN[1] =  LCD_PEN_PEN(1u<<5u) |				//LCD_P37
								 LCD_PEN_PEN(1u<<6u) |				//LCD_P38
								 LCD_PEN_PEN(1u<<8u) |				//LCD_P40
								 LCD_PEN_PEN(1u<<20u)|				//LCD_P52
								 LCD_PEN_PEN(1u<<21u);				//LCD_P53
	// skonfigurowanie 4 pinów plaszczyzny tylnej (dwa rejestry po 32 bity)
	LCD->BPEN[0] = LCD_BPEN_BPEN(1u<<18u)|			//LCD_P18  COM3
								 LCD_BPEN_BPEN(1u<<19u);			//LCD_P19, COM2
	LCD->BPEN[1] = LCD_BPEN_BPEN(1u<<8u) |			//LCD_P40, COM0
								 LCD_BPEN_BPEN(1u<<20u);			//LCD_P52, COM1
	// konfiguracja rejestrów przebiegów (Waveform register) – 4 aktywne, reszta nie
	// (44.3.7 w KL46 Reference Manual)
	LCD->WF[0]  = LCD_WF_WF0(0x00) |						
							  LCD_WF_WF1(0x00) |						
							  LCD_WF_WF2(0x00) |						
							  LCD_WF_WF3(0x00) ;						
	LCD->WF[1]  = LCD_WF_WF4(0x00) |						
							  LCD_WF_WF5(0x00) |						
							  LCD_WF_WF6(0x00) |						
							  LCD_WF_WF7(0x00) ;						
	LCD->WF[2]  = LCD_WF_WF8(0x00) |						
							  LCD_WF_WF9(0x00) |						
							  LCD_WF_WF10(0x00)|						
							  LCD_WF_WF11(0x00);						
	LCD->WF[3]  = LCD_WF_WF12(0x00)|						
							  LCD_WF_WF13(0x00)|						
							  LCD_WF_WF14(0x00)|						
							  LCD_WF_WF15(0x00);						
	LCD->WF[4]  = LCD_WF_WF16(0x00)|						
							  LCD_WF_WF17(0x00)|						
							  LCD_WF_WF18(0x88)|  // COM3						
							  LCD_WF_WF19(0x44);	// COM2					
	LCD->WF[5]  = LCD_WF_WF20(0x00)|						
							  LCD_WF_WF21(0x00)|						
							  LCD_WF_WF22(0x00)|						
							  LCD_WF_WF23(0x00);						
	LCD->WF[6]  = LCD_WF_WF24(0x00)|						
							  LCD_WF_WF25(0x00)|						
							  LCD_WF_WF26(0x00)|						
							  LCD_WF_WF27(0x00);						
	LCD->WF[7]  = LCD_WF_WF28(0x00)|						
							  LCD_WF_WF29(0x00)|						
							  LCD_WF_WF30(0x00)|						
							  LCD_WF_WF31(0x00);						
	LCD->WF[8]  = LCD_WF_WF32(0x00)|						
							  LCD_WF_WF33(0x00)|						
							  LCD_WF_WF34(0x00)|						
							  LCD_WF_WF35(0x00);						
	LCD->WF[9]  = LCD_WF_WF36(0x00)|						
							  LCD_WF_WF37(0x00)|						
							  LCD_WF_WF38(0x00)|						
							  LCD_WF_WF39(0x00);						
	LCD->WF[10] = LCD_WF_WF40(0x11)|	// COM0						
							  LCD_WF_WF41(0x00)|						
							  LCD_WF_WF42(0x00)|						
							  LCD_WF_WF43(0x00);						
	LCD->WF[11] = LCD_WF_WF44(0x00)|						
							  LCD_WF_WF45(0x00)|						
							  LCD_WF_WF46(0x00)|	 				
							  LCD_WF_WF47(0x00);						
	LCD->WF[12] = LCD_WF_WF48(0x00)|						
							  LCD_WF_WF49(0x00)|						
							  LCD_WF_WF50(0x00)|						
							  LCD_WF_WF51(0x00);						
	LCD->WF[13] = LCD_WF_WF52(0x22)|	// COM1						
							  LCD_WF_WF53(0x00)|						
							  LCD_WF_WF54(0x00)|						
							  LCD_WF_WF55(0x00);						
	LCD->WF[14] = LCD_WF_WF56(0x00)|						
							  LCD_WF_WF57(0x00)|						
							  LCD_WF_WF58(0x00)|						
							  LCD_WF_WF59(0x00);						
	LCD->WF[15] = LCD_WF_WF60(0x00)|						
							  LCD_WF_WF61(0x00)|	 				
							  LCD_WF_WF62(0x00)|						
							  LCD_WF_WF63(0x00);						
	
	// koniec konfiguracji, wiec clear PADSAFE i wlaczenie wyswietlacza
	LCD->GCR &= ~LCD_GCR_PADSAFE_MASK; //Clear PADSAFE to unlock LCD pins
	LCD->GCR |= LCD_GCR_LCDEN_MASK; // wlaczenie wyswietlacza
}

void sLCD_set(uint8_t value,uint8_t digit)
{
	if(digit > 4)
	{
		SegLCD_DisplayError(0x01);
	}
	else
	{
		if(value==0x00)			
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_D | LCD_S_E |LCD_S_F); 
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			(LCD_S_A | LCD_S_B | LCD_S_C);
		}
		else if(value==0x01)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] =
			(LCD_C); 
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			(LCD_S_B | LCD_S_C);
		}
		else if(value==0x02)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_D | LCD_S_E | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			(LCD_S_A | LCD_S_B);
		}
		else if(value==0x03)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_D | LCD_S_G); 
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			(LCD_S_A | LCD_S_B | LCD_S_C);
		}
		else if(value==0x04)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_F | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] =
			(LCD_S_B | LCD_S_C);
		}
		else if(value==0x05)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_D | LCD_S_F | LCD_S_G); 
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] =
			(LCD_S_A | LCD_S_C);
		}
		else if(value==0x06)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] =
			(LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] =
			(LCD_S_A | LCD_S_C);
		}
		else if(value==0x07)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] =
			(LCD_C); 
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			(LCD_S_A | LCD_S_B | LCD_S_C);
		}
		else if(value==0x08)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G); 
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			(LCD_S_A | LCD_S_B | LCD_S_C);
		}
		else if(value==0x09)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] =
			(LCD_S_F | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			(LCD_S_A | LCD_S_B | LCD_S_C);
		}
		else if(value==0x0A)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_E | LCD_S_F | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			(LCD_S_A | LCD_S_B | LCD_S_C);
		}
		else if(value==0x0B)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			LCD_S_C;
		}
		else if(value==0x0C)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] = 
			(LCD_S_D | LCD_S_E | LCD_S_F);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			LCD_S_A;
		}
		else if(value==0x0D)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] =
			(LCD_S_D | LCD_S_E | LCD_S_G); 
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			LCD_S_B | LCD_S_C;
		}
		else if(value==0x0E)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] =
			(LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] =
			LCD_S_A;
		}
		else if(value==0x0F)
		{
			LCD->WF8B[LCD_Front_Pin[((2*digit)-2)]] =
			(LCD_S_E | LCD_S_F | LCD_S_G);
			LCD->WF8B[LCD_Front_Pin[((2*digit)-1)]] = 
			LCD_S_A;
		}
		else
		{
			SegLCD_DisplayError(0x02);
		}
	}
}

void SegLCD_DisplayDecimal(uint16_t Value)
{
		if(Value > 9999)
		{
			SegLCD_DisplayError(0x10); //Display "Err" if value is greater than 4 digits
		}
		else
		{
			sLCD_set(Value/1000,1);
			sLCD_set((Value - (Value/1000)*1000)/100,2);
			sLCD_set((Value - (Value/100)*100)/10,3);
			sLCD_set(Value - (Value/10)*10,4);
		}
}

void SegLCD_DisplayBinary(uint16_t Value)
{
		if(Value > 15)
		{
			SegLCD_DisplayError(0x10); //Display "Err" if value is greater than 4 digits
		}
		else
		{
			sLCD_set(Value/8,1);
			sLCD_set((Value%8)/4,2);
			sLCD_set((Value%4)/2,3);
			sLCD_set(Value%2,4);
		}
}


//Displays a 16 bit number in Hex Format
void SegLCD_DisplayHex(uint16_t Value)
{
	sLCD_set((Value & 0xF000)>>12, 1);
	sLCD_set((Value & 0x0F00)>>8 , 2);
	sLCD_set((Value & 0x00F0)>>4 , 3);
	sLCD_set((Value & 0x000F)>>0 , 4);
}

void SegLCD_DisplayOctal(uint16_t Value)
{
	sLCD_set(Value/512, 1);
	sLCD_set((Value%512)/64, 2);
	sLCD_set((Value%64)/8, 3);
	sLCD_set(Value%8, 4);
}

void SegLCD_DisplayTime(uint8_t Value1, uint8_t Value2)
{
		if((Value1 > 99) | (Value2 > 99))
		{
			SegLCD_DisplayError(0x10); //Display "Err" if either value is greater than 2 digits
		}
		else
		{
			sLCD_set(Value1/10, 1);
			sLCD_set(Value1 % 10, 2);
			sLCD_set(Value2/10, 3);
			sLCD_set(Value2 % 10, 4);
			SegLCD_Col_On();
		}
}

void SegLCD_DisplayError(uint8_t ErrorNum)
{
	LCD->WF8B[LCD_FRONT0] = (LCD_S_D | LCD_S_E | LCD_S_F | LCD_S_G);
	LCD->WF8B[LCD_FRONT1] = (LCD_S_A);
	LCD->WF8B[LCD_FRONT2] = (LCD_S_E | LCD_S_G);
	LCD->WF8B[LCD_FRONT3] = (LCD_C);
	LCD->WF8B[LCD_FRONT4] = (LCD_S_E | LCD_S_G);
	LCD->WF8B[LCD_FRONT5] = (LCD_C);
	if(ErrorNum < 0x10)
		sLCD_set(ErrorNum,4); //Display ErrorNum in digit 4 if within valid range.  If not, leave blank.
	else
	{
		LCD->WF8B[LCD_FRONT6] = (LCD_C);
		LCD->WF8B[LCD_FRONT7] = (LCD_C);
	}
}

