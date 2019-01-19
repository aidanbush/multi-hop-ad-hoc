#include <stdio.h>
#include <assert.h>

#include "packet.h"

void test_packet() {
    assert(sizeof(packet_s) == TRANS_PACKET_SIZE);

    printf("done packet.h\n");
}

int main() {
    test_packet();
}
