#include <stdint.h>
#include "eeprom_status.h"

typedef struct _EepromData {
    uint8_t size;
    uint8_t MajorVersion;
    uint8_t MinorVersion;
    uint8_t HeaderLength;
    uint8_t TransmitInterval;
    uint8_t CRC;
} EepromData;


eEepromStatus ReadEepromData(EepromData *data);

eEepromStatus WriteEepromData(EepromData *data);