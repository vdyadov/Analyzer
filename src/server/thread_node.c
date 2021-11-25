// Двусвязанный список
//

#include "thread_node.h"
#include <stdio.h>
#include <stdlib.h>


// Добавление элемента
//
struct thread_node *push_thread_node(
        struct thread_node **head_thr_node,
        void *ptr_data)
{
    struct thread_node *new_thr_node = NULL;

    new_thr_node = malloc(sizeof(struct thread_node));

    if (NULL == new_thr_node)
    {
        perror("Error memory allocate for thread node!");
        goto finally;
    }

    new_thr_node->ptr_data = ptr_data;

    new_thr_node->prev_thr_node = NULL;
    new_thr_node->next_thr_node = *head_thr_node;

    if (NULL != *head_thr_node)
    {
        (*head_thr_node)->prev_thr_node = new_thr_node;
    }

    (*head_thr_node) = new_thr_node;

 finally:

    return new_thr_node;
}


// Удаление элемента
//
int delete_thread_node(
        struct thread_node **head_thr_node,
        struct  thread_node *del_thr_node)
{
    int ret = 0;

    if (NULL == *head_thr_node || NULL == del_thr_node)
    {
        ret = -1;
        goto finally;
    }
    
    if (del_thr_node == *head_thr_node)
    {
        *head_thr_node = del_thr_node->next_thr_node;
    }

    if (NULL != del_thr_node->next_thr_node)
    {
        del_thr_node->next_thr_node->prev_thr_node = del_thr_node->prev_thr_node;
    }

    if (NULL != del_thr_node->prev_thr_node)
    {
        del_thr_node->prev_thr_node->next_thr_node = del_thr_node->next_thr_node;
    }

    free(del_thr_node);

 finally:

    return ret;
}


// Удаление всех элементов
//
struct thread_node *del_all_thread_node(struct thread_node **head_thr_node)
{
    struct thread_node *next_del_thr_node = NULL;

    while (NULL != *head_thr_node)
    {
        next_del_thr_node = (*head_thr_node)->next_thr_node;

        free(*head_thr_node);

        *head_thr_node = next_del_thr_node;
    }

    return *head_thr_node;
}


// Получить данные по указателю на элемент списка
void *get_ptr_data_by_ptr(
        struct thread_node **head_thr_node,
        struct  thread_node *thr_node)
{
    void *ptr = NULL;

    if (NULL != *head_thr_node && NULL != thr_node)
    {
        ptr = thr_node->ptr_data;
    }

    return ptr;
}


// Получить данные по индексу
void *get_ptr_data_by_index(
        struct thread_node **head_thr_node,
        int index)
{
    int i = 0;
    struct thread_node *curr_thr_node = *head_thr_node;

    while (NULL != curr_thr_node && i < index) {
        curr_thr_node = curr_thr_node->next_thr_node;
        i++;
    }

    return ((NULL != curr_thr_node) ? curr_thr_node->ptr_data : NULL);
}
