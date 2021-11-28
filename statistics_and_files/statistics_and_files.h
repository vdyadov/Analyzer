#include <stdio.h>

struct Static_rtt
{
    int id_packet;
    double latency_time;
}__attribute__((packed)) Static_rtt;

int statistics_and_files(struct Static_rtt *statistic, int size, char *name);
