#ifndef HELP_H
#define HELP_H

#include <stdint.h>
#include "packet.h"

void set_id(uint8_t id);

uint8_t get_id();

int send_pkt(packet_s *pkt);

#endif /* HELP_H */
