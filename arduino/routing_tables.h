#ifndef ROUTING_TABLES_H
#define ROUTING_TABLES_H

#include <stdint.h>

//starting memory allocation, and rate at which it grows.
#define ROUTING_TABLE_SIZE 10

typedef struct __attribute__ ((packed)) route {
    uint8_t id: 6;
    uint8_t dest: 6;
    uint8_t hops;
} route;

typedef struct __attribute__ ((packed)) route_table {
    uint8_t id;
    uint8_t capacity;
    uint8_t end;
    route *routes;
} route_table;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

route_table *init_routing_table (uint8_t id);

void add_to_table(route_table *table, uint8_t hops, uint8_t source, uint8_t dest);

void delete_from_table(route_table *table, uint8_t id);

uint8_t get_next_hop(route_table *table, uint8_t dest);

void free_tables (route_table *table);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ROUTING_TABLES_H */
