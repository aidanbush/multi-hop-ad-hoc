#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "packet.h"

// init packet
packet_s *init_packet(uint8_t source_id, uint8_t hop_id, uint8_t dest_id,
        uint8_t type, uint8_t ttl) {
    packet_s *pkt = malloc(sizeof(packet_s));
    if (pkt == NULL)
        return NULL;

    pkt->source_id = source_id;
    pkt->hop_id = hop_id;
    pkt->dest_id = dest_id;
    pkt->ttl = ttl;

    pkt->unused = 0;
    pkt->type = type;

    pkt->mng = NULL;

    return pkt;
}

// add_hop decrement ttl returns new ttl, or -1 on NULL pkt or ttl <= 0
int next_hop(packet_s *pkt, uint8_t hop_id) {
    if (pkt == NULL)
        return -1;

    if (pkt->ttl <= 0)
        return -1;

    pkt->hop_id = hop_id;
    return pkt->ttl--;
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
        switch (pkt->type) {
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
