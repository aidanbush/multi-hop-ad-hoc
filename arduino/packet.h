#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define TRANS_PACKET_SIZE   sizeof(uint32_t) + sizeof(void*)

#define MNG_PKT             0
#define APP_PKT             1

#define PACKET_HEADER_LEN   4
#define MNG_PKT_HEADER_LEN  1

typedef struct __attribute__ ((packed)) mng_pkt_s {
    uint8_t unused: 4;
    uint8_t type: 4;
    uint8_t *data;
    uint8_t data_len;
} mng_pkt_s;

typedef struct __attribute__ ((packed)) app_pkt_s {
    uint8_t *data;
    uint8_t data_len;
} app_pkt_s;

typedef struct __attribute__ ((packed)) header_s {
    uint32_t hop_id: 6;
    uint32_t source_id: 6;
    uint32_t dest_id: 6;
    uint32_t ttl: 6;

    uint32_t unused: 7;
    uint32_t type: 1;
} header_s;

typedef struct __attribute__ ((packed)) packet_s {
    // header
    union {
        uint32_t header;
        header_s fields;
    };
    //data
    union {
        mng_pkt_s * mng;
        app_pkt_s * app;
    };
    // TODO add checksums
} packet_s;

packet_s *init_packet(uint8_t source_id, uint8_t hop_id, uint8_t dest_id,
        uint8_t type, uint8_t ttl);

int next_hop(packet_s *pkt, uint8_t hop_id);

void free_packet(packet_s *pkt);

#ifdef _TEST
mng_pkt_s *init_mng_pkt(uint8_t type, uint8_t *data, int len);
#endif /* _TEST */

uint8_t *encode_packet(packet_s *pkt, uint8_t *len);

packet_s *decode_pkt(uint8_t *buf, uint8_t buf_len);

#endif /* PACKET_H */
