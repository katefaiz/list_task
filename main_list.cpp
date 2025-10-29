#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "list_struct_enum.h"
#include "list_funks.h"
#include "list_dumps.h"


int main() {
    List list;

    
    list_init(&list, 10);
    
    list_insert_after(&list, 0, 10);
    list_insert_after(&list, 1, 20);
    list_insert_after(&list, 1, 30);

    list_destroy(&list);

    return 0;
}