#include <avr/eeprom.h>
#include "eeprom.h"

eEepromStatus ReadEepromData(EepromData *data) {
    eeprom_read_block(data, 0, sizeof(EepromData));
    
    if(crc_8(&data, sizeof(data)) == 0) {
        return CHECK_SUCCESS;
    } else {
        return CHECK_FAILED;
    }
}

eEepromStatus WriteEepromData(EepromData *data) {
    (*data).CRC = crc_8(&data, sizeof(data) - 1);

    if(crc_8(&data, sizeof(data)) == 0){
        return CHECK_SUCCESS;
    }

    return CHECK_FAILED;
}