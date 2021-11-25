#ifndef _DEFINITION_H_
#define _DEFINITION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>


// Command socket
//
typedef struct comm_sock_thread
{
    pthread_t tid_comm_sock;
    struct thread_node *thr_node;
    struct client_socket *cli_sock;
    //pthread_mutex_t mutex_comm_sock
} comm_sock_thread;

// Configuration for server/client
//
typedef struct client_socket
{
    int fd_sock_client;

    struct in_addr srv_addr;
    uint16_t srv_port;

    struct in_addr cli_addr;
    uint16_t cli_port;

} client_socket;


#endif
