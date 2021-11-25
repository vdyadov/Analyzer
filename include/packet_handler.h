#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_


int packet_handler_client(struct packet *original_packet);

int packet_handler_server(struct packet *original_packet, int fd_command_sock);


#endif /* _PACKET_HANDLER_H_ */
