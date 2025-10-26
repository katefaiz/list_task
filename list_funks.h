#ifndef LISTF_H
#define LISTF_H

#include "list_struct_enum.h"

int get_free_cell(List *list);
List_error list_size_increase(List *list);
int list_insert(List *list, int index, double value);
List_error list_init(List *list, size_t list_size);
List_error list_delete(List *list, int index);
void list_output_errrors(List *list);
List_error list_verify(List *list);
List_error list_destroy(List *list);



#endif //LISTF