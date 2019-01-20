
#include <stdint.h>
#include <stdlib.h>
#include "routing_tables.h"


route_table* init_routing_table (uint8_t id) {

    route_table* router = calloc(1, sizeof(route_table));
    router->id = id;
    router->len = 0;

    //starting size for routes
    router->end = ROUTING_TABLE_SIZE;
    router->routes = calloc(ROUTING_TABLE_SIZE, sizeof(route));
    return router;
}


void free_tables (route_table *table) {
    free(table->routes);
    free(table);
}



void add_to_table(route_table *table, route new_route) {
    

    for (int i = 0; i < table->len; i++) {
        if (new_route.id == table->routes[i].id &&
        new_route.dest == table->routes[i].dest)

        if (new_route.hops == table->routes[i].hops) {
            //don't add if already in table
            return;
        } else {
            //change hops if they change
            table->routes[i].hops = new_route.hops;
        }
    }

    table->routes[table->len].id = new_route.id;
    table->routes[table->len].dest = new_route.dest;
    table->routes[table->len].hops = new_route.hops;
    table->len++;

}

void delete_from_table(){
}

