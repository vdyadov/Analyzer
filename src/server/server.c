#include "server.h"



void *comand_work(struct server_arg* parameters)
{

    int sock, listener;

    struct packet recv_packet;

    int length = sizeof(recv_packet);
    while (1)
    {
        listener = create_socket_serverTCP(parameters->server_port);
        if (listener < 0)
        {
            continue;
        }
        sock = accept(listener, NULL, NULL);
        close(listener);

        if (sock < 0)
        {
            perror("server_accept");
//            exit(3);
            close(listener);
            continue;
        }

        while(length)
        {
            length -= recv(sock, (char *)(&recv_packet + sizeof(recv_packet) - length), length, 0);
        }

        printf("\nlong_packet: %"PRIu32"\n", recv_packet.size_packet);
        printf("type_packet: %"PRIu16"\n", recv_packet.type_packet);
        printf("number_packet: %"PRIu32"\n", recv_packet.number_packet);
        printf("in_addr: %"PRIu32"\n", recv_packet.ip_client);
        printf("port: %"PRIu16"\n", recv_packet.port_client);
        printf("number_test: %"PRIu16"\n", recv_packet.number_test);

        close(sock);
    }

    return 0;

}

int server()
{
    pthread_t th[8];
    struct server_arg th_params[8];

    for (int i = 0; i < 8; i++)
    {
        th_params[i].number = i;
        th_params[i].server_port = 8888 + i;
    }

    for (int i = 0; i < 8; i++)
    {
        pthread_create(th + i, NULL, comand_work, th_params + i);
    }

    for (int i = 0; i < 8; i++)
    {
        pthread_join(th[i], NULL);
    }

    return 0;
}
