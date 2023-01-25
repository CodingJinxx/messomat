#include <stdlib.h>
#include <stdint.h>
#include "client_lcd.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "adc_access.h"
#include "eeprom.h"
#define USE_SIMPLE

#ifdef USE_SIMPLE 
static uint16_t data = 19;
static uint16_t last_displayed_data;


void ButtonOne() {
    data = rand();
}

void ButtonTwo() {
    eeprom_write_word((uint16_t *) 1, data);
}

void ButtonThree() {
    data = eeprom_read_word((uint16_t *) 1);
}
#else 
static EepromData data = {10, 0, 0, 0, 0, 0};
static EepromData last_displayed_data = {10,1,1,1,1,0};
static LCD *lcd;
static int i = 0;

void init() {
    lcd = Initialize();
}

void ButtonOne() {
    ClearScreen(lcd);
    char tmp[8];
    float result = ReadADC(ADC_5);
    dtostrf(result,5,3,tmp);
    PutString(lcd, tmp);
}

void ButtonTwo() {
    eEepromStatus result = WriteEepromData(&data);
    if(result == CHECK_FAILED)
        PutString(lcd, "CHECK FAILED");
    else 
        PutString(lcd, "CHECK SUCCESS");
}

void ButtonThree() {
    ReadEepromData(&data);
    char tmp[32];
    itoa(data.CRC, tmp, 10);
    PutString(lcd, tmp);
}
#endif

void processDisplay() {
    // if(data.CRC != last_displayed_data.CRC) {
    //     lcd_clrscr();
    //     char tmp[32];
    //     itoa(data.CRC, tmp, 10);
    //     lcd_puts(tmp);
    //     last_displayed_data = data;
    // }
}

