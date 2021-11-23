#include <stdio.h>

struct stat {
    int id_packet;
    double latency_time;
};

int statistics_and_files(struct stat *statistic, int size, char *name);
