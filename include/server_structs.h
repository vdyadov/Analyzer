#ifndef SERVER_STRUCTS_H
#define SERVER_STRUCTS_H

#include <stdint.h>

typedef struct Struct_rtt
{ 
    uint64_t server_recv_time;
    uint64_t server_send_time;
    uint16_t n_packet;
} Struct_rtt;

typedef struct Packet_rtt
{
    uint16_t n_packet;
    char *payload;
} Packet_rtt;

typedef struct Struct_lose_packet
{
    uint64_t server_recv_time;
    uint16_t n_round;
    uint16_t n_packet;
} Struct_lose_packet;

typedef struct Packet_lose
{
    uint16_t n_round;
    uint16_t n_packet;
    char *payload;
} Packet_lose;

int set_packet_rtt(Packet_rtt *, int, char *);

int set_packet_lose(Packet_lose *, int, int, char *);

#endif /* SERVER_STRUCTS_H */
