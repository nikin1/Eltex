#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#include <pthread.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


void* thread_func(void* arg) {
    int *port;
    port = (void *) arg;

    // A += 1000;
   // Perform thread tasks...
    int fd;
    socklen_t client_size;
    struct sockaddr_in serv_addr, client_addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = *port;

    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind");

    client_size = sizeof(serv_addr);

    char buf[128];
    int recv_res = recvfrom(fd, buf, 128, 0, (struct sockaddr *) &client_addr, &client_size);
    if (recv_res == -1) handle_error("recvfrom");
    printf("msg_2: %s\n", buf);
    // sendto
    char buf_to[] = "Hi";
    int len_buf = strlen(buf_to);

    // printf("sun_family: %d, sun_port: %d\n", client_addr.sin_family, client_addr.sin_port);


    if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &client_addr, client_size) == -1)
        handle_error("sendto");
    close(fd);




    // Exit the thread
    pthread_exit(NULL);
}




int main() {
    int fd, port = 7777;
    socklen_t client_size;
    struct sockaddr_in serv_addr, client_addr;

    int index_threads = 0, num_threads = 100;
    pthread_t threads[num_threads];


    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = port;

    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind");
    

    client_size = sizeof(serv_addr);


    while (1) {
        
        // recvfrom
        char buf[128];
        int recv_res = recvfrom(fd, buf, 128, 0, (struct sockaddr *) &client_addr, &client_size);
        if (recv_res == -1) handle_error("recvfrom");

        printf("msg: %s\n", buf);
        port++;
        if (pthread_create(&threads[index_threads], NULL, thread_func, &port) != 0) {
            fprintf(stderr, "Failed to create thread %d", index_threads);
            return 1; // Return error code
        }


        // sendto
        char buf_to[5];
        sprintf(buf_to, "%d", port);
        int len_buf = strlen(buf_to);

        if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &client_addr, client_size) == -1)
            handle_error("sendto");
    }


    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    close(fd);



    return 0;
}

