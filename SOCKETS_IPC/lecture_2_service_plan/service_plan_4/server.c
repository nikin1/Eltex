#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#include <pthread.h>

#include <sys/wait.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define LISTEN_BACKLOG 5




int main() {
    int fd_udp, fd_tcp, cfd;
    int  port_tcp = 7780, port_udp = 7781;


    socklen_t client_size_udp, client_size_tcp;
    struct sockaddr_in serv_addr_tcp, client_addr_tcp;
    struct sockaddr_in serv_addr_udp, client_addr_udp;




    // int index_threads = 0, num_threads = 100;
    // pthread_t threads[num_threads];


    fd_udp = socket(AF_INET, SOCK_DGRAM, 0);
    fd_tcp = socket(AF_INET, SOCK_STREAM, 0);

    if (fd_udp == -1) handle_error("socket");
    if (fd_tcp == -1) handle_error("socket");





    pid_t pid = fork();


    
    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process

        memset(&serv_addr_tcp, 0, sizeof(serv_addr_tcp));
        serv_addr_tcp.sin_family = AF_INET;
        serv_addr_tcp.sin_port = htons(port_tcp);
        
        

        if (bind(fd_tcp, (struct sockaddr *) &serv_addr_tcp, sizeof(serv_addr_tcp)) == -1)
            handle_error("bind_tcp");
        
        if (listen(fd_tcp, LISTEN_BACKLOG) == -1)
        handle_error("listen");
        client_size_tcp = sizeof(serv_addr_tcp);

        while (1) {
            cfd = accept(fd_tcp, (struct sockaddr *) &client_addr_tcp,
                &client_size_tcp);
            
            if (cfd == -1)  handle_error("accept");
             char buf;
    
            // flag = MSG_DONTWAIT -- enables nonblocking operation
            while (recv(cfd, &buf, 1, MSG_DONTWAIT) > 0) {
                write(STDOUT_FILENO, &buf, 1);
            }
            write(STDOUT_FILENO, "\n", 1);


            char buf_send[] = "Hi";
            if (send(cfd, buf_send, strlen(buf_send), 0) == -1)
                handle_error("send");

        }

        


    } else {
        // Parent process

        memset(&serv_addr_udp, 0, sizeof(serv_addr_udp));
        serv_addr_udp.sin_family = AF_INET;
        serv_addr_udp.sin_port = htons(port_udp);

        client_size_udp = sizeof(serv_addr_udp);

        if (bind(fd_udp, (struct sockaddr *) &serv_addr_udp, sizeof(serv_addr_udp)) == -1)
            handle_error("bind_udp");

        while (1) {
            
            // recvfrom
            char buf[128];
            int recv_res = recvfrom(fd_udp, buf, 128, 0, (struct sockaddr *) &client_addr_udp, &client_size_udp);
            if (recv_res == -1) handle_error("recvfrom");

            printf("msg_udp: %s\n", buf);


            // sendto
            char buf_to[] = "Hello";
            // sprintf(buf_to, "%d", port);
            int len_buf = strlen(buf_to);

            if (sendto(fd_udp, buf_to, len_buf, 0, (struct sockaddr *) &client_addr_udp, client_size_udp) == -1)
                handle_error("sendto");
        }
        // Wait for the child process to terminate
        wait(NULL);
    }

    close(fd_tcp);
    close(fd_udp);
    close(cfd);

    return 0;
}

