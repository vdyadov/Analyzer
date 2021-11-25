#include "definitions.h"
#include "tcp_command_processor.h"

// To receive data from the client
#include "create_socket.h"
#include "packet_handler.h"


#define BUFFER_SIZE 1500


static void print_struct_packet_info(struct packet const *recv_packet);


int echo_receiver(struct client_socket const *const c_s)
{
    int ret = 0;
    unsigned char buffer[BUFFER_SIZE];
    ssize_t size_buf_data = 0;
    ssize_t num_send_data = 0;


    while(1)
    {
        errno = 0;
        size_buf_data = recv(c_s->fd_sock_client, buffer, sizeof(buffer), 0);
        if (0 == size_buf_data && 0 == errno)
        {
            printf("\nReceiver returned %ld or the client closed the connection,"
                    " socket %d\n",
                    size_buf_data,
                    c_s->fd_sock_client);
            break;
        }
        else if (-1 == size_buf_data)
        {
            perror("Error in recv(...)");
            printf("Socket %d\n", c_s->fd_sock_client);
            ret = -1;
            break;
        }

        printf("\nReceive %ld bytes, socket %d\n",
                size_buf_data,
                c_s->fd_sock_client);

        num_send_data = send(c_s->fd_sock_client, buffer, size_buf_data, 0);
        if (-1 == num_send_data)
        {
            perror("Error in send(...)");
            printf("Socket %d\n", c_s->fd_sock_client);
            ret = -1;
            break;
        }
        printf("Send %ld bytes, socket %d\n",
                num_send_data,
                c_s->fd_sock_client);
    }

    return ret;
}


// The function to receive data from the client
int receive_one_struct_packet(struct client_socket const *const c_s)
{
    int ret = 0;
    unsigned char buffer[BUFFER_SIZE];
    ssize_t size_buf_data = 0;

    struct packet *recv_packet;
    size_t total_length = 0;

    while(total_length < sizeof(struct packet))
    {
        errno = 0;
        size_buf_data = recv(c_s->fd_sock_client,
                buffer + total_length,
                sizeof(buffer), 0);
        if (0 == size_buf_data && 0 == errno)
        {
            printf("\nReceiver returned %ld or the client closed the connection,"
                    " socket %d\n",
                    size_buf_data,
                    c_s->fd_sock_client);
            break;
        }
        else if (-1 == size_buf_data)
        {
            perror("Error in recv(...)");
            printf("Socket %d\n", c_s->fd_sock_client);
            ret = -1;
            break;
        }

        total_length += size_buf_data;

        printf("\nReceive %ld bytes, total number of bytes received %ld (allowed %ld),"
                " client socket %d\n",
                size_buf_data,
                total_length,
                sizeof(struct packet),
                c_s->fd_sock_client);

        if (total_length > sizeof(struct packet))
        {
            printf("\nAn incorrect packet was received, the data size %ld"
                    " exceeds the allowed %ld, socket %d\n",
                    total_length,
                    sizeof(struct packet),
                    c_s->fd_sock_client);
            ret = -1;
            break;
        }
    }

    if (0 == ret && sizeof(struct packet) == total_length)
    {
        recv_packet = (struct packet *)buffer;

        printf("\n\n********* Accepted data on socket: %d\n",
                c_s->fd_sock_client);

        print_struct_packet_info(recv_packet);

        puts("*********  END of data\n");
    }

    return ret;
}


static void print_struct_packet_info(struct packet const *const recv_packet)
{
    struct in_addr recv_ip_addr = {0};

    recv_ip_addr.s_addr = ntohl(recv_packet->ip_client);

    printf("\nlong_packet: %"PRIu32"\n",
            recv_packet->size_packet);

    printf("type_packet: %"PRIu16"\n",
            recv_packet->type_packet);

    printf("in_addr: %s\n",
            inet_ntoa(recv_ip_addr));

    printf("port: %"PRIu16"\n",
            recv_packet->port_client);

    printf("number_test: %"PRIu16"\n",
            recv_packet->number_test);

}


// The function to receive data and packet_handler execution
int receive_wexec_packet_handler(struct client_socket const *const c_s)
{
    int ret = 0;
    unsigned char buffer[BUFFER_SIZE];
    ssize_t size_buf_data = 0;

    struct packet *recv_packet;
    size_t total_length = 0;

    while(total_length < sizeof(struct packet))
    {
        errno = 0;
        size_buf_data = recv(c_s->fd_sock_client,
                buffer + total_length,
                sizeof(buffer), 0);
        if (0 == size_buf_data && 0 == errno)
        {
            printf("\nReceiver returned %ld or the client closed the connection,"
                    " socket %d\n",
                    size_buf_data,
                    c_s->fd_sock_client);

            total_length = 0;
            break;
        }
        else if (-1 == size_buf_data)
        {
            perror("Error in recv(...)");
            printf("Socket %d\n", c_s->fd_sock_client);
            
            ret = -1;
            break;
        }

        total_length += size_buf_data;

        if (total_length > sizeof(struct packet))
        {
            printf("\nAn incorrect packet was received, the data size %ld"
                    " exceeds the allowed %ld, socket %d\n",
                    total_length,
                    sizeof(struct packet),
                    c_s->fd_sock_client);
            
            ret = -1;
            break;
        }
    }

    if (0 == ret && sizeof(struct packet) == total_length)
    {
        recv_packet = (struct packet *)buffer;

        printf("\n\n********* Accepted data on socket: %d\n",
                c_s->fd_sock_client);

        print_struct_packet_info(recv_packet);

        puts("*********  END of data\n");


        recv_packet->port_client = 0;

        ret = packet_handler_server(recv_packet, c_s->fd_sock_client);
    }

    return ret;
}
