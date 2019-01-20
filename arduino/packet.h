#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define MAX_PACKET_LEN      64

#define TRANS_PACKET_SIZE   sizeof(uint32_t) + sizeof(void*)

#define MNG_PKT             0
#define APP_PKT             1

#define PACKET_HEADER_LEN   4
#define PACKET_TAIL_LEN     0
#define MNG_PKT_HEADER_LEN  1

#define MNG_DATA_MAX_LEN    (MAX_PACKET_LEN - (PACKET_HEADER_LEN \
        + PACKET_TAIL_LEN + MNG_PKT_HEADER_LEN))

#define BROADCAST_ID        0

#define TABLE_REQ_TYPE      0

#define TABLE_REQ_TTL       1
#define TABLE_REQ_LEN       0

#define TABLE_RES_TYPE      1

#define TABLE_RES_TTL       1
#define TABLE_RES_MAX_LEN   (MNG_DATA_MAX_LEN - (MNG_DATA_MAX_LEN % 2))

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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

packet_s *init_packet(uint8_t source_id, uint8_t hop_id, uint8_t dest_id,
        uint8_t type, uint8_t ttl);

int next_hop(packet_s *pkt, uint8_t hop_id);

void free_packet(packet_s *pkt);

#ifdef _TEST
mng_pkt_s *init_mng_pkt(uint8_t type, uint8_t *data, int len);
#endif /* _TEST */

uint8_t *encode_packet(packet_s *pkt, uint8_t *len);

packet_s *decode_pkt(uint8_t *buf, uint8_t buf_len);

// Management packets

packet_s *create_table_request();

packet_s *create_table_response(uint8_t dest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PACKET_H */
