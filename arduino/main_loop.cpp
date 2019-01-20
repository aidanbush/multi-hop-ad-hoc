#include <Arduino.h>
#include <stdint.h>

#include "main_loop.h"
//#include "packet.h"
#include "help.h"

#define SECOND                  1000
#define TABLE_UPDATE_SEP_TIME   (5 * (SECOND))

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

//rx
