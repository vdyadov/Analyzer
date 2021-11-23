#include "statistics_and_files.h"

int statistics_and_files(struct stat *statistic, int size, char *name)
{

    FILE *fd;

    int learn = strlen(name);
    learn = learn + 6;
    char file_name[learn];
    sprintf(file_name, "%s.json", name);

    fd = fopen(file_name, "w");

    fprintf(fd, "{\n\t\"statistic\": [\n");

    for (int i = 0; i < size; i++)
    {
        fprintf(fd, "\t\t{\n\t\t\t\"number_of_packet\": %d,\n\t\t\t\"time\": %f\n\t\t}", statistic[i].id_packet, statistic[i].latency_time);
        if (i < size - 1)
            fprintf(fd, ",");
        fprintf(fd, "\n");
    }

    fprintf(fd, "\t]\n}");

    close(fd);
    return 0;
}
