#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "list_struct_enum.h"
#include "list_funks.h"

void list_dump(List *list) {

    printf("head = %d, tail = %d, free = %d, capacity = %zu\n", 
           list->head, list->tail, list->free, list->capacity);
    
    printf("Индексы:  ");
    for (size_t i = 0; i < list->capacity; i++) {
        printf("%4zu ", i);
    }
    printf("\n");
    
    printf("data:     ");
    for (size_t i = 0; i < list->capacity; i++) {
        if (list->prev[i] == -1) 
            printf(" FREE ");
        else 
            printf("%5.1f", list->data[i]);
    }
    printf("\n");
    
    printf("next:     ");
    for (size_t i = 0; i < list->capacity; i++) {
        printf("%4d ", list->next[i]);
    }
    printf("\n");
    
    printf("prev:     ");
    for (size_t i = 0; i < list->capacity; i++) {
        printf("%4d ", list->prev[i]);
    }
    printf("\n\n");
}

int main() {
    List list;

    list_init(&list, 10);
    list_output_errrors(&list);
    list_dump(&list);
    
    
    
    return 0;
}