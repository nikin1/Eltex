#include "chat.h"

void errExit(char *str) {
    printf("error: %d\n", errno);
    perror(str);
    exit(EXIT_FAILURE);
}

void send_msg_all_users(int qid_2, list_t *head_name_list, char *str_to_msg, long pid, char *name, int right_status) {

    int len = Len_List(head_name_list);
    list_t *a = NULL;
    a = head_name_list;

    for (int i = 0; i < len; i++) {
        char str_with_pid[100];
        sprintf(str_with_pid, "%s$%ld$%s$%d", str_to_msg, pid, name, right_status);
        send_msg(qid_2, a->pid, str_with_pid);
        fflush(stdin);
        sync();
        
        a = GetTail(a);
    }
}


void send_msg(int qid, int msgtype, char *str_to_msg) {
    struct my_msgbuf msg;

    msg.mtype = msgtype;

    snprintf(msg.mtext, 80, "%s", str_to_msg);

    if (msgsnd(qid, &msg, sizeof(msg.mtext), MSG_NOERROR) == -1) {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }
}


int get_msg(int qid, int msgtype, struct my_msgbuf* msg, int var_nowait) {

    int res;
    if (var_nowait) {
        res = msgrcv(qid, msg, sizeof(msg->mtext), msgtype, MSG_NOERROR | IPC_NOWAIT);
    } else {
        res = msgrcv(qid, msg, sizeof(msg->mtext), msgtype, MSG_NOERROR);
    }
    return res;
}

