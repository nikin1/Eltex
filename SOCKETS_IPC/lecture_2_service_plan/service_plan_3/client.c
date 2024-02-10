#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#include <arpa/inet.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main() {
    int fd;
    // size_t size_client;
    struct sockaddr_in serv_addr, new_serv_addr;
    socklen_t serv_size;


    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");


    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7777);



    char buf_to[] = "Hello";
    int len_buf = strlen(buf_to);

    serv_size = sizeof(serv_addr); 

    if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &serv_addr, serv_size) == -1)
        handle_error("sendto");


    sleep(1);
    char buf[128];
    int recv_res = recvfrom(fd, buf, 128, 0, (struct sockaddr *) &new_serv_addr, &serv_size);
        if (recv_res == -1) handle_error("recvfrom");
    printf("msg: %s\n", buf);

    sleep(1);

    buf_to[4] = '2';
    if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &new_serv_addr, serv_size) == -1)
        handle_error("sendto");


    if (recvfrom(fd, buf, 128, 0, (struct sockaddr *) &new_serv_addr, &serv_size) == -1)
        handle_error("recvfrom"); 
    printf("msg: %s\n", buf);

    close(fd);
    return 0;
}


