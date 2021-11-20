#include <client_structs.h>

int set_packet_rtt(Packet_rtt *rtt_pack, const int id_pack, char * const payload)
{
    int ret = 0;
    
    rtt_pack->id_packet = id_pack;
    
    if (NULL == payload)
    {
        ret = 1;
        goto finally;
    }

    rtt_pack->payload = payload;

 finally:

    return ret;
}

int set_packet_lose(Packet_lose *lose_pack, const int id_round, const int id_pack, 
                    char * const payload)
{
    int ret = 0;
    
    lose_pack->id_round = id_pack;
    lose_pack->id_packet = id_pack;
    
    if (NULL == payload)
    {
        ret = 1;
        goto finally;
    }

    lose_pack->payload = payload;

 finally:

    return ret;
}

