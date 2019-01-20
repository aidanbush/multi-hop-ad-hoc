#include <EEPROM.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <stdint.h>

#include "packet.h"
#include "routing_tables.h"
#include "help.h"
#include "options.h"
#include "main_loop.h"

RH_ASK driver;

unsigned long next;
uint8_t id = 1;
route_table *routing_table;

void setup() {
    set_id(DEFAULT_ID);
    id = get_id();

    routing_table = init_routing_table(id);

    Serial.begin(BAUD_RATE);
    if (!driver.init())
        Serial.println("init failed");
}

void loop() {
    // check if need to update routing table
    check_update_table();

    // if serial deal with

    // if rx deal with
}
