#include "client.h"



int client()
{

    int client_port = 8888;
    struct packet send_packet;

//Вводим порт

//    printf("Введите порт: ");
//    scanf("%d", &client_port);

//Заполняем пакет

    send_packet.size_packet = 64;
    send_packet.type_packet = UDP;
    send_packet.number_packet = 1;
    send_packet.ip_client = INADDR_LOOPBACK;
    send_packet.port_client = client_port;
    send_packet.number_test = 0;

//Создаём сокет

    int sock;

    sock = create_socket_clientTCP(client_port);

    while (sock < 0 || client_port == 8897)
    {
        client_port++;
        send_packet.port_client = client_port;
        sock = create_socket_clientTCP(client_port);
    }

//Пересылка пакета
    if(sock >= 0) {
        send(sock, (char *)&send_packet, sizeof(send_packet), 0);
    }

    close(sock);

    return 0;

}
