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
    uint32_t number_packet;
    uint32_t ip_client;
    uint16_t port_client;
    uint16_t number_test;

}__attribute__((packed));

enum {

    UDP,  // = 0
    TCP,  // = 1
    ICMP  // = 2

};

//#include "decoder_packet.h"

int create_socket_clientTCP(int client_port);
//int cr_socket_clientUDP();
//int cr_socket_clientACMP();

int create_socket_serverTCP(int server_port);
//int cr_socket_serverUDP();
//int cr_socket_serverACMP();
