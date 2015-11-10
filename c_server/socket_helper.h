#ifndef SOCKET_HELPER_H
#define SOCKET_HELPER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

/*
 * Socket helper functions for Linux.
 */

void close_socket ( int *fd );
int set_socket_send_timeout( int fd, int timeout_sec );
int set_socket_reusable(int fd);
int set_TCP_NODELAY( int fd );
int set_TCP_keepalive(int fd);
int bind_socket( int *socket_ptr, int port);
int get_localhost_addr();
__inline int readable_timeout( int fd, int sec,int usec);
int readable_timeout_usec( int fd, int usec);
int readable_timeout_millisec( int fd, int millisec);
int readable_timeout_sec( int fd, int sec);
int connect_to ( int *fd , char *ip , int port );
void close_socket ( int *fd );
#endif
