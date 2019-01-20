#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define TRANS_PACKET_SIZE sizeof(uint32_t) + sizeof(void*)

#define MNG_PKT 0
#define APP_PKT 1

typedef struct __attribute__ ((packed)) mng_pkt_s {
    uint8_t unused: 4;
    uint8_t type: 4;
    uint8_t *data;
} mng_pkt_s;

typedef struct __attribute__ ((packed)) app_pkt_s {
    uint8_t *data;
} app_pkt_s;


typedef struct __attribute__ ((packed)) packet_s {
    // header
    uint32_t hop_id: 6;
    uint32_t source_id: 6;
    uint32_t dest_id: 6;
    uint32_t ttl: 6;

    uint32_t unused: 7;
    uint32_t type: 1;
    //data
    union {
        mng_pkt_s * mng;
        app_pkt_s * app;
    };
} packet_s;

packet_s *init_packet(uint8_t source_id, uint8_t hop_id, uint8_t dest_id,
        uint8_t type, uint8_t ttl);

int next_hop(packet_s *pkt, uint8_t hop_id);

void free_packet(packet_s *pkt);

#endif /* PACKET_H */