
#include <stdint.h>
#include <stdlib.h>
#include "routing_tables.h"


route_table* init_routing_table (uint8_t id) {

    route_table* router = calloc(1, sizeof(route_table));
    router->id = id;
    router->capacity = ROUTING_TABLE_SIZE;

    //starting size for routes
    router->end = ROUTING_TABLE_SIZE;
    router->routes = calloc(ROUTING_TABLE_SIZE, sizeof(route));
    return router;
}


void free_tables (route_table *table) {
    free(table->routes);
    free(table);
}



void add_to_table(route_table *table, uint8_t hops, uint8_t source, uint8_t dest) {
    

    for (int i = 0; i < table->capacity; i++) {
        if (source == table->routes[i].id &&
        dest == table->routes[i].dest)

        if (hops == table->routes[i].hops) {
            //don't add if already in table
            return;
        } else {
            //change hops if they change
            table->routes[i].hops = hops;
        }
    }

    for (int i = 0; i < table->end; i++) {
        if (table->routes[i].id != 0)
            continue;


        table->routes[i].id = source;
        table->routes[i].dest = dest;
        table->routes[i].hops = hops;
        table->capacity--;
        return;
    }

}

void delete_from_table(route_table *table, uint8_t id){
    for (int i = 0; i< table->end; i++) {
        if (table->routes[i].id == id) {
            table->routes[i].id = 0;
            table->routes[i].hops = 0;
            table->routes[i].dest = 0;
            table->capacity++;
            
        }
    } 
}


uint8_t get_next_hop(route_table *table, uint8_t dest) {

    uint8_t min_hops = 128;
    uint8_t next_hop = 0;

    for (int i = 0; i< table->end; i++) {
        if (table->routes[i].dest == dest) {
            if (table->routes[i].hops < min_hops)
                next_hop = table->routes[i].id;
                min_hops = table->routes[i].hops;

           
        }
    }
    return next_hop; 
}