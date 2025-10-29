#ifndef LISTD_H
#define LISTD_H

#include <stdio.h>

#include "list_struct_enum.h"

List_error list_dump(List *list);
List_error dump_create_edge(List *list, FILE *filestream);
List_error dump_create_node(List *list, FILE *filestream);
List_error list_dump_base(List *list, FILE *filestream);
List_error dump_create_invisedge(List *list, FILE *filestream) ;
List_error dump_make_html(List *list, FILE *html_file, int test_id);



#endif //LISTD