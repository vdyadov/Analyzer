#include <time.h>
#include <stdlib.h>
#include "statistics_and_files.h"

int main() {

    struct Static_rtt statistic[10];
    srand(time(NULL));
    char name[] = "test";

    for (int i = 0; i < 10; i++) {

        statistic[i].id_packet = i + 1;
        statistic[i].latency_time = rand() % 500;
    }

    statistics_and_files(statistic, 10, name);

    return 0;
}
