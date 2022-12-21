#include <avr/pgmspace.h>

#define SCALEFACTOR 50
#define VREF 5.0

#define LCD_RS PORTC0
#define LCD_RW PORTC1
#define LCD_E PORTC2
#define LCD_DATA0 PORTD4 
#define LCD_DATA1 PORTD5 
#define LCD_DATA2 PORTD6 
#define LCD_DATA3 PORTD7

#define BUTTON_DDR DDRB
#define BUTTON_PORT PORTB
#define BUTTON_0 PINB0
#define BUTTON_1 PINB1
#define BUTTON_2 PINB2

#define TEMP_PORT PORTC
#define TEMP_PIN PORTC5