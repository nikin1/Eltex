#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>


#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main() {
    int fd;
    struct sockaddr_in serv_addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7780);


    if (connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) 
        handle_error("connect");



    char buf_send[] = "Hello";
    if (send(fd, buf_send, strlen(buf_send), 0) == -1)
        handle_error("send");


    sleep(1);

    char buf;
    // printf("<<<sending>>>\n");

    while (recv(fd, &buf, 1, MSG_DONTWAIT) > 0) {
        write(STDOUT_FILENO, &buf, 1);
    }
    write(STDOUT_FILENO, "\n", 1);


    close(fd);
    // printf("end\n");
    return 0;
}



