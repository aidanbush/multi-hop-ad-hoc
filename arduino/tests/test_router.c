#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "routing_tables.h"

int main() {

    route_table* table = init_routing_table(1);
    add_to_table(table, 1, 2, 3);
    add_to_table(table, 2, 2, 4);
    add_to_table(table, 3, 2, 5);

    add_to_table(table, 1, 3, 5);
    add_to_table(table, 2, 3, 4);
    add_to_table(table, 3, 3, 3);

    add_to_table(table, 1, 4, 4);
    add_to_table(table, 2, 4, 5);
    add_to_table(table, 3, 4, 3);



    printf("nexthop:%d\n", get_next_hop(table, 3));
    printf("nexthop:%d\n", get_next_hop(table, 4));
    printf("nexthop:%d\n", get_next_hop(table, 5));

    printf("id:%d\n", table->id);
    printf("capacity:%d\n", table->capacity);
    printf("size:%d\n", table->end);
    printf("peer:%d\n", table->routes[0].id);
    printf("peer dest:%d\n", table->routes[0].dest);
    printf("peer hops expected:%d\n", table->routes[0].hops);

    free_tables(table);
    

    return 1;
}
