#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "list_struct_enum.h"
#include "list_funks.h"
#include "list_dumps.h"

//#define FREE(object) { free(object); object = NULL; } можно попробовать 

// int get_free_cell(List *list) {//возвращает индекс первой свободной ячейки
//     assert(list != NULL);

//     if (list->free == 0) {
//         return 0; 
//     }
//     int free_cell = list->free;
//     list->free = list->next[free_cell];
//     return free_cell;
// }

List_error list_size_increase(List *list) {
    assert(list != NULL);

    size_t new_capacity = list->capacity * 2;
    double* new_data = (double*)realloc(list->data, new_capacity * sizeof(double));
    int* new_next = (int*)realloc(list->next, new_capacity * sizeof(int));
    int* new_prev = (int*)realloc(list->prev, new_capacity * sizeof(int));
    if (new_data == NULL || new_next == NULL || new_prev == NULL) {
        printf("Не сработал realloc\n");
        return LIST_MEMORY_ERROR;
    }
    list->data = new_data;
    list->next = new_next;
    list->prev = new_prev;

        int last_free = list->free;
        while (list->next[last_free] != 0) { //находим последнюю свободную ячейку
            last_free = list->next[last_free];
        }
        for (size_t i = list->capacity; i < new_capacity - 1; ++i) {
            list->next[i] = (int)i + 1;
            list->prev[i] = -1;
        }
        list->next[new_capacity - 1] = 0;
        list->prev[new_capacity - 1] = -1;
        list->next[last_free] = (int)list->capacity;

        list->capacity = new_capacity;
        return LIST_NO_ERROR;
}

List_error list_init(List *list, size_t list_size) {
    assert(list != NULL);

    list->data = (double*)calloc(list_size, sizeof(double));
    list->next = (int*)calloc(list_size, sizeof(int));
    list->prev = (int*)calloc(list_size, sizeof(int));
    if (list->data == NULL || list->next == NULL || list->prev == NULL) {
        printf("Calloc не сработал \n");
        return LIST_MEMORY_ERROR;
    }

    list->next[0] = 0;
    list->prev[0] = 0;
    list->data[0] = POIZON;

    for (size_t i = 1; i < list_size; i++) {
        list->next[i] = (int)i + 1;
        list->prev[i] = -1;
        list->data[i] = POIZON;
    }
    list->next[list_size - 1] = 0;

    list->next[0] = 0;
    list->prev[0] = 0;
    list->free = 1;
    list->capacity = list_size;

    List_error err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return err;
    }
    return LIST_NO_ERROR;
}

int list_insert_after(List *list, int index, double value) {
    assert(list != NULL);

    list_dump(list);
    List_error err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return err;
    }

    if (list->free == -1) { 
        printf("Нет свободного места для вставки\n");
        list_size_increase(list);
        //return LIST_INSERT_ERROR;
    }
    int new_ind = list->free; //первая свободная
    int old_ind = list->next[index]; //старый следующий - тот,что раньше был после index

    list->free = list->next[new_ind];
    list->data[new_ind] = value;

    list->next[index] = new_ind;
    list->next[new_ind] = old_ind;

    list->prev[new_ind] = list->prev[old_ind];
    list->prev[old_ind] = new_ind;

    err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return err;
    }
    list_dump(list);
    return new_ind; //todo что должно возвращать 

}

List_error list_insert_before(List *list, int index, double value) {
    assert(list != NULL);

    list_dump(list);
    List_error err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return err;
    }

    list_insert_after(list, list->prev[index], value);

    err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return err;
    }
    list_dump(list);

    return LIST_NO_ERROR;
}


List_error list_delete(List *list, int index) {
    assert(list != NULL);

    list_dump(list);
    List_error err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return err;
    }


    list->data[index] = POIZON;
    int next_ind = list->next[index];
    int prev_ind = list->prev[index];

    list->next[prev_ind] = next_ind;
    list->prev[next_ind] = prev_ind;

    list->next[index] = list->free;
    list->prev[index] = -1;

    list->free = index;

    err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return err;
    }
    list_dump(list);

    return LIST_NO_ERROR;
}

void list_output_errrors(List *list) {
    List_error err = list_verify(list);
    switch(err) {
        case LIST_NO_ERROR:
            printf("Все ок\n");
            break;
        case LIST_INSERT_ERROR:
            printf("ОШибка вставки элемента\n");
            break;
        case LIST_DELETE_ERROR:
            printf("Ошибка удаления элемента\n");
            break;
        case LIST_MEMORY_ERROR:
            printf("Ошибка выделения памяти\n");
            break;
        case LIST_INVALID_PTR:
            printf("Ошибка в указателе\n");
            break;
        case LIST_HEAD_ERROR:
            printf("Ошибка в голове\n");
            break;
        case LIST_TAIL_ERROR:
            printf("Ошибка в хвосте\n");
            break;
        case LIST_NEXT_ERROR:
            printf("Ошибка в массиве next\n");
            break;
        case LIST_COMMUNICATION_ERROR:
            printf("Ошибка в связях элементов массивов\n");
            break;
        case LIST_FILE_ERROR:
            printf("Ошибка работы с выходным файлом\n");
            break;
        default :
            printf("FATAL ERROR\n");
    }
    return;

}



List_error list_verify(List *list) {

    if (list == NULL)
        return LIST_INVALID_PTR;
    if (list->data == NULL || list->next == NULL || list->prev == NULL)
        return LIST_MEMORY_ERROR;

    if (list->next[0] != 0) { //роверка головы и хвоста
        if (list->next[0] >= (int)list->capacity || list->prev[list->next[0]] != 0)
            return LIST_HEAD_ERROR;
        if (list->prev[0] >= (int)list->capacity || list->next[list->prev[0]] != 0)
            return LIST_TAIL_ERROR;
    } else {
        if (list->prev[0] != 0) 
            return LIST_TAIL_ERROR;
    }

    if (list->next[0] != 0) {
        int current_ind = list->next[0];
        int cnt = 0;

        while(current_ind != 0 && cnt < (int)list->capacity) {
            if (list->next[current_ind] >= (int)list->capacity)
                return LIST_NEXT_ERROR;

            if (list->next[current_ind] != 0 && list->prev[list->next[current_ind]] != current_ind)
                return LIST_COMMUNICATION_ERROR;
            
            current_ind = list->next[current_ind];
            cnt++;
        }

    }

    return LIST_NO_ERROR; 

}


List_error list_destroy(List *list) {
    assert(list != NULL);

    List_error err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        //list_dump(stk, err);
        return err;
    }
    free(list->data);
    free(list->next);
    free(list->prev);

    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;

    list->free = 0;
    list->capacity = 0;

    return LIST_NO_ERROR;
}




