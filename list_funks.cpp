#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "list_struct_enum.h"
#include "list_funks.h"


int get_free_cell(List *list) {//возвращает индекс первой свободной ячейки
    if (list->free == 0) {
        return 0; 
    }
    int free_cell = list->free;
    list->free = list->next[free_cell];
    return free_cell;
}

List_error list_size_increase(List *list) {
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
        for (size_t i = list->capacity; i < new_capacity - 1; i++) {
            list->next[i] = i + 1;
            list->prev[i] = -1;
        }
        list->next[new_capacity - 1] = 0;
        list->prev[new_capacity - 1] = -1;
        list->next[last_free] = list->capacity;

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

    for (size_t i = 1; i < list_size - 1; i++) {
        list->next[i] = i + 1;
        list->prev[i] = -1;
    }
    list->next[list_size - 1] = 0;
    list->prev[list_size - 1] = -1;

    list->head = 0;
    list->tail = 0;
    list->free = 1;
    list->capacity = list_size;

    List_error err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        //list_dump(stk, err);
        return err;
    }
    return LIST_NO_ERROR;
}

int list_insert(List *list, int index, double value) {
    assert(list != NULL);
    List_error err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return -1;
    }

    int free_index = get_free_cell(list);
    
    if (free_index == 0) {
        list_size_increase(list);
        free_index = get_free_cell(list);
    }
    list->data[free_index] = value;

    if (list->head == 0) { //если список пуст
        list->head = free_index;
        list->tail = free_index;
        list->next[free_index] = 0;
        list->prev[free_index] = 0;
    } else if (index == list->head) { // вставка в начало
        list->next[free_index] = list->head;
        list->prev[free_index] = 0;
        list->prev[list->head] = free_index;
        list->head = free_index;
        
    } else if (index > list->tail) {
        list->next[free_index] = 0;
        list->next[list->tail] = free_index;
        list->prev[free_index] = list->tail;
        list->tail = free_index;
    } else {//вставка в середину под индекс index
        int prev_elem = list->prev[index]; // индекс элемента ПЕРЕД вставкой

        list->next[free_index] = index;
        list->prev[free_index] = prev_elem;
        list->next[prev_elem] = free_index;
        list->prev[index] = free_index;
        //list->prev[list->next[index]] = free_index;

    }

    return free_index;

}


List_error list_delete(List *list, int index) {
    assert(list != NULL);

    List_error err = list_verify(list);
    if (err != LIST_NO_ERROR) {
        return err;
    }
    if (index == 0 || index >= list->capacity || list->head == 0 || list->prev[index] == -1) {
        return LIST_DELETE_ERROR;
    }

    if (index == list->head) {//удаление головы
        if (list->head == list->tail) 
            list->head = list->tail = 0;
        else {
            list->head = list->next[index];
            list->prev[list->head] = 0;
        }
    } else if (index == list->tail) {//удаление хвоста
        list->tail = list->prev[index];
        list->next[list->tail] = 0;
    } else {//середиинка
        list->next[list->prev[index]] = list->next[index];
        list->prev[list->next[index]] = list->prev[index];
    }
    
    list->data[index] = 0;
    list->next[index] = list->free;
    list->prev[index] = -1;
    list->free = index; // НАДО?
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

    if (list->head != 0) { //роверка головы и хвоста
        if (list->head >= list->capacity || list->prev[list->head] != 0)
            return LIST_HEAD_ERROR;
        if (list->tail >= list->capacity || list->next[list->tail] != 0)
            return LIST_TAIL_ERROR;
    } else {
        if (list->tail != 0) 
            return LIST_TAIL_ERROR;
    }

    if (list->head != 0) {
        int current_ind = list->head;
        int cnt = 0;

        while(current_ind != 0 && cnt < list->capacity) {
            if (list->next[current_ind] >= list->capacity)
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

    list->head = 0;
    list->tail = 0;
    list->free = 0;
    list->capacity = 0;

    return LIST_NO_ERROR;
}
