#include "chat.h"

extern WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN;

int main(int argc, char *argv[]) {

    pid_t my_pid = getpid();

    key_t msgkey = ftok("chat_on_message_queue.c", 1000);

    int qid = msgget(msgkey, IPC_CREAT | 0666); // изменить

    if (qid == -1) errExit("msgget");

    char name[80], c;
    printf("What is your name ? ");
    scanf("%s%c", name, &c);

    send_msg(qid, SERVICE_PRIORITY, name, my_pid);


    while(1) {
        char str_message[80], c, tmp[80];

        // printf("%s: ", name);
        fgets(str_message, sizeof(str_message), stdin);
        str_message[strlen(str_message) - 1] = '\0';

        send_msg(qid, MESSAGE_PRIORITY, str_message, my_pid);
    }


    return 0;
}

