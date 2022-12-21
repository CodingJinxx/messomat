#include <stdint.h>
#include "eeprom_status.h"

typedef struct _EepromData {
    uint16_t MajorVersion;
    uint16_t MinorVersion;
    uint16_t HeaderLength;
    uint16_t TransmitInterval;
    uint8_t Checksum;
    uint16_t Data;
} EepromData;


eEepromStatus ReadEepromData(EepromData *data);

eEepromStatus WriteEepromData(EepromData *data);