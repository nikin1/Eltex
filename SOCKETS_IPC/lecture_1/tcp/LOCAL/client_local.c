#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <sys/un.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#define MY_SOCK_PATH "/tmp/stream_serv"
#define LISTEN_BACKLOG 5

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main() {
    int fd;
    struct sockaddr_un serv;

    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd == -1)
        handle_error("socket");

    memset(&serv, 0, sizeof(serv));
                /* Clear structure */

    serv.sun_family = AF_LOCAL;
    strncpy(serv.sun_path, MY_SOCK_PATH,
                sizeof(serv.sun_path) - 1);
    
    if (connect(fd, (struct sockaddr *) &serv, sizeof(serv)) == -1) 
        handle_error("connect");
    // printf("<<<connect>>>\n");


    // /* Code to deal with incoming connection(s)... */

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

    // printf("<<<recv>>>\n");
 
    close(fd);
    // unlink(MY_SOCK_PATH);
    return 0;
}



