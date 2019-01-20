#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "packet.h"

void test_init_packet() {
    uint8_t src = 0, hop = 0, dest = 0, ttl = 0, type = 0;

    // all low 0 test
    packet_s *pkt = init_packet(src, hop, dest, type, ttl);

    assert(pkt->source_id == src);
    assert(pkt->hop_id == hop);
    assert(pkt->dest_id == dest);
    assert(pkt->ttl == ttl);
    assert(pkt->unused == 0);
    assert(pkt->type == type);
    assert(pkt->mng == NULL);

    free_packet(pkt);

    // all max test
    src = 0x3f;
    hop = 0x3f;
    dest = 0x3f;
    ttl = 0x3f;
    type = 1;

    pkt = init_packet(src, hop, dest, type, ttl);

    assert(pkt->source_id == src);
    assert(pkt->hop_id == hop);
    assert(pkt->dest_id == dest);
    assert(pkt->ttl == ttl);
    assert(pkt->unused == 0);
    assert(pkt->type == type);
    assert(pkt->mng == NULL);

    free_packet(pkt);

    // over limit
    src = 0x40;
    hop = 0x40;
    dest = 0x40;
    ttl = 0x40;
    type = 2;

    pkt = init_packet(src, hop, dest, type, ttl);

    assert(pkt->source_id == src % 0x40);
    assert(pkt->hop_id == hop % 0x40);
    assert(pkt->dest_id == dest % 0x40);
    assert(pkt->ttl == ttl % 0x40);
    assert(pkt->unused == 0);
    assert(pkt->type == type % 2);
    assert(pkt->mng == NULL);

    free_packet(pkt);
}

void test_next_hop() {
    // test w/ valid ttl
    uint8_t src = 1, hop = 0, dest = 0, ttl = 1, type = 0, new_hop = 1;

    packet_s *pkt = init_packet(src, hop, dest, type, ttl);

    assert(next_hop(pkt, new_hop) == ttl - 1);
    assert(pkt->hop_id == new_hop);
    assert(pkt->hop_id != hop);
    assert(pkt->ttl == ttl - 1);

    free_packet(pkt);

    // test w/ invalid ttl
    hop = 0;
    new_hop = 1;
    ttl = 0;

    pkt = init_packet(src, hop, dest, type, ttl);

    assert(next_hop(pkt, new_hop) == -1);
    assert(pkt->hop_id != new_hop);
    assert(pkt->hop_id == hop);
    assert(pkt->ttl == ttl);

    free_packet(pkt);

    // test w/ NULL pkt
    assert(next_hop(NULL, 1) == -1);
}

void test_packet() {
    // test size
    assert(sizeof(packet_s) == TRANS_PACKET_SIZE);

    // test create packet
    test_init_packet();

    // test next hop
    test_next_hop();

    printf("done packet.h\n");
}

int main() {
    test_packet();
}