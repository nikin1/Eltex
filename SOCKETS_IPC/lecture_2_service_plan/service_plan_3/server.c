#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#include <pthread.h>

#include <arpa/inet.h>

// for mqueue
#include <sys/wait.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

// for mqueue
#define PMODE 0666
#define QUEUE_MESSAGE "/my_mqueue_for_service"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


#define CNT_THREAD 5
int SERVICE_STATUS_FOR_INIT[] = {0,    0,    0,    0,    0   };
int SERVICE_STATUS[] = {0,    0,    0,    0,    0   };

int SERVICE_PORTS[] = {7778, 7779, 7780, 7781, 7782};


pthread_mutex_t M1 = PTHREAD_MUTEX_INITIALIZER;
mqd_t mqd;

int index_free_service(int mode) {
    if (mode == 0) {
        for (int i = 0; i < CNT_THREAD; i++) {
            if (SERVICE_STATUS_FOR_INIT[i] == 0) {
                return i;
            }
        }

    } else {
        for (int i = 0; i < CNT_THREAD; i++) {
            if (SERVICE_STATUS[i] == 0) {
                return i;
            }
        }
    }
    return -1;
}





void* thread_func(void* arg) {


    // for mqueue
    struct mq_attr attr;
    unsigned int prio;
    char *buffer;
    ssize_t numRead;


    if (mq_getattr(mqd, &attr) == -1) handle_error("getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL) handle_error("malloc");
    // for mqueue  -- end






    pthread_mutex_lock(&M1);
    int index = index_free_service(0);

    int port = SERVICE_PORTS[index];
    printf("port: %d\n", port);

    uint16_t port_big = htons(port);
    SERVICE_STATUS_FOR_INIT[index] = 1;

    pthread_mutex_unlock(&M1);



    int fd;
    socklen_t client_size;
    struct sockaddr_in serv_addr, client_addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = port_big;

    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind_threads");

    
    client_size = sizeof(serv_addr);

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;



    while (1) {
        /* code */


        numRead = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
        if (numRead == -1) handle_error("mq_receive");

        printf("str receive: %s\n", buffer);

        sscanf(buffer, "%hd", &client_addr.sin_port);

        // sendto
        char buf_to[] = "Hi";
        int len_buf = strlen(buf_to);

        // printf("sun_family: %d, sun_port: %d\n", client_addr.sin_family, client_addr.sin_port);


        if (sendto(fd, buf_to, len_buf, 0, (struct sockaddr *) &client_addr, client_size) == -1)
            handle_error("sendto");
        

        char buf[128];
        int recv_res = recvfrom(fd, buf, 128, 0, NULL, NULL);
        if (recv_res == -1) handle_error("recvfrom");
        printf("msg: %s\n", buf);


    }
    
    close(fd);



    // Exit the thread
    pthread_exit(NULL);
}




int main() {

    int fd;
    socklen_t client_size;
    struct sockaddr_in serv_addr, client_addr;

    int index_threads = 0, num_threads = 100;
    pthread_t threads[num_threads];

    char end_point_str[16];

    // for mqueue
    struct mq_attr attr;


    mqd = mq_open(QUEUE_MESSAGE, O_CREAT | O_RDWR, PMODE, NULL);

    if (mqd == (mqd_t) - 1) handle_error("mq_open");

    if (mq_getattr(mqd, &attr) == -1) handle_error("getattr");







    uint16_t port_big = htons(7777);




    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) handle_error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = port_big;
    // close(fd);
    // exit(EXIT_SUCCESS);
    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind");
    

    client_size = sizeof(serv_addr);

    // create a few servers
    for (index_threads = 0; index_threads < CNT_THREAD; index_threads++) {
        if (pthread_create(&threads[index_threads], NULL, thread_func, NULL) != 0) {
            fprintf(stderr, "Failed to create thread %d", index_threads);
            return 1; // Return error code
        }
    }

    while(1) {
        // recvfrom
        char buf[128];
        printf("fd_before: %d\n", fd);
        int recv_res = recvfrom(fd, buf, 128, 0, (struct sockaddr *) &client_addr, &client_size);
        if (recv_res == -1) handle_error("recvfrom");

        // printf("peer_port: %d\n", ntohs(client_addr.sin_port));
        // printf("fd: %d\n", fd);
        sprintf(end_point_str, "%d", client_addr.sin_port);

        printf("msg: %s\n", buf);
    

        mq_send(mqd, end_point_str, attr.mq_msgsize, 1);
    }


    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    close(fd);

    pthread_mutex_destroy(&M1);




    return 0;
}



