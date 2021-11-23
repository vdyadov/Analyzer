#include "../include/client_structs.h"

int create_packet_rtt(struct Packet_rtt *rtt_pack, int id, int size)
{
    int ret = 0;

    rtt_pack->id_packet = id;
    rtt_pack->payload = malloc(size * sizeof(char));

    if (NULL == rtt_pack->payload)
    {
        perror("Malloc failed");
        ret = 1;
        goto finally;
    }

    for (unsigned int i = 0; i < size - sizeof(uint16_t); i++)
    {
        rtt_pack->payload[i] = 65 + rand() % 60;
    }

 finally:

    return ret;
}

int create_packet_lose(struct Packet_lose *lose_pack, int id, int round, int size)
{
    int ret = 0;
    
    lose_pack->id_packet = id;
    lose_pack->id_round = round;
    lose_pack->payload = malloc(size * sizeof(char));

    if (NULL == lose_pack->payload)
     {
        perror("Malloc failed");
        ret = 1;
        goto finally;
    }

    for (int i = 0; i < size - 2; i++)
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