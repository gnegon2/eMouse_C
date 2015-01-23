#include "MKL46Z4.h"

// zdefiniowanie ilosci pinów na poszczególnych plaszczyznach
#define LCD_N_FRONT 8
#define LCD_N_BACK 4
// Makra dla segmentów, kazdy znak sterowany jest przez dwa piny
// Pin 1 -> (Digit*2 - 1), Pin 2 -> Digit*2
#define LCD_S_D 0x11
#define LCD_S_E 0x22
#define LCD_S_G 0x44
#define LCD_S_F 0x88
#define LCD_S_DEC 0x11
#define LCD_S_C 0x22
#define LCD_S_B 0x44
#define LCD_S_A 0x88
#define LCD_C 0x00 // clear
// Makra dla kazdego pinu
#define LCD_FRONT0 37u
#define LCD_FRONT1 17u
#define LCD_FRONT2 7u
#define LCD_FRONT3 8u
#define LCD_FRONT4 53u
#define LCD_FRONT5 38u
#define LCD_FRONT6 10u
#define LCD_FRONT7 11u
#define LCD_BACK0 40u
#define LCD_BACK1 52u
#define LCD_BACK2 19u
#define LCD_BACK3 18u

#define LCD_SEG_DECIMAL 0x11

//Macros for turning decimal points and colon on and off
#define SegLCD_DP1_On() LCD->WF8B[LCD_FRONT1]	 |= LCD_SEG_DECIMAL;
#define SegLCD_DP1_Off() LCD->WF8B[LCD_FRONT1] &= ~LCD_SEG_DECIMAL;
#define SegLCD_DP2_On() LCD->WF8B[LCD_FRONT3]	 |= LCD_SEG_DECIMAL;
#define SegLCD_DP2_Off() LCD->WF8B[LCD_FRONT3] &= ~LCD_SEG_DECIMAL;
#define SegLCD_DP3_On() LCD->WF8B[LCD_FRONT5]	 |= LCD_SEG_DECIMAL;
#define SegLCD_DP3_Off() LCD->WF8B[LCD_FRONT5] &= ~LCD_SEG_DECIMAL;
#define SegLCD_Col_On() LCD->WF8B[LCD_FRONT7]	 |= LCD_SEG_DECIMAL;
#define SegLCD_Col_Off() LCD->WF8B[LCD_FRONT7] &= ~LCD_SEG_DECIMAL;

void sLCD_init(void);
void SegLCD_DisplayTime(uint8_t Value1, uint8_t Value2);
void SegLCD_DisplayError(uint8_t ErrorNum);
void SegLCD_DisplayHex(uint16_t Value);


void sLCD_init(void) ;
void sLCD_set(uint8_t value,uint8_t digit);
void SegLCD_DisplayDecimal(uint16_t Value);
void SegLCD_DisplayBinary(uint16_t Value);
//Displays a 16 bit number in Hex Format
void SegLCD_DisplayHex(uint16_t Value);
void SegLCD_DisplayOctal(uint16_t Value);
void SegLCD_DisplayTime(uint8_t Value1, uint8_t Value2);
//Displays Err# on screen, where # is a value 0-F.  If ErrorNum is outside of that range, just displays Err
void SegLCD_DisplayError(uint8_t ErrorNum);
