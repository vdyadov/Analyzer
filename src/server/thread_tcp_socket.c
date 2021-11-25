#define _GNU_SOURCE
#include <pthread.h>
#undef _GNU_SOURCE


#include "definitions.h"
#include "tcp_socket_function.h"
#include "thread_tcp_socket.h"
#include "thread_node.h"
#include "tcp_command_processor.h"


#define MAX_NUM_CLIENTS 8


typedef enum Pthread_join_block
{
    JOIN_BLOCK_DISABLE = 0,
    JOIN_BLOCK_ENABLE, 
} Pthread_join_block;


/** A public global variables.
 * Volatile flag for Ctr-C cansel thread.
 * Set non-zero after SIGINT.
 */
volatile sig_atomic_t stop_all_thr = 0;


// Double linked list of thread, head:
static struct thread_node *head_thr_node = NULL;

// Number of connected clients
static int num_clients = 0;

static int cansel_tcp_sock_thr(void);

static int join_tcp_sock_thr(int block);

static int connect_single_client(struct client_socket *c_s);

static int create_tcp_sock_thr(struct client_socket const *c_s);

static void free_mem(struct comm_sock_thread *c_s_thr);

static void *thr_tcp_socket(void *args);

static void print_socket_info(struct client_socket const *c_s);



int connect_clients(int const fd_soc)
{
    int ret = 0;
    int err_ret = 0;
    struct client_socket client_soc = {0};

    num_clients = 0;

    for (;; ret = 0)
    {
        if (1 == stop_all_thr)
        {
            puts("\nResive signal SIGINT, server interrupted!\n");
            break;
        }

        ret = accept_socket(
                fd_soc,
                &client_soc,
                &err_ret);

        if (0 != ret)
        {
            switch (err_ret)
            {
                // accept_socket(...) interrupted by signal SIGINT
                case EINTR:
                {
                    continue;
                }
                /*  EWOULDBLOCK - оperation would block
                    we don't check because in <include/asm-generic/errno.h> set
                    #define EWOULDBLOCK     EAGAIN
                    where EAGAIN - try again */
                case EAGAIN:
                {
                    if (0 != num_clients)
                    {
                        ret = join_tcp_sock_thr(JOIN_BLOCK_DISABLE);
                        if (0 != ret)
                        {
                            fprintf(stderr, "Error in join_tcp_sock_thr(...) in function"
                                    " connect_clients(...)!\n");
                            ret = -1;
                            goto finally;
                        }
                    }
                    continue;   // No clients to connect
                }
                default:
                {
                    fprintf(stderr, "Error in accept_socket(...) in function"
                            " connect_clients(...)!\n");
                    ret = -1;
                    goto finally;
                }
            }
        }

        ret = connect_single_client(&client_soc);

        if (0 != ret)
        {
            close_socket(&client_soc);
        }
    }

 finally:

    if (0 != num_clients)
    {
        ret = cansel_tcp_sock_thr();
        if (0 == ret)
        {
            ret = join_tcp_sock_thr(JOIN_BLOCK_ENABLE);
        }
    }

    return ret;
}


static int cansel_tcp_sock_thr(void)
{
    int ret = 0;
    int i = 0;

    struct comm_sock_thread *c_s_thr = NULL;

    if (NULL == head_thr_node)
    {
        fprintf(stderr, "Error, head_thr_node = %p!\n", head_thr_node);
        ret = -1;
        goto finally;
    }

    while (NULL != (c_s_thr = get_ptr_data_by_index(&head_thr_node, i)))
    {
        ret = pthread_cancel(c_s_thr->tid_comm_sock);
 
        if (0 != ret)
        {
            fprintf(stderr, "pthread_cancel(...) for thread %ld return error: %d\n",
                    c_s_thr->tid_comm_sock,
                    ret);
            ret = -1;
            break;
        }

        i++;
    }

 finally:

    return ret;
}


static int join_tcp_sock_thr(int const block)
{
    int ret = 0;
    int i = 0;

    void *retval = NULL;
    struct comm_sock_thread *c_s_thr = NULL;

    if (NULL == head_thr_node)
    {
        fprintf(stderr, "Error, head_thr_node = %p!\n", head_thr_node);
        ret = -1;
        goto finally;
    }

    while (NULL != (c_s_thr = get_ptr_data_by_index(&head_thr_node, i)))
    {
        if (JOIN_BLOCK_DISABLE == block)
        {
            ret = pthread_tryjoin_np(c_s_thr->tid_comm_sock, &retval);
        }
        else
        {
            ret = pthread_join(c_s_thr->tid_comm_sock, &retval);
        }

        if (0 == ret)
        {
            close_socket(c_s_thr->cli_sock);

            ret = delete_thread_node(
                        &head_thr_node,
                        c_s_thr->thr_node);

            if (-1 == ret)
            {
                fprintf(stderr, "Error in delete_thread_node(...)"
                        " with index = %d!\n", i);
                break;
            }

            free_mem(c_s_thr);

            num_clients--;

            continue;
        }
        else if (EBUSY != ret)
        {
            fprintf(stderr, "pthread_tryjoin_np(...) for thread %ld return error: %d\n",
                    c_s_thr->tid_comm_sock,
                    ret);
            ret = -1;
            break;
        }

        ret = 0;
        i++;
    }

 finally:

    return ret;
}


static int connect_single_client(struct client_socket *const c_s)
{
    int ret = 0;

    if (num_clients < MAX_NUM_CLIENTS)
    {
        ret = get_socket_params(c_s);

        if (-1 == ret)
        {
            fprintf(stderr, "Error in get_socket_params(...) for client %d!\n",
                    num_clients + 1);
            goto finally;
        }

        // Create thread for command socket
        ret = create_tcp_sock_thr(c_s);
        
        // Thread OK
        if (0 == ret)
        {
            num_clients++;
            goto finally;
        }
    }
    else
    {
        ret = -1;
        printf("The number of clients exceeds %d, connection close!\n", num_clients);
    }

 finally:

    return ret;
}


static int create_tcp_sock_thr(struct client_socket const *const c_s)
{
    int ret = 0;

    struct comm_sock_thread *c_s_thr = NULL;

    if (NULL == c_s || -1 == c_s->fd_sock_client)
    {
        fprintf(stderr, "Error in arguments create_tcp_sock_thr(...)!\n");
        ret = -1;
        goto finally;
    }

    c_s_thr = malloc(sizeof(struct comm_sock_thread));
    if (NULL == c_s_thr)
    {
        perror("Error memory allocate for struct command_sock_thread!");
        ret = -1;
        goto finally;
    }

    c_s_thr->cli_sock = malloc(sizeof(struct client_socket));
    if (NULL == c_s_thr->cli_sock)
    {
        perror("Error memory allocate for struct client_socket!");
        ret = -1;
        goto finally;
    }

    memcpy(c_s_thr->cli_sock, c_s, sizeof(struct client_socket));

    c_s_thr->thr_node = push_thread_node(
            &head_thr_node,
            (void *)c_s_thr);

    if (NULL == c_s_thr->thr_node)
    {
        ret = -1;
        goto finally;
    }

    ret = pthread_create(
            &c_s_thr->tid_comm_sock,
            NULL,
            &thr_tcp_socket,
            c_s_thr); 
    if (0 != ret) 
    {
        perror("Error pthread_create(...) for command sock,"
                " function create_tcp_sock_thr(...)");
        
        delete_thread_node(
                &head_thr_node,
                c_s_thr->thr_node);
        
        ret = -1;
        goto finally;
    }

 finally:

    if (-1 == ret)
    {
        free_mem(c_s_thr);
    }

    return ret;
}


static void free_mem(struct comm_sock_thread *c_s_thr)
{
    if (NULL != c_s_thr)
    {
        if (NULL != c_s_thr->cli_sock)
        {
            free(c_s_thr->cli_sock);
        }

        free(c_s_thr);
        c_s_thr = NULL;
    }
}


static void *thr_tcp_socket(void *args)
{
    struct comm_sock_thread *c_s_thr = args;

    printf("\n******* Thread: %ld\n", c_s_thr->tid_comm_sock);
    print_socket_info(c_s_thr->cli_sock);

    // Receive data from the client and starting packet_handler
    receive_wexec_packet_handler(c_s_thr->cli_sock);

    return c_s_thr->thr_node;
}


static void print_socket_info(struct client_socket const *const c_s)
{
    printf("\nFor socket: %d\n"
            "\t--connected client:\n"
            "\t\taddr: %s\n"
            "\t\tport: %u\n",
            c_s->fd_sock_client,
            inet_ntoa(c_s->cli_addr),
            ntohs(c_s->cli_port));
    printf("\t--server:\n"
            "\t\taddr: %s\n"
            "\t\tport: %u\n\n",
            inet_ntoa(c_s->srv_addr),
            ntohs(c_s->srv_port));
}
