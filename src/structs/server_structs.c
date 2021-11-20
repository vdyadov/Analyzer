#include <client_structs.h>

int set_packet_rtt(Packet_rtt *rtt_pack, int id_pack, char *payload)
{
    rtt_pack->id_packet = id_pack;
    rtt_pack->payload = payload;

    return 0;
}

int set_packet_lose(Packet_lose *lose_pack, int id_round, int id_pack, char *payload)
{
    lose_pack->id_round = id_pack;
    lose_pack->id_packet = id_pack;
    lose_pack->payload = payload;

    return 0;
}

