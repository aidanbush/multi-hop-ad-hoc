#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "packet.h"

void test_init_packet() {
    uint8_t src = 0, hop = 0, dest = 0, ttl = 0, type = 0;

    // all low 0 test
    packet_s *pkt = init_packet(src, hop, dest, type, ttl);

    assert(pkt->fields.source_id == src);
    assert(pkt->fields.hop_id == hop);
    assert(pkt->fields.dest_id == dest);
    assert(pkt->fields.ttl == ttl);
    assert(pkt->fields.unused == 0);
    assert(pkt->fields.type == type);
    assert(pkt->mng == NULL);

    free_packet(pkt);

    // all max test
    src = 0x3f;
    hop = 0x3f;
    dest = 0x3f;
    ttl = 0x3f;
    type = 1;

    pkt = init_packet(src, hop, dest, type, ttl);

    assert(pkt->fields.source_id == src);
    assert(pkt->fields.hop_id == hop);
    assert(pkt->fields.dest_id == dest);
    assert(pkt->fields.ttl == ttl);
    assert(pkt->fields.unused == 0);
    assert(pkt->fields.type == type);
    assert(pkt->mng == NULL);

    free_packet(pkt);

    // over limit
    src = 0x40;
    hop = 0x40;
    dest = 0x40;
    ttl = 0x40;
    type = 2;

    pkt = init_packet(src, hop, dest, type, ttl);

    assert(pkt->fields.source_id == src % 0x40);
    assert(pkt->fields.hop_id == hop % 0x40);
    assert(pkt->fields.dest_id == dest % 0x40);
    assert(pkt->fields.ttl == ttl % 0x40);
    assert(pkt->fields.unused == 0);
    assert(pkt->fields.type == type % 2);
    assert(pkt->mng == NULL);

    free_packet(pkt);
}

void test_next_hop() {
    // test w/ valid ttl
    uint8_t src = 1, hop = 0, dest = 0, ttl = 1, type = 0, new_hop = 1;

    packet_s *pkt = init_packet(src, hop, dest, type, ttl);

    assert(next_hop(pkt, new_hop) == ttl - 1);
    assert(pkt->fields.hop_id == new_hop);
    assert(pkt->fields.hop_id != hop);
    assert(pkt->fields.ttl == ttl - 1);

    free_packet(pkt);

    // test w/ invalid ttl
    hop = 0;
    new_hop = 1;
    ttl = 0;

    pkt = init_packet(src, hop, dest, type, ttl);

    assert(next_hop(pkt, new_hop) == -1);
    assert(pkt->fields.hop_id != new_hop);
    assert(pkt->fields.hop_id == hop);
    assert(pkt->fields.ttl == ttl);

    free_packet(pkt);

    // test w/ NULL pkt
    assert(next_hop(NULL, 1) == -1);
}

void test_encode_packet() {
    // test simple mng packet
    uint8_t src = 1, hop = 2, dest = 4, ttl = 1, type = 0;
    uint8_t *buf = NULL;
    uint8_t len;
    uint8_t mng_type = 1;

    uint8_t soln1[5] = {0x8, 0x11, 0x1, 0x0, 0x1};
    uint8_t soln_len = 5;

    packet_s *pkt = init_packet(src, hop, dest, type, ttl);

    pkt->mng = init_mng_pkt(mng_type, NULL, 0);

    buf = encode_packet(pkt, &len);

    for (int i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");

    // test
    assert(soln_len == len);
    assert(memcmp(buf, soln1, soln_len) == 0);
    free_packet(pkt);
    free(buf);

    // test simple mng
    src = 1, hop = 2, dest = 4, ttl = 1, type = 0;
    buf = NULL;
    len;
    mng_type = 0;
    uint8_t mng_buf[1] = {0x1};
    uint8_t mng_buf_len = 1;

    uint8_t soln2[6] = {0x8, 0x11, 0x1, 0x0, 0x0, 0x1};
    soln_len = 6;

    pkt = init_packet(src, hop, dest, type, ttl);

    pkt->mng = init_mng_pkt(mng_type, mng_buf, mng_buf_len);

    buf = encode_packet(pkt, &len);

    for (int i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");

    // test
    assert(soln_len == len);
    assert(memcmp(buf, soln2, soln_len) == 0);
    free_packet(pkt);
    free(buf);
}

void test_packet() {
    // test size
    assert(sizeof(packet_s) == TRANS_PACKET_SIZE);

    // test create packet
    test_init_packet();

    // test next hop
    test_next_hop();

    // test encode packets
    test_encode_packet();

    printf("done packet.h\n");
}

int main() {
    test_packet();
}
