#ifndef _CREATE_SOCKET_H_
#define _CREATE_SOCKET_H_


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>


struct packet {
    uint32_t size_packet;
    uint16_t type_packet;
    uint32_t ip_client;
    uint16_t port_client;
    uint16_t number_test;
    uint32_t count_packet;
    uint32_t time_test;
}__attribute__((packed));


typedef enum Test_pack_type
{
    UDP = 0,    // = 0
    TCP,        // = 1
    ICMP,       // = 2
} Test_packet_type;


//#include "decoder_packet.h"


int create_socket_clientTCP(int client_port, int server_ip);
int create_socket_clientUDP(void);
//int create_socket_clientUDP(/*int client_port, int server_ip*/);
//int create_socket_clientACMP();


int create_socket_serverTCP(int server_port);
int create_socket_serverUDP(int server_port);
//int create_socket_serverACMP();


#endif
