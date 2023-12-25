#include "chat.h"
// #include "display_main.c"


extern WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN, *WIN_INPUT;

int main() {

    pid_t pid;
    
    list_t *head_name_list = NULL;
    list_t *name_list = NULL;

    key_t msgkey1 = ftok("/home/santonet/PROJECT/Eltex/IPC/message_queue/chat", 1000);
    key_t msgkey2 = ftok("/home/santonet/PROJECT/Eltex/IPC/message_queue/chat", 1001);



    int qid = msgget(msgkey1, IPC_CREAT | PMODE); // изменить
    int qid_2 = msgget(msgkey2, IPC_CREAT | PMODE); // изменить

    if (qid == -1) errExit("msgget");
    if (qid_2 == -1) errExit("msgget");
    
    init_for_ncurses();
    init_display();
    refresh_display();

    while (1) {
        struct my_msgbuf msg;
        
        int msgtype = 0, flag_no_wait = 0;
        get_msg(qid, 0, &msg, flag_no_wait);   // 0 - WAIT. -5 , т.к. Сначала будет искать 5, потом 4,3,2,1. От большего к меньшему

        int right_status = 0;
        // В моей очереди PID - это msgtype
        list_t *element_name = Search_Element_on_PID(head_name_list, msg.mtype);
        // Смотрим - есть ли пользователь с таким PID
        if (element_name == NULL)  {

            if (head_name_list == NULL) {
                head_name_list = Create(msg.mtype, msg.mtext);
                name_list = head_name_list;
                element_name = head_name_list;
            } else {
                Append(name_list, msg.mtype, msg.mtext);
                name_list = name_list->next;
                element_name = name_list;
            }

            display_right(WIN_RIGHT, head_name_list, msg.mtype);
            right_status = 1;
        } else {
            display_left(WIN_LEFT, element_name->name, msg.mtext);
        }

        send_msg_all_users(qid_2, head_name_list, msg.mtext, msg.mtype, element_name->name, right_status);

    }

    end_display(WIN_LEFT_BORDER, WIN_RIGHT_BORDER, WIN_DOWN_BORDER);

    return 0;
}

