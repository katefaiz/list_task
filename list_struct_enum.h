#ifndef LIST_H
#define LIST_H

#include <math.h>

#define POIZON -666

struct List {
    double *data;
    int *next;
    int *prev;
    int free;
    // int head;
    // int tail;
    size_t capacity;

};

enum List_error {
    LIST_NO_ERROR               = 0,
    LIST_INSERT_ERROR           = 1,
    LIST_DELETE_ERROR           = 2,
    LIST_MEMORY_ERROR           = 3,
    LIST_INVALID_PTR            = 4,
    LIST_HEAD_ERROR             = 5,
    LIST_TAIL_ERROR             = 6,
    LIST_NEXT_ERROR             = 7,
    LIST_COMMUNICATION_ERROR    = 8,
    LIST_FILE_ERROR             = 9 
};

#endif //LIST