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
    int sfd, cfd;
    struct sockaddr_un my_addr, peer_addr;
    socklen_t peer_addr_size;


    sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sfd == -1)
        handle_error("socket");

    memset(&my_addr, 0, sizeof(my_addr));
                /* Clear structure */

    my_addr.sun_family = AF_LOCAL;
    strncpy(my_addr.sun_path, MY_SOCK_PATH,
                sizeof(my_addr.sun_path) - 1);


    if (bind(sfd, (struct sockaddr *) &my_addr,
        sizeof(my_addr)) == -1)
    handle_error("bind");
    // printf("<<<bind>>>\n");


    if (listen(sfd, LISTEN_BACKLOG) == -1)
    handle_error("listen");
    // printf("<<<listen>>>\n");



    /* Now we can accept incoming connections one
        at a time using accept(2) */

    peer_addr_size = sizeof(peer_addr);
    cfd = accept(sfd, (struct sockaddr *) &peer_addr,
                &peer_addr_size);
    if (cfd == -1)
        handle_error("accept");
    // printf("<<<accept>>>\n");

    /* Code to deal with incoming connection(s)... */
    char buf;
    
    // flag = MSG_DONTWAIT -- enables nonblocking operation
    while (recv(cfd, &buf, 1, MSG_DONTWAIT) > 0) {
        write(STDOUT_FILENO, &buf, 1);
    }
    write(STDOUT_FILENO, "\n", 1);

    // printf("<<<recv>>>\n");


    char buf_send[] = "Hi";
    if (send(cfd, buf_send, strlen(buf_send), 0) == -1)
        handle_error("send");
    
    // printf("<<<sending>>>\n");
    
    close(sfd);
    close(cfd);

    // unlink(MY_SOCK_PATH);
    return 0;
}



