#include <stdio.h>
#include "socket_helper.h"

int main(int argc, char* argv[])
{
    fprintf(stdout,"test get_localhost_addr()\n");
    get_localhost_addr();
    char buf[256];
    int fd = 0, ret = 0;
    bind_socket( &fd, 5566);
    ret = listen ( fd, 5 );
    fprintf(stdout,"listen on 5566\n");
    while(1) {
        ret = readable_timeout_millisec ( fd, 1000 );
        memset( buf, 0, sizeof(buf));
        int ifd = accept( fd, 0, 0);
        int rsize = recv(ifd, buf, 256, 0);
        fprintf(stdout,"recv %s\n", buf);
        close_socket(&ifd);
    }
    return 0;
}
