#include "chat.h"

void errExit(char *str) {
    printf("error: %d\n", errno);
    perror(str);
    exit(EXIT_FAILURE);
}

void send_msg(int qid, int msgtype, char *str_to_msg, long msg_pid) {
    struct msgbuf msg;

    msg.mtype = msgtype;
    msg.mpid = msg_pid; //test

    // time(&t);
    snprintf(msg.mtext, sizeof(msg.mtext), "%s", str_to_msg);

    if (msgsnd(qid, &msg, sizeof(msg.mtext),
                IPC_NOWAIT) == -1) {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }
    printf("sent: %s\n", msg.mtext);
}


int get_msg(int qid, int msgtype, struct msgbuf* msg) {

    if (msgrcv(qid, msg, sizeof(msg->mtext), msgtype,
                MSG_NOERROR) == -1) {
        // if (errno != ENOMSG) {
            // perror("msgrcv");
            // exit(EXIT_FAILURE);
        // }
        // printf("No message available for msgrcv()\n");
        printf("Message have a Mistake\n");
        return -1;
    } else {
        printf("message received: %s\n", msg->mtext);
        return 1;

    }
}

