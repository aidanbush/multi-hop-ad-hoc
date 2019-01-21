#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <stdint.h>

#include "main_loop.h"
//#include "packet.h"
#include "help.h"

#define SECOND                  1000
#define TABLE_UPDATE_SEP_TIME   (5 * (SECOND))

extern RH_ASK driver;
extern route_table *routing_table;

//check of send req
void check_update_table() {
    static uint32_t next = 0;
    static uint8_t overflow = 0;
    uint32_t time = millis();

    // if timer elapsed and timer watch out for timer overflow!!!
    if (time >= next && (!overflow || time - next <= next)) {
        packet_s *pkt = create_table_request();
        send_pkt(pkt);
        free_packet(pkt);

        if (next > next + TABLE_UPDATE_SEP_TIME)
            overflow = 1;
        else
            overflow = 0;
        next += TABLE_UPDATE_SEP_TIME;
    }
}

//serial

void handle_pkt(packet_s *pkt) {
    // if not for me ignore
    if ((pkt->hop != 0 || pkt->dest != 0) && (pkt->hop != id && pkt->dest != id))
    // if duplicate ignore
    // add packet to duplicate buffer

    if (pkt->dest == id) {
    } else {
        // forward
        // get new hop
        get_next_hop(table, pkt->dest);
        // send
    }
}

//rx
void check_rx() {
    uint8_t buf[MAX_PACKET_LEN];
    uint8_t buflen;

    while (driver.recv(buf, &buflen)) {
        packet_s *pkt = decode_pkt(buf, buflen);
        handle_pkt(pkt);
        free_packet(pkt);
    }
}
