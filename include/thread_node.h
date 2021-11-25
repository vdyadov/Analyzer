#ifndef _THREAD_NODE_H_
#define _THREAD_NODE_H_

// Двусвязанный список
/*
typedef struct dblinked_thread_list
{
    size_t size;
    struct thread_node *head_thr_node;
}
*/

// Элемент двусвязанного списка
typedef struct thread_node 
{
    void *ptr_data;
    struct thread_node *prev_thr_node;
    struct thread_node *next_thr_node;
} thread_node;


// Добавление элемента
struct thread_node *push_thread_node(
        struct thread_node **head_thr_node,
        void *ptr_data);


// Удаление элемента
int delete_thread_node(
        struct thread_node **head_thr_node,
        struct  thread_node *del_thread_node);


// Удаление всех элементов
struct thread_node *del_all_thread_node(struct thread_node **head_thr_node);


// Получить данные по указателю на элемент списка
void *get_ptr_data_by_ptr(
        struct thread_node **head_thr_node,
        struct  thread_node *thr_node);


// Получить данные по индексу
void *get_ptr_data_by_index(
        struct thread_node **head_thr_node,
        int index);


#endif
