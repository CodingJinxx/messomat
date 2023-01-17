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

// bits for uart stx received, data buffer and etx received
volatile uint8_t stx = 0;
volatile uint8_t uart_data[64];
volatile uint8_t uart_data_index = 0;


void uart_worker(UartWorker worker) {
      // read uart input
    unsigned int input = uart_getc();
    // get lower byte from input as data and higher byte as status
    uint8_t data = input & 0xFF;

    if (input == UART_NO_DATA) {
        return;
    }
    if (stx == 0) {
        if (data == 50) {
            stx = 1;
            // clear data buffer
            memset(uart_data, 0, sizeof(uart_data));
            uart_data_index = 0;
        }
    } else {
        if (data == 51) {
            stx = 0;
            // handle data
            if (uart_data[0] == 'q') {
                transmit_off();
            } else {
                if (bit_is_clear(status, TRANSMIT_ON)) {
                    transmit_on();
                }
            }
            switch (uart_data[0]) {
                case 'q':
                    transmit_off();
                    break;
                case 'a':
                    transmitSensors = ALL;
                    break;
                case 's':
                    transmitSensors = SWITCH;
                    break;
                case 'l':
                    transmitSensors = PHOTO_RESISTOR;
                    break;
                default:
                    break;
            }
            memset(uart_data, 0, sizeof(uart_data));
            uart_data_index = 0;
        } else {
            uart_data[uart_data_index] = data;
            uart_data_index++;
        }
    }
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