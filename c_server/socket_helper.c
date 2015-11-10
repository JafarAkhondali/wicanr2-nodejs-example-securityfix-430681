#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include "socket_helper.h"

/*
 * Socket helper functions for Linux.
 */

/*
    set socket option timeout
*/
int set_socket_send_timeout( int fd, int timeout_sec )
{
	struct timeval timeout;
	timeout.tv_sec = timeout_sec;
	timeout.tv_usec = 0;
	int ret = 0;
	ret = setsockopt(fd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));
	if ( ret != 0 ) 
	{
		fprintf(stdout,"setsockopt(SO_SNDTIMEO) failed\n");
		return -1;
	}
	return 0;
}

/*
    set socket option reusable 
*/
int set_socket_reusable(int fd)
{
	int addr_reuse_flag = 1;
	int ret = 0;
	ret = setsockopt( fd , SOL_SOCKET, SO_REUSEADDR, 
		              (char*)&addr_reuse_flag, sizeof(addr_reuse_flag));
	if ( ret < 0 ) {
		fprintf(stdout,"setsockopt(SO_REUSEADDR) failed\n" );
		return -1;
	}
	return 0;
}

/*
    set socket option TCP NODELAY 
*/
int set_TCP_NODELAY( int fd ) 
{
	int nodelay =1 ;
	int ret = 0;
	ret	= setsockopt( fd, IPPROTO_TCP, TCP_NODELAY , (char*) &nodelay , sizeof(nodelay));
	if ( ret < 0 ) {
		fprintf(stdout,"setsockopt(TCP_NODELAY) failed\n" );
		return -1;
	}
	return 0;
}

/*
    set socket option TCP KeepAlive 
*/
int set_TCP_keepalive(int fd)
{
	int ret = 0;
	int keepalive = 1;
	int kp_cnt = 3;
	int kp_time = 1; // 1 sec
	int kp_intvl = 1; // 1sec
	
	// enable keepalive
	setsockopt( fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepalive, sizeof(keepalive));

	// set keepalive parameter, In Linux, the resolution of keepalive falls in second.
	ret = setsockopt ( fd, IPPROTO_TCP, TCP_KEEPCNT, &kp_cnt, sizeof(kp_cnt));
	if ( ret < 0 )   
	{   
		fprintf(stdout,"setsockopt(TCP_KEEPCNT) failed\n" );   
		return -1;   
	}
	ret = setsockopt ( fd, IPPROTO_TCP, TCP_KEEPIDLE, &kp_time, sizeof(kp_time));
	if ( ret < 0 )   
	{   
		fprintf(stdout,"setsockopt(TCP_KEEPIDLE) failed\n" );   
		return -1;   
	}
	ret = setsockopt ( fd, IPPROTO_TCP, TCP_KEEPINTVL, &kp_intvl, sizeof(kp_intvl));
	if ( ret < 0 )   
	{   
		fprintf(stdout,"setsockopt(TCP_KEEPINTVL) failed\n" );   
		return -1;   
	}
	   
	fprintf(stdout,"Linux TCP KeepAlive Parameter :\n");   
	fprintf(stdout,"   Keepalive Time     = %d sec\n", kp_time);   
	fprintf(stdout,"   Keepalive Interval = %d sec\n", kp_intvl);   
	return 0;
}

/*
    bind socket with port for server socket
*/
int bind_socket( int *socket_ptr, int port)
{
	struct sockaddr_in serv_addr;
	
	int ret = 0;

	//printf("port = %d\n", port );
	//printf("socket_ptr = %d\n", socket_ptr );
	*socket_ptr = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( *socket_ptr < 0 ) {
        fprintf(stdout,"create socket failed\n");
        return -1;
    }

	set_socket_reusable(*socket_ptr);
	set_TCP_keepalive(*socket_ptr);
	set_TCP_NODELAY ( *socket_ptr );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	serv_addr.sin_port = htons ( port );
	

	ret = bind( *socket_ptr, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));
	if ( ret  < 0 ) {
		fprintf(stdout,"bind socket error\n");
		return -1;
	}
	return 0;
}

/*
	helper function : get local socket address
*/ 
int get_localhost_addr() 
{
	struct addrinfo hints;
	struct addrinfo *result = 0;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	getaddrinfo("localhost", NULL, &hints, &result);

	for( res = result; res != NULL; res = res->ai_next ){   
		struct sockaddr_in* saddr = (struct sockaddr_in*)res->ai_addr;
		fprintf(stdout,"hostname: %s\n", inet_ntoa(saddr->sin_addr));
	} 	
	freeaddrinfo(result);
	return 0;
}

/* check readable inline function */
__inline int readable_timeout( int fd, int sec,int usec) {
	fd_set rset;
	struct timeval tv;
	FD_ZERO(&rset);	
	FD_SET(fd, &rset);
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	return ( select(fd+1, &rset, NULL, NULL, &tv) );
}

/* check readable when timeout in mircoseconds usec */
int readable_timeout_usec( int fd, int usec) {
	return readable_timeout( fd, 0, usec );
}

/* check readable when timeout in mircoseconds usec */
int readable_timeout_millisec( int fd, int millisec) {
	return readable_timeout( fd, 0, millisec * 1000 );
}

/* check readable when timeout in seconds usec */
int readable_timeout_sec( int fd, int sec) {
	return readable_timeout( fd, sec, 0 );
}

/*
 *  TCP Connections Establishment function
 */
int connect_to ( int *fd , char *ip , int port ) 
{
	struct sockaddr_in peer_addr;
	int ret = 0;

	*fd =socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if ( *fd < 0 ) {
		fprintf(stdout," socket creation failed\n");
		return -1;
	}

	set_socket_send_timeout( *fd, 3);
	set_socket_reusable(*fd);
	//set_TCP_keepalive(*fd);
	set_TCP_NODELAY ( *fd );

	peer_addr.sin_family = AF_INET;
	peer_addr.sin_addr.s_addr = inet_addr(ip);
	peer_addr.sin_port = htons(port);
	
	fprintf(stdout," connect to %s:%d\n",  ip, port);
	ret = connect( *fd, (struct sockaddr*) &peer_addr, sizeof( peer_addr ) ); 
	if ( ret < 0 ) 
	{
		fprintf(stdout,"  connect() failed\n");
		close_socket( fd );
		return -1;
	}
	return 0;
}

/*
 * Close Socket functions
 */
void close_socket ( int *fd ) {
	close( *fd );
}
