#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "packet.h"

// init packet
packet_s *init_packet(uint8_t source_id, uint8_t hop_id, uint8_t dest_id,
        uint8_t type, uint8_t ttl) {
    packet_s *pkt = malloc(sizeof(packet_s));
    if (pkt == NULL)
        return NULL;

    pkt->fields.source_id = source_id;
    pkt->fields.hop_id = hop_id;
    pkt->fields.dest_id = dest_id;
    pkt->fields.ttl = ttl;

    pkt->fields.unused = 0;
    pkt->fields.type = type;

    pkt->mng = NULL;

    return pkt;
}

#ifndef _TEST
static
#endif /* _TEST */
mng_pkt_s *init_mng_pkt(uint8_t type, uint8_t *data, int len) {
    mng_pkt_s *pkt = malloc(sizeof(mng_pkt_s));
    if (pkt == NULL)
        return NULL;

    pkt->unused = 0;
    pkt->type = type;
    pkt->data = NULL;
    pkt->data_len = 0;

    if (data != NULL && len != 0) {
        uint8_t *new_data = malloc(sizeof(uint8_t) * len);
        if (new_data == NULL) {
            free(pkt);
            return NULL;
        }

        memcpy(new_data, data, len);
        pkt->data = new_data;
        pkt->data_len = len;
    }

    return pkt;
}

static mng_pkt_s *decode_mng(uint8_t *data, uint8_t len) {
    if (data == NULL)
        return NULL;

    // mng headder
    uint8_t type = data[0] & 0xf;

    return init_mng_pkt(type, data + MNG_PKT_HEADER_LEN,
            len - MNG_PKT_HEADER_LEN);
}

packet_s *decode_pkt(uint8_t *buffer, uint8_t len) {
    if (len < PACKET_HEADER_LEN) {
        return NULL;
    }

    // decode header
    uint8_t hop = (buffer[0] & 0xfc) >> 2;
    uint8_t src = ((buffer[0] & 0x3) << 4) | ((buffer[1] & 0xf0) >> 4);
    uint8_t dest = ((buffer[1] & 0xf) << 2) | ((buffer[2] & 0xc0) >> 6);
    uint8_t ttl = buffer[2] & 0x3f;
    uint8_t type = buffer[3] & 1;

    packet_s *pkt = init_packet(src, hop, dest, type, ttl);

    // decode rest
    switch (type) {
        case MNG_PKT:
            pkt->mng = decode_mng(buffer + PACKET_HEADER_LEN, len - PACKET_HEADER_LEN);
            break;
        case APP_PKT:
#ifdef _TEST
            fprintf(stderr, "not implemented\n");
#endif /* _TEST */
            break;
        default:
            // error
            break;
    }

    return pkt;
}

static int encode_header(packet_s *pkt, uint8_t *buf) {
    if (pkt == NULL || buf == NULL)
        return 0;
    //hop id
    buf[0] = pkt->fields.hop_id << 2;

    //source id
    buf[0] |= (pkt->fields.source_id & 0x30) >> 4;
    buf[1] = (pkt->fields.source_id & 0xf) << 4;

    //dest id
    buf[1] |= (pkt->fields.dest_id & 0x3c) >> 2;
    buf[2] = (pkt->fields.dest_id & 0x3) << 6;

    //ttl
    buf[2] |= pkt->fields.ttl & 0x3f;

    //type
    buf[3] = pkt->fields.type & 1;

    return 1;
}

static int mng_pkt_total_size(mng_pkt_s *pkt) {
    if (pkt == NULL)
        return -1;

    return PACKET_HEADER_LEN + MNG_PKT_HEADER_LEN + pkt->data_len;
}

static int mng_encode_header(mng_pkt_s *pkt, uint8_t *buf) {
    if (pkt == NULL || buf == NULL)
        return 0;

    buf[0] = pkt->type & 0xf;

    return 1;
}

static uint8_t *encode_mng_pkt(packet_s *pkt, uint8_t *size) {
    // calculate size
    int len = mng_pkt_total_size(pkt->mng);
    if (len < 0) {
        *size = 0;
        return NULL;
    }

    *size = len;

    // create buffer
    uint8_t *buf = calloc(sizeof(uint8_t), *size);
    if (buf == NULL)
        return NULL;

    // trans header
    if (!encode_header(pkt, buf)) {
        free(buf);
        return NULL;
    }

    // mng headder
    if (!mng_encode_header(pkt->mng, buf + PACKET_HEADER_LEN)) {
        free(buf);
        return NULL;
    }

    // mng data
    memcpy(buf + PACKET_HEADER_LEN + MNG_PKT_HEADER_LEN, pkt->mng->data,
            pkt->mng->data_len);

    return buf;
}

static uint8_t *encode_app_pkt(packet_s *pkt, uint8_t *size) {
    // get packet size

    // create buffer

    // trans header

    // app data
}

uint8_t *encode_packet(packet_s *pkt, uint8_t *len) {
    uint8_t *buf = NULL;

    switch (pkt->fields.type) {
        case MNG_PKT:
            buf = encode_mng_pkt(pkt, len);
            break;
        case APP_PKT:
#ifdef _TEST
            fprintf(stderr, "not implemented\n");
#endif /* _TEST */
            break;
        default:
            // error
            break;
    }

    return buf;
}

// add_hop decrement ttl returns new ttl, or -1 on NULL pkt or ttl <= 0
int next_hop(packet_s *pkt, uint8_t hop_id) {
    if (pkt == NULL)
        return -1;

    if (pkt->fields.ttl <= 0)
        return -1;

    pkt->fields.hop_id = hop_id;
    return --(pkt->fields.ttl);
}

static void free_mng_pkt(mng_pkt_s *pkt) {
    if (pkt == NULL)
        return;

    free(pkt->data);
    free(pkt);
}

static void free_app_pkt(app_pkt_s *pkt) {
    if (pkt == NULL)
        return;

    free(pkt->data);
    free(pkt);
}

void free_packet(packet_s *pkt) {
    if (pkt == NULL)
        return;

    if (pkt->mng != NULL) {
        switch (pkt->fields.type) {
            case MNG_PKT:
                free_mng_pkt(pkt->mng);
                break;
            case APP_PKT:
                free_app_pkt(pkt->app);
                break;
            default:
                // error TODO handle?
                break;
        }
    }

    free(pkt);
}
