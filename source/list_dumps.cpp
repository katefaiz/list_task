#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "list_types.h"
#include "list_dumps.h"

List_error list_dump(List *list) {
    assert(list != NULL);

    FILE *filestream = fopen("graf.dot", "w");
    if (filestream == NULL) 
        return LIST_FILE_ERROR;

    list_dump_base(list, filestream);
    fprintf(filestream, "\n");
    dump_create_node(list, filestream);
    fprintf(filestream, "\n");
    dump_create_edge(list, filestream);
    fprintf(filestream, "}");

    fclose(filestream);

    char* buffer = (char*)calloc(1024, sizeof(char));
    if (buffer == NULL) {
        printf("Calloc не сработал \n");
        return LIST_MEMORY_ERROR;
    }

    int id = rand();
    sprintf(buffer, "dot -Tpng graf.dot -o png/test%d.png", id);
    int status = system(buffer);
    if (status != 0) 
        return LIST_FILE_ERROR;
    
    free(buffer);

    FILE *html_file = fopen("dump.html", "a");
    dump_make_html(list, html_file, id); 
    fclose(html_file);


    return LIST_NO_ERROR;

}

List_error list_dump_base(List *list, FILE *filestream) {
    assert(list);
    assert(filestream);

    fprintf(filestream, "digraph List {\n");
    fprintf(filestream, "\trankdir=LR\n");

    return LIST_NO_ERROR;

}

List_error dump_create_node(List *list, FILE *filestream) {
    assert(list);
    assert(filestream);

    fprintf(filestream, "\tnode0 [shape=Mrecord, style=\"rounded, filled\", label=\"0 | %g | next=%d | prev=%d\", fillcolor=\"grey\"];\n",
                                                                         list->data[0], list->next[0], list->prev[0]);
    for (size_t i = 1; i < list->capacity; i++) {
        if (list->prev[i] == -1) { // свободные - зеленые
            fprintf(filestream, "\tnode%zu [shape=Mrecord, style=\"rounded, filled\", label=\"%zu | %g | next=%d | prev=%d\", fillcolor=\"lightgreen\"];\n", 
                                        i,                                              i, list->data[i], list->next[i], list->prev[i]);
        } else { // занятые - голубые
            fprintf(filestream, "\tnode%zu [shape=Mrecord, style=\"rounded, filled\", label=\"%zu | %g | next=%d | prev=%d\", fillcolor=\"lightblue\"];\n", 
                                        i,                                              i, list->data[i], list->next[i], list->prev[i]);
        }
    }

    return LIST_NO_ERROR;

}

List_error dump_create_edge(List *list, FILE *filestream) {
    assert(list);
    assert(filestream);

    //dump_create_invisedge(list, filestream);
    //next edge - синие
    fprintf(filestream, "\tedge[color=blue];\n  ");

    fprintf(filestream, "\tnode0 -> node%d\n  ", list->next[0]);
    for (int i = list->next[0]; i != 0; i = list->next[i]) { 
        fprintf(filestream, "\tnode%d -> node%d\n", 
                                    i,   list->next[i]);
        
    }

    //prev edge - серые
    fprintf(filestream, "\n\tedge[color=gray]\n");

    fprintf(filestream, "\tnode0 -> node%d\n  ", list->prev[0]);
    for (int i = list->prev[0]; i != 0; i = list->prev[i]) { 
        fprintf(filestream, "\tnode%d -> node%d\n", 
                                    i,   list->prev[i]);
    }
    //free edge - зеленые
    fprintf(filestream, "\n\tedge[color=green]\n");

    fprintf(filestream, "\tnode%d\n", list->free);

    for (int i = list->free; i != 0; i = list->next[i]) {
        fprintf(filestream, "\tnode%d [fillcolor=\"lightgreen\"]\n", i);
    
    if (list->next[i] != 0) {
        fprintf(filestream, "\tnode%d -> node%d\n", 
                                    i,    list->next[i]);
    }
}
    


    return LIST_NO_ERROR;

}
List_error dump_create_invisedge(List *list, FILE *filestream) { //невидимые стрелки - не работают 
    assert(list);
    assert(filestream);
    for (int i = list->next[0]; i != 0; i = list->next[i]) {
        fprintf(filestream, "\tnode%d -> node%d [style = \"invis\"]\n", 
                                    i,       i+1);
    }
    fprintf(filestream, "\n");

    return LIST_NO_ERROR;

}
List_error dump_make_html(List *list, FILE *html_file, int test_id) {
    assert(list != NULL);
    assert(html_file != NULL);
    
    fprintf(html_file, "<pre>\n<hr>\n    <font size=\"10\"> ");
    fprintf(html_file, "LIST_DUMP\n");

    fprintf(html_file, "    data : ");
    for (size_t i = 0; i < list->capacity; i++) {
        if (i != list->capacity - 1) {
            fprintf(html_file, "%g, ", list->data[i]);
        } else {
            fprintf(html_file, "%g", list->data[i]);
        }
    }
    fprintf(html_file, "\n");
    fprintf(html_file, "    next : ");
    for (size_t i = 0; i < list->capacity; i++) {
        if (i != list->capacity - 1) {
            fprintf(html_file, "%d, ", list->next[i]);
        } else {
            fprintf(html_file, "%d", list->next[i]);
        }
    }
    fprintf(html_file, "\n");
    fprintf(html_file, "    prev : ");
    for (size_t i = 0; i < list->capacity; i++) {
        if (i != list->capacity - 1) {
            fprintf(html_file, "%d, ", list->prev[i]);
        } else {
            fprintf(html_file, "%d", list->prev[i]);
        }
    }
    fprintf(html_file, "\n");

    fprintf(html_file, "<img src=png/test%d.png>\n", test_id);

    return LIST_NO_ERROR;
}


