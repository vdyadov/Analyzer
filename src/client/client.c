#include "client.h"

/** client -n 1 -s 1024 -c 100 -t 60 -ip 127.0.0.1
*   n - номер теста
*   s - размер пакета
*   с - количество пакетов
*   t - время выполнения
*   ip - ip-адрес сервера
*/

int client(int argc, char *argv[])
{

    int client_port = 15200;
    //int client_port = 8888;

    struct packet send_packet;

    char *str_number_test = NULL;
    char *str_size_packet = NULL;
    char *str_count_packet = NULL;
    char *str_time_test = NULL;
    char *str_server_ip = NULL;

    int number_test = 0;
    int size_packet = 0;
    int count_packet = 0;
    int time_test = 0;

    puts("Attention! Для того, чтобы клиент заработал, установи на сервере"
            " слушающий (listen) порт из диапазона: 15200 - 15207\n\n");

    // Обработка аргументов от пользователя
    for (int i = 1; i < argc; i += 2) 
    {
        switch (argv[i][1]) 
        {
            case 'n':
                str_number_test = argv[i + 1];
                number_test = atoi(str_number_test);
                break;
            case 's':
                str_size_packet = argv[i + 1];
                size_packet = atoi(str_size_packet);
                break;
            case 'c':
                str_count_packet = argv[i +1];
                count_packet = atoi(str_count_packet);
                break;
            case 't':
                str_time_test = argv[i + 1];
                time_test = atoi(str_time_test);
                break;
            case 'i':
                str_server_ip = argv[i + 1];
                break;
            default:
                printf("Вы ввели неправильные ключи\n");
                break;
        }
    }

    // Проверка на ввод значений в переменные
    if (NULL == str_number_test)
    {
        str_number_test = malloc(3);
        printf("Тесты:\n"
                "\t 1 - нарушение последовательности передачи пакетов\n"
                "\t 2 - тест RTT\n"
                "\t 3 - остаточная полоса канала (100%% загрузка)\n"
                "Введите номер теста: ");
        fgets(str_number_test, 3, stdin);
        number_test = atoi(str_number_test);
        free(str_number_test);
    }
    if (NULL == str_size_packet)
    {
        str_size_packet = malloc(10);
        printf("Введите размер пакета (64, 128, 256, 512, 1024, 1280, 1518): ");
        fgets(str_size_packet, 10, stdin);
        size_packet = atoi(str_size_packet);
        free(str_size_packet);
    }
    if (3 != number_test && NULL == str_count_packet)
    {
        str_count_packet = malloc(10);
        printf("Введите количество пакетов: ");
        fgets(str_count_packet, 10, stdin);
        count_packet = atoi(str_count_packet);
        free(str_count_packet);
    }
    if (3 == number_test && NULL == str_time_test)
    {
        str_time_test = malloc(10);
        printf("Введите время теста в секундах: ");
        fgets(str_time_test, 10, stdin);
        time_test = atoi(str_time_test);
        free(str_time_test);
    }
    if (NULL == str_server_ip)
    {
        str_server_ip = malloc(16);
        printf("Введите IPv4-адрес сервера в формате xxx.xxx.xxx.xxx: ");
        fgets(str_server_ip, 16, stdin);
    }

    int type_packet = 0;

    if (1 == number_test || 2 == number_test)
    {
        type_packet = UDP;
    }

//Заполняем пакет
    send_packet.size_packet = size_packet;
    send_packet.type_packet = type_packet;
    send_packet.number_packet = 1;
    send_packet.ip_client = INADDR_LOOPBACK;
    send_packet.port_client = client_port;
    send_packet.number_test = number_test;

//Создаём сокет
    int sock = -1;

    printf("\nAttempt to connect to the server port: %d\n", client_port);

    sock = create_socket_clientTCP(client_port, str_server_ip);

    while (sock < 0 && client_port < 15207)
    {
        client_port++;

        printf("\nAttempt to connect to the server port: %d\n", client_port);

        send_packet.port_client = client_port;
        sock = create_socket_clientTCP(client_port, str_server_ip);
    }

//Пересылка пакета
    if(sock >= 0) {
        printf("The connection with the server on the port %d is successful!\n"
                "Sending data!\n", client_port);
        send(sock, (char *)&send_packet, sizeof(send_packet), 0);
    }

    free(str_server_ip);
    close(sock);

    return 0;
}
