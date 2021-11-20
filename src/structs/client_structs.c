#include <client_structs.h>

int create_packet_rtt(struct Packet_rtt *rtt_pack, int lenght)
{
    int ret = 0;

    rtt_pack->id_pack = ++id;
    rtt_pack->payload = malloc(size * sizeof(char));

    if (NULL == rtt_pack->payload)
    {
        perror("Malloc failed");
        ret = 1;
        goto finally;
    }

    for (int i = 0; i < lenght; i++)
    {
        rtt_pack->payload[i] = i;
    }

 finally:

    return ret;
}

int create_packet_lose(struct Packet_lose *lose_pack, int round, int lenght)
{
    int ret = 0;
    
    lose_pack->id_pack = ++id;
    lose_pack->id_round = round;
    lose_pack->payload = malloc(size * sizeof(char));

    if (NULL == lose_pack->payload)
     {
        perror("Malloc failed");
        ret = 1;
        goto finally;
    }

    for (int i = 0; i < lenght; i++)
    {
        lose_pack->payload[i] = i;
    }
        
 finally:

    return ret;
}
