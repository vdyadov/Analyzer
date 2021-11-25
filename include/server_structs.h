#ifndef _SERVER_STRUCTS_H_
#define _SERVER_STRUCTS_H_

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

typedef struct Packet_lose
{
    uint16_t id_round;
    uint16_t id_packet;
    char *payload;
} __attribute__((packed)) Packet_lose; 

typedef struct Info_lose
{
    int size;
    int count_packet;
} __attribute__((packed)) Info_lose;

typedef struct Statistic_lose
{
    uint16_t id_round;
    uint16_t id_packet;
    uint64_t recv_time;
} __attribute__((packed)) Statistic_lose;

#endif /* _SERVER_STRUCTS_H_ */
