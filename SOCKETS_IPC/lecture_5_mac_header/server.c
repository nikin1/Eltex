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
    socklen_t client_size;
    struct sockaddr_in serv_addr, client_addr;
    // uint32_t ip_adress;    
    struct in_addr adress_ip;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");


    if (inet_pton(AF_INET, "127.0.0.1", &(adress_ip.s_addr) ) != 1) handle_error("inet_pton");



    memset(&client_addr, 0, sizeof(serv_addr));
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7777);
    serv_addr.sin_addr = adress_ip;

    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind");
    printf("<<<bind>>>\n");
    
    client_size = sizeof(serv_addr);

    
    while (1) {

        char buf[128];
        int recv_res = recvfrom(fd, buf, 1024, 0, (struct sockaddr *) &client_addr, &client_size);
        if (recv_res == -1) handle_error("recvfrom");
        printf("<<<recvfrom>>>\n");
        printf("msg: %s\n", buf);


        // printf("port: %d\n", ntohs(client_addr.sin_port));


        // sendto
        char buf_to[] = "Hi";
        int len_buf = strlen(buf);


        if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &client_addr, client_size) == -1)
            handle_error("sendto");

    }


    close(fd);

    return 0;
}

