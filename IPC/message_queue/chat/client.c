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
            int right_status;

            char *token = strtok(msg.mtext, "$");
            sprintf(token_msg, "%s", token);
            
            token = strtok(NULL, "$");
            sprintf(token_pid, "%s", token);

            int message_pid;
            sscanf(token_pid, "%d", &message_pid);

            token = strtok(NULL, "$");
            sprintf(token_name, "%s", token);

            token = strtok(NULL, "$");
            // sprintf(right_status_c, "%c", token[0]);
            right_status_c = token[0];

            // int message_pid;
            // sscanf(token_pid, "%d", &message_pid);



            if (right_status_c == '1') {
                display_right_for_client(WIN_RIGHT, token_name, message_pid);

            } else {
                display_left(WIN_LEFT, token_name, token_msg);

            }


            // printf("token_msg: %s\ntoken_pid: %s\n", token_msg, token_pid);


            // list_t *element_name = Search_Element_on_PID(head_name_list, message_pid);
            // Смотрим - есть ли пользователь с таким PID
            // if (element_name == NULL)  { 
                // if (head_name_list == NULL) {
                    // head_name_list = Create(message_pid, token_msg);
                    // name_list = head_name_list;
                    // printf("Name: %s\n", name_list->name);
                // } else {
                    // Append(name_list, message_pid, token_msg);
                    // name_list = name_list->next;
                // }
                // display_right(WIN_RIGHT, head_name_list, message_pid);
            // } else {
                // display_left(WIN_LEFT, element_name, token_msg);
            // }
            // cnt++;
            // if (cnt == 5) break;
        }


        char str_message[80], c, tmp[80];

        // printf("%s: ", name);
        // fgets(str_message, sizeof(str_message), stdin);
        input_message_on_display(str_message);
        wclear(WIN_INPUT);
        // scanf("%c", &c);
        // str_message[strlen(str_message) - 1] = '\0';
        if (str_message[0] != '\0') {
          send_msg(qid, my_pid, str_message);             
          sleep(1);
        }
        // fflush(stdin);
        // sync();
    }

    return 0;
}

