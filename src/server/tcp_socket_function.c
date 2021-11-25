#include "definitions.h"
#include "tcp_socket_function.h"
#include <sys/select.h>


#define TIME_WAIT_MICROSECONDS 50000


typedef enum Select_imer_state
{
    TIMER_BREAK = 0,
    TIMER_OVER,
} Select_timer_state;


static int time_wait_for_accept(
        int fd_soc,
        int *timer_state,
        int *err_ret);


// Non blocking accept
//
int accept_socket(
        int const fd_soc,
        struct client_socket *const c_s,
        int *const err_ret)
{
    int ret = 0;
    int err_time_wait = 0;
    int timer_state = TIMER_BREAK;

    if (NULL == c_s || -1 == fd_soc)
    {
        fprintf(stderr, "Error in arguments accept_socket(...)!\n");
        ret = -1;
        goto finally;
    }

    memset(c_s, 0, sizeof(struct client_socket));

    ret = time_wait_for_accept(
            fd_soc,
            &timer_state,
            &err_time_wait);

    if (-1 == ret)
    {
        *err_ret = err_time_wait;
        goto finally;
    }

    if (TIMER_OVER == timer_state)
    {
        *err_ret = EAGAIN;
        ret = -1;
        goto finally;
    }

    errno = 0;
    c_s->fd_sock_client = accept(
            fd_soc,
            NULL,
            NULL);

    if (-1 == c_s->fd_sock_client)
    {
        if (EAGAIN == errno || EWOULDBLOCK == errno)
        {
            *err_ret = EAGAIN;
        }
        else
        {
            *err_ret = errno;
            perror("Error in accept(...), function accept_socket(...)");
        }
        ret = -1;
    }
    else
    {
        *err_ret = 0;
    }

 finally:

    return ret;
}


// Function with select for accept return -1 or 0
// Overtime set time_state to 1 (TIMER_OVER)
static int time_wait_for_accept(
        int const fd_soc,
        int *const timer_state,
        int *const err_ret)
{
    int ret = 0;
    fd_set read_fds;
    struct timeval tv;

    FD_ZERO(&read_fds);
    FD_SET(fd_soc, &read_fds);

    // Set timer
    tv.tv_sec = 0;
    tv.tv_usec = TIME_WAIT_MICROSECONDS;

    errno = 0;

    ret = select(fd_soc + 1, &read_fds, NULL, NULL, &tv);

    if (-1 == ret)
    {
        *err_ret = errno;

        if (EINTR != *err_ret)
        {
            perror("Error in select(...) for accept(...)");
        }

        goto finally;
    }

    *err_ret = 0;

    if (FD_ISSET(fd_soc, &read_fds))
    {
        *timer_state = TIMER_BREAK;
    }
    else
    {
        *timer_state = TIMER_OVER;
    }

    ret = 0;

 finally:

    return ret;
}


// Close
//
void close_socket(struct client_socket *const c_s)
{
    shutdown(c_s->fd_sock_client, SHUT_RDWR);
    close(c_s->fd_sock_client);
}


// Получение IPv4 адресов и номеров портов соединения клиент-сервер
//
int get_socket_params(struct client_socket *const c_s)
{
    int ret = 0;

    if (NULL == c_s || -1 == c_s->fd_sock_client)
    {
        fprintf(stderr, "Error in arguments get_socket_param(...)!\n");
        ret = -1;
        goto finally;
    }

    ret = get_cli_socket_params(c_s);
    if (-1 == ret)
    {
        fprintf(stderr, "Error getting client parameters!\n");
        goto finally;
    }

    ret = get_srv_socket_params(c_s);
    if (-1 == ret)
    {
        fprintf(stderr, "Error getting server parameters!\n");
        goto finally;
    }

 finally:

    return ret;
}


// Получение IPv4 адреса и номера порта клиента
//
int get_cli_socket_params(struct client_socket *const c_s)
{
    int ret = 0;
    struct sockaddr_in sock_addr = {0};
    socklen_t sock_addr_size = sizeof(struct sockaddr_in);

    if (NULL == c_s || -1 == c_s->fd_sock_client)
    {
        fprintf(stderr, "Error in arguments get_cli_socket_param(...)!\n");
        ret = -1;
        goto finally;
    }

    ret = getpeername(
            c_s->fd_sock_client,
            (struct sockaddr *)&sock_addr,
            &sock_addr_size);

    if (-1 == ret)
    {
        perror("Error in get_cli_socket_param(...), function getpeername(...)");
    }
    else
    {
        c_s->cli_addr.s_addr = sock_addr.sin_addr.s_addr;
        c_s->cli_port = sock_addr.sin_port;
    }

 finally:

    return ret;
}


// Получение IPv4 адреса и номера порта сервера
//
int get_srv_socket_params(struct client_socket *const c_s)
{
    int ret = 0;
    struct sockaddr_in sock_addr = {0};
    socklen_t sock_addr_size = sizeof(struct sockaddr_in);

    if (NULL == c_s || -1 == c_s->fd_sock_client)
    {
        fprintf(stderr, "Error in arguments get_srv_socket_param(...)!\n");
        ret = -1;
        goto finally;
    }

    ret = getsockname(
            c_s->fd_sock_client,
            (struct sockaddr *)&sock_addr,
            &sock_addr_size);

    if (-1 == ret)
    {
        perror("Error in get_srv_socket_param(...), function getsockname(...)");
    }
    else
    {
        c_s->srv_addr.s_addr = sock_addr.sin_addr.s_addr;
        c_s->srv_port = sock_addr.sin_port;
    }

 finally:

    return ret;
}
