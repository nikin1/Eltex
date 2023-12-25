#include "chat.h"

extern WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN, *WIN_INPUT;

int main(int argc, char *argv[]) {

    pid_t my_pid = getpid();

    key_t msgkey1 = ftok("/home/santonet/PROJECT/Eltex/IPC/message_queue/chat", 1000);
    key_t msgkey2 = ftok("/home/santonet/PROJECT/Eltex/IPC/message_queue/chat", 1001);

    int qid = msgget(msgkey1, IPC_CREAT | PMODE); // изменить
    int qid_2 = msgget(msgkey2, IPC_CREAT | PMODE); // изменить

    if (qid == -1) errExit("msgget");
    if (qid_2 == -1) errExit("msgget");

    list_t *head_name_list = NULL, *name_list = NULL;


    init_for_ncurses();
    init_display();
    refresh_display();

    char name[80], c;
    input_message_on_display(name);
    wclear(WIN_INPUT);




    send_msg(qid, my_pid, name);
    sleep(1);
    while (1) {
        int cnt = 0;
        while (1) {
            struct my_msgbuf msg;
            
            int flag_no_wait = 1;
            if (get_msg(qid_2, my_pid, &msg, flag_no_wait) == -1) {
                break;
            }
            char old_messasge[80];
            sprintf(old_messasge, "%s", msg.mtext);

            // msg.mtext
            char token_msg[80], token_pid[80], token_name[10], right_status_c;
            int message_pid;


            char *token = strtok(msg.mtext, "$");
            sprintf(token_msg, "%s", token);
            
            token = strtok(NULL, "$");
            sprintf(token_pid, "%s", token);

            sscanf(token_pid, "%d", &message_pid);

            token = strtok(NULL, "$");
            sprintf(token_name, "%s", token);

            token = strtok(NULL, "$");
            right_status_c = token[0];


            if (right_status_c == '1') {
                display_right_for_client(WIN_RIGHT, token_name, message_pid);

            } else {
                display_left(WIN_LEFT, token_name, token_msg);

            }
        }


        char str_message[80], c, tmp[80];
        input_message_on_display(str_message);
        wclear(WIN_INPUT);
        if (str_message[0] != '\0') {
          send_msg(qid, my_pid, str_message);             
          sleep(1);
        }
        // fflush(stdin);
        // sync();
    }

    return 0;
}

