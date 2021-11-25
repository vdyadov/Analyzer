#ifndef _CLIENT_STRUCTS_H_
#define _CLIENT_STRUCTS_H_

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct Struct_rtt
{ 
    double client_recv_time;
    double client_send_time;
    double latency_time;
    uint16_t id_packet;
} __attribute__((packed)) Struct_rtt; 

typedef struct Packet_rtt
{
    uint16_t id_packet;
    char *payload;
} __attribute__((packed)) Packet_rtt; 

typedef struct Info_rtt
{
    int size;
    int count_packet;
} __attribute__((packed)) Info_rtt;

typedef struct Statistic_rtt
{
    uint16_t id_packet;
    uint64_t latency_time;
} __attribute__((packed)) Statistic_rtt;

typedef struct Struct_lose_packet
{
    uint64_t client_send_time;
    uint16_t id_round;
    uint16_t id_packet;
} __attribute__((packed)) Struct_lose_packet;

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

int create_packet_rtt(struct Packet_rtt *rtt_pack, int id, int size);

int create_packet_lose(struct Packet_lose *lose_pack, int round, int id_packet, int size);

double get_cur_time_ms();

#endif /* _CLIENT_STRUCTS_H_ */
