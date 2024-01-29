#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/udp.h>

#define MY_SOCK_PATH "/tmp/dgram_serv"


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main() {
    int fd;
    socklen_t client_size;
    struct sockaddr_un serv_addr, client_addr;

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&client_addr, 0, sizeof(client_addr));



    serv_addr.sun_family = AF_LOCAL;
    strncpy(serv_addr.sun_path, MY_SOCK_PATH,
                sizeof(serv_addr.sun_path) - 1);

    
    // client_addr.sun_family = AF_LOCAL;
    // strncpy(client_addr.sun_path, MY_SOCK_PATH,
    //             sizeof(client_addr.sun_path) - 1);






    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind");
    
    

    client_size = sizeof(serv_addr);



    // printf("client_size_1: %d\n", client_size);

    char buf[128];
    int recv_res = recvfrom(fd, buf, 128, 0, (struct sockaddr *) &client_addr, &client_size);
    if (recv_res == -1) handle_error("recvfrom");
    // printf("<<<recvfrom>>>\n");
    printf("msg: %s\n", buf);
    // printf("recv_res: %d\n", recv_res);


    // printf("CLIENT: sun_family: %d, sun_path: %s\n", client_addr.sun_family, client_addr.sun_path);
    // printf("SERVER: sun_family: %d, sun_path: %s\n", serv_addr.sun_family, serv_addr.sun_path);
    
    // printf("client_size_2: %d\n", client_size);


    // sendto
    char buf_to[] = "Hi";
    int len_buf = strlen(buf_to);

    if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &client_addr, client_size) == -1)
        handle_error("sendto");
    close(fd);


    return 0;
}


