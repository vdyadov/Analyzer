#include "../../include/packet_handler.h"
#include "../../include/client_structs.h"

int rtt_test(int sock, struct sockaddr_in addr, struct packet *original_packet);
int rtt_test_server(int sock, struct sockaddr_in addr);

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
            sock = create_socket_clientUDP(original_packet->port_client, original_packet->ip_client);
//------------------------------------------------------------------------------

            if(original_packet->number_test == 1)
            {
                rtt_test(sock, addr, original_packet);
            } 
            // else if(original_packet->number_test == 2)
            // {
            //     test_2(&sock, &addr);
            // }

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

    for (int i = 0; i < count_packet; i++)
    {
        fd_set rfd;
        FD_ZERO(&rfd);
        FD_SET(sock, &rfd);

        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;

        create_packet_rtt(&rtt_pack, i+1, size);
        memcpy(buffer, (char *)&rtt_pack, sizeof(rtt_pack));

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

        usleep(10000);

        printf("\nid pack: %" PRIu16 "\n", new_rtt_pack.id_packet);
        printf("id pack: %s\n", new_rtt_pack.payload);
        printf("latency_time: %.3f\n", client_static[i].latency_time);
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

int packet_handler_server(struct packet *original_packet)
{

    int sock, listener;
    struct sockaddr_in addr;
    // socklen_t socklen = sizeof(struct sockaddr_in);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(original_packet->port_client);
    addr.sin_addr.s_addr = htonl(original_packet->ip_client);

    switch (original_packet->type_packet)
    {
        case UDP:
            sock = create_socket_serverUDP(original_packet->port_client);

            // length = sizeof(send_message);
//------------------------------------------------------------------------------


//            recvfrom(sock, (char *)&recv_message, sizeof(recv_message), (struct sockaddr_in)&addr, &socklen);
//            sendto(sock, (char *)&send_message, sizeof(send_message), (struct sockaddr_in)&addr, socklen);
//            printf("%d\n\n", recv_message);

            // if (original_packet->number_test == 1)
            // {
                rtt_test_server(sock, addr);
            // } else if (original_packet->number_test == 2)
            // {
            //     test_2(&sock, &addr);
            // } else
            // {
            //     test_3(&sock, &addr);
            // }



//------------------------------------------------------------------------------
            close(sock);

        break;
        case TCP:
            listener = create_socket_serverTCP(original_packet->port_client);
            sock = accept(listener, NULL, NULL);
            close(listener);

            // length = sizeof(send_message);
//------------------------------------------------------------------------------
/*
//            while(length)
//            }
//                length -= recv(sock, (char *)(&recv_message + sizeof(recv_message) - length), length, 0);
//            }
//            send(sock, (char *)&send_message, sizeof(send_message));
//            printf("%d\n\n", recv_message);

            if(original_packet->number_test == 0) {
                test_2(&sock);
            }

*/
//------------------------------------------------------------------------------
            close(sock);
        break;
    }

    return 0;
}

int rtt_test_server(int sock, struct sockaddr_in addr)
{
    int ret = 0;
    char *buffer = NULL;
    struct Info_rtt info_rtt = {0};

    unsigned int send_len = 0;
    unsigned int recv_len = 0;
    socklen_t socklen = sizeof(struct sockaddr_in);

    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    recv_len = recvfrom(sock, (char *)&info_rtt, sizeof(info_rtt), MSG_WAITALL, (struct sockaddr *)&addr, &socklen);
    if (recv_len < sizeof(info_rtt))
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
        recv_len = recvfrom(sock, buffer, info_rtt.size, MSG_WAITALL, (struct sockaddr *)&addr, &socklen);
        send_len = sendto(sock, buffer, info_rtt.size, 0, (struct sockaddr*)&addr, socklen);
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