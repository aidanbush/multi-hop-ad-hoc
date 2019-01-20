#include <EEPROM.h>
#include <stdint.h>

#include "help.h"

#define ID_EEPROM_ADDR  0

void set_id(uint8_t id) {
    EEPROM.write(ID_EEPROM_ADDR, id);
}

uint8_t get_id() {
    return (uint8_t)EEPROM.read(ID_EEPROM_ADDR);
}
