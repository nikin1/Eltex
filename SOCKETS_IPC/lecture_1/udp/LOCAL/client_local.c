#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/udp.h>

#define MY_SOCK_PATH "/tmp/dgram_client"
#define SERV_SOCK_PATH "/tmp/dgram_serv"


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main() {
    int fd;
    // size_t size_client;
    struct sockaddr_un serv_addr, client_addr;

    fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&client_addr, 0, sizeof(client_addr));
                /* Clear structure */

    serv_addr.sun_family = AF_LOCAL;
    strncpy(serv_addr.sun_path, SERV_SOCK_PATH,
                sizeof(serv_addr.sun_path) - 1);

    client_addr.sun_family = AF_LOCAL;
    strncpy(client_addr.sun_path, MY_SOCK_PATH,
                sizeof(client_addr.sun_path) - 1);




    // printf("sizeof: %ld\n", sizeof(serv_addr));

    if (bind(fd, (struct sockaddr *) &client_addr, sizeof(client_addr)))
        handle_error("bind");
    /*
    теперь к нашему fd мы назначали наш клиент end_point

    Далее через send_to мы направляем сообщение к серверу (в структуре его path)
    А Сервер в ожидании recvfrom получит fd, с клиентским end_point's 
    */
    // printf("<<<bind>>>\n");

    // sendto
    char buf_to[] = "Hello";
    int len_buf = strlen(buf_to);

    if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("sendto");


    char buf[128];
    int recv_res = recvfrom(fd, buf, 128, 0, NULL, NULL);
    if (recv_res == -1) handle_error("recvfrom");
    // printf("<<<recvfrom>>>\n");
    printf("msg: %s\n", buf);
    // printf("recv_res: %d\n", recv_res);


    close(fd);


    return 0;
}

