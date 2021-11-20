#include <client_structs.h>

int create_packet_rtt(Packet_rtt *rtt_pack, int lenght)
{
    rtt_pack->id_pack = ++id;
    rtt_pack->payload = malloc(size * sizeof(char));

    if (NULL == rtt_pack->payload)
    {
        perror("Malloc failed");
        return 1;
    }

    for (int i = 0; i < lenght; i++)
    {
        rtt_pack->payload[i] = 65 + rand() % 60;
    }
        
    return 0;
}

int create_packet_lose(Packet_lose *lose_pack, int round, int lenght)
{
    lose_pack->id_pack = ++id;
    lose_pack->id_round = round;
    lose_pack->payload = malloc(size * sizeof(char));

    if (NULL == lose_pack->payload)
    {
        perror("Malloc failed");
        return 1;
    }

    for (int i = 0; i < lenght; i++)
    {
        lose_pack->payload[i] = 65 + rand() % 60;
    }
        
    return 0;
}

