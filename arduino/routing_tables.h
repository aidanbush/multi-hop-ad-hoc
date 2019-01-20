
#ifndef ROUTING_TABLES
#define ROUTING_TABLES
#include <stdint.h>

//starting memory allocation, and rate at which it grows.
#define ROUTING_TABLE_SIZE 10


struct route {

    uint8_t id;
    uint8_t dest;
    uint8_t hops;

} typedef route;


struct route_table {

    uint8_t id;
    uint8_t len;
    uint8_t end;
    route *routes; 

} typedef route_table;


route_table *init_routing_table (uint8_t id);
void add_to_table(route_table *table, route new_route);


#endif

