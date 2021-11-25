#ifndef _TCP_SOCKET_FUNCTION_H_
#define _TCP_SOCKET_FUNCTION_H_


int accept_socket(
        int fd_soc,
        struct client_socket *c_s,
        int *err_ret);


void close_socket(struct client_socket *c_s);


int get_socket_params(struct client_socket *c_s);


int get_cli_socket_params(struct client_socket *c_s);


int get_srv_socket_params(struct client_socket *c_s);


#endif
