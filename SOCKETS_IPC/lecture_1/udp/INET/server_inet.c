#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main() {
    int fd;
    socklen_t client_size;
    struct sockaddr_in serv_addr, client_addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = 7777;

    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind");
    // printf("<<<bind>>>\n");
    
    // printf("client_size_0: %d\n", client_size);

    client_size = sizeof(serv_addr);
    // printf("client_size_1: %d\n", client_size);

    // printf("sun_family: %d, sun_port: %d\n", client_addr.sin_family, client_addr.sin_port);


    char buf[128];
    int recv_res = recvfrom(fd, buf, 128, 0, (struct sockaddr *) &client_addr, &client_size);
    if (recv_res == -1) handle_error("recvfrom");
    // printf("<<<recvfrom>>>\n");
    printf("msg: %s\n", buf);
    // sendto
    char buf_to[] = "Hi";
    int len_buf = strlen(buf_to);
    // printf("client_size_2: %d\n", client_size);


    // printf("sun_family: %d, sun_port: %d\n", client_addr.sin_family, client_addr.sin_port);


    if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &client_addr, client_size) == -1)
        handle_error("sendto");
    close(fd);

    return 0;
}

