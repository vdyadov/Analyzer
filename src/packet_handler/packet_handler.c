#include "create_socket.h"
#include "packet_handler.h"
#include "client_structs.h"


int rtt_test(int sock, struct sockaddr_in addr, struct packet *original_packet);
int rtt_test_server(int sock, struct sockaddr_in addr);

int lose_test(int sock, struct sockaddr_in addr, struct packet *original_packet);
int lose_test_server(int sock, struct sockaddr_in addr);

static int sending_client_test_sock(
        struct packet *original_packet,
        int fd_command_sock,
        int fd_test_sock);


int packet_handler_client(struct packet *original_packet)
{

    int sock;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(original_packet->port_client);
    addr.sin_addr.s_addr = htonl(original_packet->ip_client);

    switch (original_packet->type_packet)
    {
        case UDP:
            sock = create_socket_clientUDP();
            //sock = create_socket_clientUDP(original_packet->port_client, original_packet->ip_client);
//------------------------------------------------------------------------------

            if(original_packet->number_test == 1)
            {
                rtt_test(sock, addr, original_packet);
            } 
            else if(original_packet->number_test == 2)
            {
                lose_test(sock, addr, original_packet);
            }

//------------------------------------------------------------------------------
            close(sock);

        break;
        case TCP:
            sock = create_socket_clientTCP(original_packet->port_client, original_packet->ip_client);
//------------------------------------------------------------------------------
/*
//            send(sock, (char *)&send_message, sizeof(send_message));
//            while(length)
//            }
//                length -= recv(sock, (char *)(&recv_message + sizeof(recv_message) - length), length, 0);
//            }
//            printf("%d\n\n", recv_message);

            if(original_packet->number_test == 2) {
                test_2(&sock);
            }
*/
//------------------------------------------------------------------------------
            close(sock);
        break;
/*        case '2':
            sock = create_socket_clientICMP(original_packet->client_port);
            int send_message = 22;
            int recv_message = 0;
            int length = sizeof(send_message);

            sendto(sock, (char *)&send_message, sizeof(send_message));
            recvfrom(sock, &recv_message, sizeof(recv_message), 0, , );
            printf("%d\n\n", recv_message);
            close(sock);

        break;
*/
    }

    return 0;
}

int rtt_test(int sock, struct sockaddr_in addr, struct packet *original_packet)
{
    int count_packet = original_packet->count_packet;
    int size = original_packet->size_packet;
    int ret = 0;
    struct Packet_rtt rtt_pack = {0};
    struct Packet_rtt new_rtt_pack = {0};

    struct Struct_rtt client_static[count_packet];

    char *buffer = NULL;

    buffer = malloc(size);

    struct timeval tv;

    int timeout = 2000;

    int send_len = 0;
    int recv_len = 0;

    socklen_t socklen = sizeof(struct sockaddr_in);

    struct Info_rtt info_rtt;

    info_rtt.size = size;
    info_rtt.count_packet = count_packet;

    send_len = sendto(sock, (char *)&info_rtt, sizeof(info_rtt), 0, (struct sockaddr*)&addr, socklen);

    create_packet_rtt(&rtt_pack, 0, size);

    for (int i = 0; i < count_packet; i++)
    {
        fd_set rfd;
        FD_ZERO(&rfd);
        FD_SET(sock, &rfd);

        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;

        // create_packet_rtt(&rtt_pack, i+1, size);
        rtt_pack.id_packet++;
        memcpy(buffer, (char *)&rtt_pack, size);

        send_len = sendto(sock, buffer, size, 0, (struct sockaddr*)&addr, socklen);
        if (size > send_len)
        {
            perror("sendto failed");
            client_static[i].client_send_time = 0;
            client_static[i].client_recv_time = 0;
            ret = 2;
            goto finally;
        }
        
        client_static[i].client_send_time = get_cur_time_ms();

        int n = select(sock + 1, &rfd, 0, 0, &tv);

        if (n == 0) // выходим из цикла по таймауту
        {
            client_static[i].client_recv_time = 0;
            client_static[i].latency_time = timeout;
            continue; 
        }
        if (n < 0) // произошла ошибка
        {
            perror("select failed");
            client_static[i].client_recv_time = 0;
            client_static[i].latency_time = 0;
            ret = 3;
            goto finally; 
        }

        client_static[i].client_recv_time = get_cur_time_ms();

        recv_len = recvfrom(sock, (char *)&new_rtt_pack, size, MSG_WAITALL, (struct sockaddr*)&addr, &socklen);
        if (size > recv_len)
        {
            perror("recvfrom failed");
            client_static[i].client_recv_time = 0;
            client_static[i].latency_time = 0;
            ret = 4;
            goto finally;
        }

        if (strcmp((char *)&rtt_pack, (char *)&new_rtt_pack))
        {
            printf("packet isn't corret\n");
        }

        client_static[i].id_packet = new_rtt_pack.id_packet;

        client_static[i].latency_time = client_static[i].client_recv_time - client_static[i].client_send_time;

        usleep(1000000);

        printf("Pack size: %d\t", size);
        printf("id pack: %" PRIu16 "\t", new_rtt_pack.id_packet);
        printf("Time: %.3f\n", client_static[i].latency_time);
    }

 finally:

    if (-1 != sock)
    {
        close(sock);
        sock = -1;
    }
    free(rtt_pack.payload);
    free(buffer);

    return ret;
}

int lose_test(int sock, struct sockaddr_in addr, struct packet *original_packet)
{
    int count_packet = original_packet->count_packet;
    int size = original_packet->size_packet;
    uint32_t time = original_packet->time_test;
    //------------------------
    int ret = 0;
    int id_round = 0;
    int id_packet = 1;
    int err_id_pack = -1;
    int send_len = 0;
    int recv_len = 0;
    struct Packet_lose lose_pack = {0};

    char *buffer = NULL;
    buffer = malloc(size);

    struct Info_lose info_lose;

    info_lose.size = size;
    info_lose.count_packet = count_packet;

    socklen_t socklen = sizeof(struct sockaddr_in);

    send_len = sendto(sock, (char *)&info_lose, sizeof(info_lose), 0, (struct sockaddr*)&addr, socklen);

    int timeout = time / count_packet;

    create_packet_lose(&lose_pack, id_round, id_packet, size);

    for (int i = 0; i < count_packet; i++)
    {
        // create_packet_lose(&lose_pack, id_round, id_packet, size);

        lose_pack.id_round = id_round;
        lose_pack.id_packet = id_packet;
        memcpy(buffer, (char *)&lose_pack, size);

        send_len = sendto(sock, buffer, size, 0, (struct sockaddr*)&addr, socklen);
        if (size > send_len)
        {
            perror("sendto failed");
            ret = 2;
            goto finally;
        }

        recv_len = recvfrom(sock, (char *)&err_id_pack, sizeof(err_id_pack), 0, (struct sockaddr*)&addr, &socklen);
        if ((int)sizeof(err_id_pack) > recv_len)
        {
            perror("recvfrom failed");
            ret = 4;
            goto finally;
        }

        if (id_packet != err_id_pack)
        {
            id_round++;
            id_packet--;
        }

        id_packet++;

        sleep(timeout);

        printf("\nid_round: %" PRIu16 "\n", lose_pack.id_round);
        printf("id pack: %" PRIu16 "\n", lose_pack.id_packet);
        // printf("err_id_pack: %d\n", err_id_pack);
    }

    if (0 != id_round)
    {
        id_round--;
    }

    printf("Было передано: %" PRIu16 " пакетов!\n", count_packet);
    printf("Было потеряно: %" PRIu16 " пакетов!\n", id_round);

 finally:

    if (-1 != sock)
    {
        close(sock);
        sock = -1;
    }
    free(lose_pack.payload);
    free(buffer);

    return ret;
}


int packet_handler_server(
        struct packet *const original_packet,
        int const fd_command_sock)
{
    int ret = 0;

    int sock, listener;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(original_packet->port_client);
    addr.sin_addr.s_addr = htonl(original_packet->ip_client);

    switch (original_packet->type_packet)
    {
        case UDP:
        {
            sock = create_socket_serverUDP(original_packet->port_client);
            if (sock < 0)
            {
                ret = -1;
                goto finally;
            }

            ret = sending_client_test_sock(
                    original_packet,
                    fd_command_sock,
                    sock);

            if (-1 == ret)
            {
                close(sock);
                goto finally;
            }

            if (original_packet->number_test == 1)
            {
                rtt_test_server(sock, addr);
            } 
            else if (original_packet->number_test == 2)
            {
                lose_test_server(sock, addr);
            }

            close(sock);
            break;
        }
        case TCP:
        {
            listener = create_socket_serverTCP(original_packet->port_client);

            if (listener < 0)
            {
                ret = -1;
                goto finally;
            }

            ret = sending_client_test_sock(
                    original_packet,
                    fd_command_sock,
                    listener);

            if (-1 == ret)
            {
                close(listener);
                goto finally;
            }

            sock = accept(listener, NULL, NULL);

            close(listener);
            close(sock);
            break;
        }
        default:
        {
            break;
        }
    }

 finally:

    return ret;
}


static int sending_client_test_sock(
        struct packet *const original_packet,
        int const fd_command_sock,
        int const fd_test_sock)
{
    int ret = 0;
    struct sockaddr_in server_addr = {0};
    socklen_t server_addrlen = sizeof(struct sockaddr_in);
    ssize_t size_send_data = 0;

    ret = getsockname(
            fd_test_sock,
            (struct sockaddr *)&server_addr,
            &server_addrlen);
    if (-1 == ret)
    {
        perror("Error in function getsockname(...),"
                " funcion sending_client_test_sock(...)");
        goto finally;
    }
    
    original_packet->port_client = ntohs(server_addr.sin_port);

    size_send_data = send(fd_command_sock,
            original_packet,
            sizeof(struct packet),
            0);

    if (-1 == size_send_data || sizeof(struct packet) != (size_t)size_send_data)
    {
        printf("\nError! Don't sent command packet to client, socket %d\n",
                fd_command_sock);

        ret = -1;
        goto finally;
    }

 finally:

    return ret;
}


int rtt_test_server(int sock, struct sockaddr_in addr)
{
    int ret = 0;
    char *buffer = NULL;
    struct Info_rtt info_rtt = {0};

    unsigned int recv_len = 0;
    socklen_t socklen = sizeof(struct sockaddr_in);

//    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    recv_len = recvfrom(sock, (char *)&info_rtt, sizeof(info_rtt), MSG_WAITALL, (struct sockaddr *)&addr, &socklen);
    if (recv_len < (int)sizeof(info_rtt))
    {
        perror("recvfrom failed");
        ret = 2;
        goto finally;
    }

    buffer = malloc(info_rtt.size);
    if (NULL == buffer)
    {
        perror("malloc failed");
        ret = 3;
        goto finally;
    }

    for(int i = 1; i <= info_rtt.count_packet; i++)
    {
        recvfrom(sock, buffer, info_rtt.size, MSG_WAITALL, (struct sockaddr *)&addr, &socklen);
        sendto(sock, buffer, info_rtt.size, 0, (struct sockaddr*)&addr, socklen);
    }

    // printf("recv bytes: %d\n", recv_len);
    // printf("send bytes: %d\n", send_len);
    
 finally:

    if (-1 != sock)
    {
        close(sock);
        sock = -1;
    }

    free(buffer);

    return ret;
}

int lose_test_server(int sock, struct sockaddr_in addr)
{
    int ret = 0;
    int id_packet = 0;
    char *buffer = NULL;
    struct Info_lose info_lose = {0};
    struct Packet_lose lose_pack = {0};
    struct Statistic_lose *stat_lose = NULL;

    struct timeval tv;
    int timeout = 2000;
    int send_len = 0;
    int recv_len = 0;
    socklen_t socklen = sizeof(struct sockaddr_in);

//    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    recv_len = recvfrom(sock, (char *)&info_lose, sizeof(info_lose), 0, (struct sockaddr *)&addr, &socklen);
    if ((int)sizeof(info_lose) > recv_len)
    {
        perror("recvfrom failed");
        ret = 2;
        goto finally;
    }

    stat_lose = malloc(info_lose.count_packet * sizeof(struct Statistic_lose));

    buffer = malloc(info_lose.size);
    if (NULL == buffer)
    {
        perror("malloc failed");
        ret = 3;
        goto finally;
    }

    for(int i = 0; i < info_lose.count_packet; i++)
    {
        fd_set rfd;
        FD_ZERO(&rfd);
        FD_SET(sock, &rfd);
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;

        int n = select(sock + 1, &rfd, 0, 0, &tv);
        if (n == 0) // переходим на следующую итерацию по таймауту
        {
            i--;
            send_len = sendto(sock, (char *)&id_packet, sizeof(id_packet), 0, (struct sockaddr*)&addr, socklen);
            continue; 
        }
        if (n < 0) // произошла ошибка
        {
            perror("select failed");
            ret = 3;
            goto finally; 
        }

        recv_len = recvfrom(sock, buffer, info_lose.size, 0, (struct sockaddr *)&addr, &socklen);
        if ((int)sizeof(info_lose.size) > recv_len)
        {
            perror("recvfrom failed");
            ret = 4;
            goto finally;
        }
        memcpy((char *)&lose_pack, buffer, sizeof(lose_pack));

        id_packet = lose_pack.id_packet;

        if (id_packet == lose_pack.id_packet) 
        {
            stat_lose[i].id_packet = lose_pack.id_packet;
            stat_lose[i].id_round = lose_pack.id_round;
        }

        send_len = sendto(sock, (char *)&id_packet, sizeof(id_packet), 0, (struct sockaddr*)&addr, socklen);
        if ((int)sizeof(id_packet) > send_len)
        {
            perror("recvfrom failed");
            ret = 4;
            goto finally;
        }
    }
    
    printf("Было потеряно: %" PRIu16 " пакетов!\n", stat_lose[info_lose.count_packet-1].id_round);
    printf("Было принято: %" PRIu16 " пакетов!\n", stat_lose[info_lose.count_packet-1].id_packet);

 finally:

    if (-1 != sock)
    {
        close(sock);
        sock = -1;
    }

    free(buffer);
    free(stat_lose);

    return ret;
}
