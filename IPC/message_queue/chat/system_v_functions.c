#include "chat.h"

void errExit(char *str) {
    printf("error: %d\n", errno);
    perror(str);
    exit(EXIT_FAILURE);
}

void send_msg_all_users(int qid_2, list_t *head_name_list, char *str_to_msg) {
    int len = Len_List(head_name_list);
    list_t *a = NULL;
    a = head_name_list;
    // wprintw(WIN_RIGHT, "<2>");

    for (int i = 0; i < len; i++) {
        char test[80];
        sprintf(test, "i: %d", i);
        send_msg(qid_2, a->pid, "<<HELLO>>");
        // display_right(,"<1>");
        // wprintw(WIN_RIGHT, "<1>");
        a = GetTail(a);
    }
}


void send_msg(int qid, int msgtype, char *str_to_msg) {
    struct my_msgbuf msg;

    msg.mtype = msgtype;
    // msg.mpid = msg_pid; //test

    // time(&t);
    // printf("pre_sent: %s\n", str_to_msg);
    snprintf(msg.mtext, 80, "%s", str_to_msg);

    if (msgsnd(qid, &msg, sizeof(msg.mtext), MSG_NOERROR) == -1) {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }
    // printf("sent: %s\n", msg.mtext);
}


int get_msg(int qid, int msgtype, struct my_msgbuf* msg, int var_nowait) {

    int res;
    if (var_nowait) {
        res = msgrcv(qid, msg, sizeof(msg->mtext), msgtype, MSG_NOERROR | IPC_NOWAIT);
    } else {
        res = msgrcv(qid, msg, sizeof(msg->mtext), msgtype, MSG_NOERROR);
    }

    // if (res == -1) {
    //     if (errno != ENOMSG) {
    //         perror("msgrcv");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    return res;
}

