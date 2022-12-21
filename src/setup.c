#include "setup.h"
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <uart.h>
#include "client_lcd.h"
#include "button.h"

#ifndef F_CPU
#error "F_CPU undefined, please define CPU frequency in Hz in Makefile"
#endif
#define UART_BAUD_RATE      9600

void _setup_lcd();
void _setup_buttons();
void _setup_interrupt();
void _setup_uart();
void _setup_uart_worker();
void _setup_adc();
void _setup_temp_sensor();

void setup() {
    _setup_lcd();
    _setup_buttons();
    _setup_interrupt();    
    _setup_uart();
    _setup_uart_worker();
    _setup_adc();
    _setup_temp_sensor();
}

void _setup_lcd() {
    init();
}

void _setup_buttons() {
    // Internal Pull up on all three buttons
    BUTTON_DDR |= (1 << BUTTON_0) | (1 << BUTTON_1) | (1 << BUTTON_2);
    BUTTON_PORT |= (1 << BUTTON_0) | (1 << BUTTON_1) | (1 << BUTTON_2);
}

void _setup_interrupt() {
    cli();  
    
    // Enable Pin Change Interrupt
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2);

    sei();
}

void _setup_uart() {
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));  
}

void _setup_uart_worker() {
    TCCR2A = (1 << WGM21); // CTC Mode
	TCCR2B = (1 << CS22) | (1 << CS21); // Prescaler = 256
	TIMSK2 = (1 << OCIE2A); // Interrupt when Compare OCR2A
	OCR2A = 124;
	DDRD |= (1 << PORTD7);
}

void _setup_adc() {
       // Select Vref=AVcc
    ADMUX |= (1<<REFS0);
    //set prescaller to 128 and enable ADC  
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);    
}

void _setup_temp_sensor() {

}