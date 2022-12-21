#include <uart.h>
#include "config.h"
#include <lcd.h>
#include "adc_access.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#define CHAR_BUFF_SIZE 64

typedef void (*Callback)(char);

typedef struct UartWorker {
    Callback callback; 
} UartWorker;

void uart_worker(UartWorker worker) {
    int c = uart_getc();

    if ( c & UART_NO_DATA ) {

    }
    else {
        if ( c & UART_FRAME_ERROR ) {
            uart_puts_P("UART FRAME ERROR");
        }
        if ( c & UART_OVERRUN_ERROR ) {
            uart_puts_P("UART OVERRUN ERROR");
        }
        if ( c & UART_BUFFER_OVERFLOW ) {
            uart_puts_P("UART BUFFER OVERFLOW");
        }
        // char * buffer;
        // lcd_clrscr();
        // int d = c;
        // sprintf(buffer, "%d", c);
        // lcd_puts(c);
        worker.callback((char)c);
    }
}

static char * _float_to_char(float x, char *p) {
    char *s = p + CHAR_BUFF_SIZE; // go to end of buffer
    uint16_t decimals;  // variable to store the decimals
    int units;  // variable to store the units (part to left of decimal place)
    if (x < 0) { // take care of negative numbers
        decimals = (int)(x * -100) % 100; // make 1000 for 3 decimals etc.
        units = (int)(-1 * x);
    } else { // positive numbers
        decimals = (int)(x * 100) % 100;
        units = (int)x;
    }

    *--s = (decimals % 10) + '0';
    decimals /= 10; // repeat for as many decimal places as you need
    *--s = (decimals % 10) + '0';
    *--s = '.';

    while (units > 0) {
        *--s = (units % 10) + '0';
        units /= 10;
    }
    if (x < 0) *--s = '-'; // unary minus sign for negative numbers
    return s;
}

void UartHandler(char c) {
    lcd_clrscr();
    if ( c & UART_FRAME_ERROR ) {
        lcd_puts("UART FRAME ERROR");
    }
    if ( c & UART_OVERRUN_ERROR ) {
        lcd_puts("UART OVERRUN ERROR");
    }
    if ( c & UART_BUFFER_OVERFLOW ) {
        lcd_puts("UART BUFFER OVERFLOW");
    }
    else {
        if(c == 'x') {
            float f = ReadADC(ADC_3);
            char *buffer;
            buffer = _float_to_char(f, buffer);
            lcd_puts(buffer);
            uart_putc(c);
        }
    }
}

static UartWorker worker = {
    callback: UartHandler
};


ISR(TIMER2_COMPA_vect) {
	static int counter = 0;
	counter++;
	if(counter == SCALEFACTOR) {
        uart_worker(worker);
		counter = 0;
	}
}