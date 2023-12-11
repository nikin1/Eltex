#include "chat.h"


int main(int argc, char *argv[]) {

    pid_t my_pid = getpid();

    key_t msgkey = ftok("chat_on_message_queue.c", 1000);

    int qid = msgget(msgkey, IPC_CREAT | 0666); // изменить

    if (qid == -1) errExit("msgget");

    char name[80];
    printf("What is your name ? ");
    scanf("%s", name);

    send_msg(qid, SERVICE_PRIORITY, name, my_pid);

    while(1) {
        char message[80];
        printf("%s: ", name);
        scanf("%s", message);

        send_msg(qid, MESSAGE_PRIORITY, message, my_pid);

    }


    return 0;
}

