#ifndef _TCP_COMMAND_PROCESSOR_H_
#define _TCP_COMMAND_PROCESSOR_H_


int echo_receiver(struct client_socket const *c_s);


// The Function to receive data from the client
int receive_one_struct_packet(struct client_socket const *const c_s);


// The Function to exchanging data with the client 
int receive_wexec_packet_handler(struct client_socket const *const c_s);


#endif
