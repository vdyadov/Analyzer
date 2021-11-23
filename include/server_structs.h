#ifndef SERVER_STRUCTS_H
#define SERVER_STRUCTS_H

#include <stdint.h>

typedef struct Struct_rtt
{
    uint64_t latency_time;
    uint16_t id_packet;
} __attribute__((packed)) Struct_rtt; 

typedef struct Info_rtt
{
    int size;
    int count_packet;
} __attribute__((packed)) Info_rtt;

typedef struct Struct_lose_packet
{
    uint64_t server_recv_time;
    uint16_t id_round;
    uint16_t id_packet;
} Struct_lose_packet;

// int set_struct_rtt(struct Struct_rtt *, int, char *);

// int set_packet_lose(Packet_lose *, int, int, char *);

#endif /* SERVER_STRUCTS_H */
