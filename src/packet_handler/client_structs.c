#include "client_structs.h"


const int SIZE_HDR = 18 + 20 + 8;

int create_packet_rtt(struct Packet_rtt *rtt_pack, int id, int size)
{
    int ret = 0;

    rtt_pack->id_packet = id;
    rtt_pack->payload = malloc(size - sizeof(Packet_rtt) - SIZE_HDR);

    if (NULL == rtt_pack->payload)
    {
        perror("Malloc failed");
        ret = 1;
        goto finally;
    }

    for (unsigned int i = 0; i < size - sizeof(Packet_rtt) - SIZE_HDR; i++)
    {
        rtt_pack->payload[i] = 65 + rand() % 60;
    }

 finally:

    return ret;
}

int create_packet_lose(struct Packet_lose *lose_pack, int round, int id_packet, int size)
{
    int ret = 0;
    
    lose_pack->id_round = round;
    lose_pack->id_packet = id_packet;
    lose_pack->payload = malloc(size - (2 * sizeof(Packet_lose) - SIZE_HDR));

    if (NULL == lose_pack->payload)
     {
        perror("Malloc failed");
        ret = 1;
        goto finally;
    }

    for (unsigned int i = 0; i < size - (2 * sizeof(Packet_lose) - SIZE_HDR); i++)
    {
        lose_pack->payload[i] = 65 + rand() % 60;
    }
        
 finally:

    return ret;
}

double get_cur_time_ms()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    double time_ms = (time.tv_sec * 1000.0) + (time.tv_nsec / 1000000.0);
    return time_ms;
}