#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#include <errno.h>
#include <string.h>

extern errno;


#define PMODE 0666
#define QUEUE_MESSAGE "/myipc2"


void errExit(char *str) {
    printf("error: %d\n", errno);
    perror(str);
    exit(EXIT_FAILURE);
}



int main(int argc, char* argv[]) {


    struct mq_attr attr, *attrp;
    unsigned int prio;
    char *buffer;
    ssize_t numRead;

    // attrp = NULL;
    // attr.mq_maxmsg = 16;
    // attr.mq_msgsize = 5;
    // attr.mq_flags = 0;    
    // attrp = &attr;

    mqd_t mqd = mq_open(QUEUE_MESSAGE, O_CREAT | O_RDWR, PMODE, NULL);

    if (mqd == (mqd_t) - 1) errExit("mq_open");

    if (mq_getattr(mqd, &attr) == -1) errExit("getattr");

    // printf("msg_size: %ld\n", attr.mq_msgsize);
    // printf("msg_cnt: %ld\n", attr.mq_maxmsg);

    mq_send(mqd, "Hi", attr.mq_msgsize, 1);


    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL) errExit("malloc");

    numRead = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
    if (numRead == -1) errExit("mq_receive");


    printf("str receive: %s\n", buffer);
    

    int qid = fork();

    if (qid == 0) {
        mq_send(mqd, "HELLO", attr.mq_msgsize, 1);

        exit(EXIT_SUCCESS);

    } else {
        numRead = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
        if (numRead == -1) errExit("mq_receive");

        printf("str receive: %s\n", buffer);
        wait(NULL);
    }

    free(buffer);
    mq_close(mqd);
    mq_unlink(QUEUE_MESSAGE);

    return 0;
}
