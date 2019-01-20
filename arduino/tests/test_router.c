#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "routing_tables.h"

int main() {

    route_table* table = init_routing_table(1);

    
    route new_route;
    new_route.id = 3;
    new_route.dest = 2;
    new_route.hops = 3;
    add_to_table(table, new_route);



    printf("id:%d\n", table->id);
    printf("len:%d\n", table->len);
    printf("size:%d\n", table->end);
    printf("peer:%d\n", table->routes[0].id);
    printf("peer dest:%d\n", table->routes[0].dest);
    printf("peer hops expected:%d\n", table->routes[0].hops);
    free_tables(table);
    


    return 1;
}
