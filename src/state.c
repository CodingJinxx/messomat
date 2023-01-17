#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "state.h"

void transmit_off();
void transmit_on();
void power_on();
void power_off();

volatile uint8_t transmitSensors = 0;
volatile uint8_t buttons = 0;
volatile uint8_t status = 0;

void processStatemachine() {
    if(bit_is_set(state, ON)) {
        lcd_home();
        lcd_puts("RUNNING...");
        lcd_gotoxy(0, 1);
        lcd_puts(getDigitalInput());

        handleUartInput();

        if (bit_is_set(status, TRANSMIT)) {
            transmit();
            status &= ~(1 << TRANSMIT);
        }

        if (bit_is_set(buttons, ON_OFF_BTN)) {
            power_off();
            buttons &= ~(1 << ON_OFF_BTN);
        }
        if (bit_is_set(buttons, TRANSMIT_INTERVAL_BTN)) {
            // read transmitAll interval from last two bits of status byte
            uint8_t transmit_interval = myversionControl.TransmitInterval;
            if ((transmit_interval == 3) &&
                bit_is_set(status, TRANSMIT_ON)) { // if transmit interval is 3 and transmitAll is on
                transmit_off();
            } else if (bit_is_clear(status, TRANSMIT_ON)) { // if transmit interval is 3 and transmitAll is off
                transmit_on();
                transmit_interval = 0;
            } else {
                transmit_interval++;
            }

            char tint[20];
            if (bit_is_set(status, TRANSMIT_ON)) {
                myversionControl.TransmitInterval = transmit_interval;
                eeprom_save();
                /*status &= ~(1 << TRANSMIT_INTERVAL_L | 1 << TRANSMIT_INTERVAL_H); // clear last two bits
                status |= transmit_interval; // set last two bits to transmitAll interval*/

                uint8_t seconds = transmit_interval + 1; // add 1 to get the actual seconds
                OCR1A = timerOneSec * seconds; // set timer to new interval

                lcd_say(itoa(seconds, tint, 10));
            }
            buttons &= ~(1 << TRANSMIT_INTERVAL_BTN);
    }
}

void transmit_off() {
    lcd_say("transmit off");
    TIMSK1 &= ~(1 << OCIE1A); // Enable CTC interrupt
    status &= ~(1 << TRANSMIT_ON);
}

void transmit_on() {
    lcd_say("transmit on");
    TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
    status |= (1 << TRANSMIT_ON);
}

void power_on() {
    lcd_say("Hello");
    transmit_on();
    status |= (1 << ON);
}

void power_off() {
    lcd_say("Goodbye");
    transmit_off();
    status &= ~(1 << ON);
}
