#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <time.h>
#include <unistd.h>

#include <sys/wait.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long mtype;
    char mtext[80];
};

static void 
send_msg(int qid, int msgtype, char *str_to_msg) {
    struct msgbuf msg;
    time_t t;

    msg.mtype = msgtype;

    // time(&t);
    snprintf(msg.mtext, sizeof(msg.mtext), "%s", str_to_msg);

    if (msgsnd(qid, &msg, sizeof(msg.mtext),
                IPC_NOWAIT) == -1) {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }
    printf("sent: %s\n", msg.mtext);
}


static void
get_msg(int qid, int msgtype)
{
    struct msgbuf msg;

    if (msgrcv(qid, &msg, sizeof(msg.mtext), msgtype,
                MSG_NOERROR | IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("No message available for msgrcv()\n");
    } else
        printf("message received: %s\n", msg.mtext);
}


int main() {
    key_t msgkey = ftok("main.c", 1000);
    pid_t cpid;

    // msgget(key, )    
    // printf("key: %d\n", msgkey);

    int qid = msgget(msgkey, IPC_CREAT | 0666);

    if (qid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    cpid = fork();

    if (cpid == 0) {
        // int qid2 = msgget(msgkey, IPC_CREAT | 0666);

        send_msg(qid, 1000, "Hi");
        exit(EXIT_SUCCESS);
    } else {

        get_msg(qid, 1000);

        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }


    exit(EXIT_SUCCESS);



    return 0;
}

