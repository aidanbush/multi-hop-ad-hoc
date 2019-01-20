#include <EEPROM.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <stdint.h>

#include "help.h"
#include "packet.h"

#define ID_EEPROM_ADDR      0

#define PACKET_REDUNDANCY   2

extern RH_ASK driver;

void set_id(uint8_t id) {
    EEPROM.write(ID_EEPROM_ADDR, id);
}

uint8_t get_id() {
    return (uint8_t)EEPROM.read(ID_EEPROM_ADDR);
}

int send_pkt(packet_s *pkt) {
    uint8_t len;
    uint8_t *buf;

    buf = encode_packet(pkt, &len);
    if (buf == NULL)
        return 0;

    // send packet
    for (int i = 0; i < PACKET_REDUNDANCY; i++) {
        driver.send(buf, len);
        driver.waitPacketSent();
    }

    return 1;
}
