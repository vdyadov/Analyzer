#include "definitions.h"
#include "thread_tcp_socket.h"


#define LISTEN_QUEUE_LEN 5


/** Extern public global variables.
 * Volatile flag for Ctr-C cansel thread.
 * Set non-zero after SIGINT.
 */
extern volatile sig_atomic_t stop_all_thr;


/** Interrupt threads on a signal SIGINT
 */
static void handler()
{
    stop_all_thr = 1;
}


int main(int argc, char *argv[])
{
    int ret = 0;
    int fd_soc = -1;

    struct sockaddr_in server_in = {0};
    int server_num_port = 0;
    //struct in_addr addr = {0};

    // Signals
    struct sigaction sa = {0};


    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_port>\n", argv[0]);
        ret = EXIT_FAILURE;
        goto finally;
    }

    server_num_port = atoi(argv[1]);

    if (server_num_port < 1024 || server_num_port > 49151)
    {
        fprintf(stderr, "Invalid port number (must be in the range: 1024-49151)\n");
        fprintf(stderr, "Usage: %s <server_port> <server_address_IPv4>\n", argv[0]);
        ret = EXIT_FAILURE;
        goto finally;
    }

/*
    if (0 == inet_aton(argv[2], &addr))
    {
        fprintf(stderr, "Invalid IP_v4 address (must be xxx.xxx.xxx.xxx)\n");
        fprintf(stderr, "Usage: %s <server_port> <server_address_IPv4>\n", argv[0]);
        ret = EXIT_FAILURE;
        goto finally;
    }
*/

    memset(&server_in, 0, sizeof(struct sockaddr_in));
    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(server_num_port);
    //server_in.sin_addr.s_addr = addr.s_addr;
    server_in.sin_addr.s_addr = htonl(INADDR_ANY);

    fd_soc = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (-1 == fd_soc)
    {
        perror("Error in socket(...)");
        ret = EXIT_FAILURE;
        goto finally;
    }

    int opt_val = 1;
    if (-1 == setsockopt(fd_soc, SOL_SOCKET, SO_KEEPALIVE, &opt_val, sizeof(opt_val)))
    {
        perror("Error in setsockopt(...) for SO_KEEPALIVE");
        ret = EXIT_FAILURE;
        goto finally;
    }

    if (-1 == setsockopt(fd_soc, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)))
    {
        perror("Error in setsockopt(...) for SO_REUSEADDR");
        ret = EXIT_FAILURE;
        goto finally;
    }

    if (-1 == bind(fd_soc, (struct sockaddr *)&server_in, sizeof(struct sockaddr_in)))
    {
        perror("Error in bind(...)");
        ret = EXIT_FAILURE;
        goto finally;
    }

    if (-1 == listen(fd_soc, LISTEN_QUEUE_LEN))
    {
        perror("Error in listen(...)");
        ret = EXIT_FAILURE;
        goto finally;
    }

    puts("Binding OK! Port listen!");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (-1 == sigaction(SIGINT, &sa, NULL))
    {
        perror("Error in sigaction(...)");
        ret = EXIT_FAILURE;
        goto finally;
    }

    ret = connect_clients(fd_soc);

 finally:

    if (-1 != fd_soc)
    {
        shutdown(fd_soc, SHUT_RDWR);
        close(fd_soc);
        fd_soc = -1;
    }

    return ret;
}
