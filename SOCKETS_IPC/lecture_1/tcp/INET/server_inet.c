#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define LISTEN_BACKLOG 5

int main() {
    int fd, cfd;
    socklen_t client_addr_size;
    struct sockaddr_in serv_addr, client_addr;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));


    // иногда есть bind: Address already in use. Не знаю, как удалять адрес 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = 7775;
    // serv_addr.sin_addr = 127.0.0.1;



    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind");
    // printf("<<<bind>>>\n");
    
    if (listen(fd, LISTEN_BACKLOG) == -1)
    handle_error("listen");
    // printf("<<<listen>>>\n");



    client_addr_size = sizeof(client_addr);
    cfd = accept(fd, (struct sockaddr *) &client_addr,
                &client_addr_size);
    if (cfd == -1)
        handle_error("accept");
    // printf("<<<accept>>>\n");





    char buf;
    
    // flag = MSG_DONTWAIT -- enables nonblocking operation
    while (recv(cfd, &buf, 1, MSG_DONTWAIT) > 0) {
        write(STDOUT_FILENO, &buf, 1);
    }
    write(STDOUT_FILENO, "\n", 1);





    char buf_send[] = "Hi";
    if (send(cfd, buf_send, strlen(buf_send), 0) == -1)
        handle_error("send");



    close(fd);
    close(cfd);




    return 0;
}