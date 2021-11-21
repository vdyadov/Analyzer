#ifndef CLIENT_STRUCTS_H
#define CLIENT_STRUCTS_H

#include <stdint.h>
#include <malloc.h>

int id = 0;

typedef struct Struct_rtt
{ 
    uint64_t client_recv_time;
    uint64_t client_send_time;
    uint16_t id_packet;
} Struct_rtt; 

typedef struct Packet_rtt
{
    uint16_t id_packet;
    char *payload;
} Packet_rtt; 

typedef struct Struct_lose_packet
{
    uint64_t client_send_time;
    uint16_t id_round;
    uint16_t id_packet;
} Struct_lose_packet;

typedef struct Packet_lose
{
    uint16_t id_round;
    uint16_t id_packet;
    char *payload;
} Packet_lose; 

int create_packet_rtt(Packet_rtt *, int);

int create_packet_lose(Packet_lose *, int, int);

#endif /* CLIENT_STRUCTS_H */
