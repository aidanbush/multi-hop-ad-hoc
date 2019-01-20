#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "routing_tables.h"

int main() {

    route_table* table = init_routing_table(1);
    add_to_table(table, 1, 2, 3);
    add_to_table(table, 1, 4, 3);
    add_to_table(table, 1, 3, 3);
    printf("id:%d\n", table->id);
    printf("capacity:%d\n", table->capacity);
    printf("size:%d\n", table->end);
    printf("peer:%d\n", table->routes[0].id);
    printf("peer dest:%d\n", table->routes[0].dest);
    printf("peer hops expected:%d\n", table->routes[0].hops);

    delete_from_table(table, 2);


    printf("id:%d\n", table->id);
    printf("capacity:%d\n", table->capacity);
    printf("size:%d\n", table->end);
    printf("peer:%d\n", table->routes[0].id);
    printf("peer dest:%d\n", table->routes[0].dest);
    printf("peer hops expected:%d\n", table->routes[0].hops);

    free_tables(table);
    


    return 1;
}
