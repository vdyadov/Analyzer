#include "../../include/create_socket.h"

int create_socket_clientTCP(int client_port, int server_ip)
{
    struct sockaddr_in addr;

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("socket_clientTCP");
        goto finally;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(client_port);
    addr.sin_addr.s_addr = htonl(server_ip);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect_clientTCP");
        close(sock);
        sock = -1;
        goto finally;
    }

    finally:
        return sock;
}


int create_socket_clientUDP(int client_port, int server_ip)
{
    struct sockaddr_in addr;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock < 0)
    {
        perror("socket_clientUDP");
        goto finally;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(client_port);
    addr.sin_addr.s_addr = htonl(server_ip);

    finally:
        return sock;
}

int create_socket_serverTCP(int server_port)
{

    struct sockaddr_in addr;

    int listener = socket(AF_INET, SOCK_STREAM, 0);

    if (listener < 0)
    {
        perror("socket_serverTCP");
        goto finally;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind_serverTCP");
        close(listener);
        listener = -1;
        goto finally;
    }
    listen(listener, 1);
    finally:
        return listener;
}

int create_socket_serverUDP(int server_port)
{
    struct sockaddr_in addr;

    int listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (listener < 0)
    {
        perror("socket_serverUDP");
        goto finally;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind_serverUDP");
        close(listener);
        listener = -1;
        goto finally;
    }
    listen(listener, 1);
    finally:
        return listener;
}
