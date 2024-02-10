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
    // size_t size_client;
    struct sockaddr_in serv_addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");


    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = 7777;


    if (connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) 
        handle_error("connect");
    // printf("<<<connect>>>\n");
    

    char buf_send[10] = "Hello";
    if (send(fd, buf_send, strlen(buf_send), 0) == -1)
        handle_error("send");
    // printf("<<<send>>>\n");
    
    sleep(1);

    // recv
    char buf_recv[128];
    if (recv(fd, buf_recv, 128, 0) == -1) 
        handle_error("recv");
    printf("msg: %s\n", buf_recv);



    int port;
    sscanf(buf_recv, "%d", &port);

    // fd = socket()
    serv_addr.sin_port = port;

    if (connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) 
        handle_error("connect");
    // printf("<<<connect>>>\n");
    

    memset(buf_send, '\0', strlen(buf_send));
    strcat(buf_send, "Hello_2") ;
    if (send(fd, buf_send, strlen(buf_send), 0) == -1)
        handle_error("send");
    // printf("<<<send>>>\n");
    
    sleep(1);

    // recv
    //  buf_recv[128];
    if (recv(fd, buf_recv, 128, 0) == -1) 
        handle_error("recv");

    close(fd);
    return 0;
}


