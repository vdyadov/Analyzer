#include "create_socket.h"

int create_socket_clientTCP(int client_port, char *server_ip)
{
    struct sockaddr_in addr;

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("socket_client");
        goto finally;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(client_port);
    inet_aton(server_ip, (struct in_addr *)&addr.sin_addr.s_addr);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect_client");
        close(sock);
        sock = -1;
        goto finally;
    }

    finally:
        return sock;
}


/*int cr_socket_clientUDP() {


}

int cr_socket_clientACMP() {


}*/

int create_socket_serverTCP(int server_port)
{

    struct sockaddr_in addr;

    int listener = socket(AF_INET, SOCK_STREAM, 0);

    if (listener < 0)
    {
        perror("socket_server");
        goto finally;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind_server");
        close(listener);
        listener = -1;
        goto finally;
    }
    listen(listener, 1);
    finally:
        return listener;
}

/*int cr_socket_serverUDP() {


}

int cr_socket_serverACMP() {


}*/
