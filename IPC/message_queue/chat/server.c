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
    
        int file_descriptor = open("file.txt", O_CREAT|O_RDWR, S_IRWXU);
        char buffer[80];
        sprintf(buffer, "msgkey1: %d\tmsgkey2: %d\nqid: %d\tqid_2: %d\n", msgkey1, msgkey2, qid, qid_2); 
        size_t size = sizeof(char) * strlen(buffer); 

    init_for_ncurses();
    init_display();
    refresh_display();

    while (1) {
        ssize_t rv = write(file_descriptor, buffer, size);
        struct my_msgbuf msg;
        
        int msgtype = 0, flag_no_wait = 0;
        get_msg(qid, 0, &msg, flag_no_wait);   // 0 - WAIT. -5 , т.к. Сначала будет искать 5, потом 4,3,2,1. От большего к меньшему

        int right_status = 0;
        // В моей очереди PID - это msgtype
        list_t *element_name = Search_Element_on_PID(head_name_list, msg.mtype);
        // Смотрим - есть ли пользователь с таким PID
        if (element_name == NULL)  {
            // printf("Hhhhh....\n");
            if (head_name_list == NULL) {
                head_name_list = Create(msg.mtype, msg.mtext);
                name_list = head_name_list;
                element_name = head_name_list;
                // printf("Name: %s\n", name_list->name);
            } else {
                Append(name_list, msg.mtype, msg.mtext);
                name_list = name_list->next;
                element_name = name_list;
            }

            display_right(WIN_RIGHT, head_name_list, msg.mtype);
            right_status = 1;
        } else {
            // list_t *element_name = Search_Element_on_PID(head_name_list, msg.mpid);
            display_left(WIN_LEFT, element_name->name, msg.mtext);
        }

        // SEND MESSAGE ALL USERS
        // send_msg_all_users(qid_2, head_name_list, msg.mtext);
        // send_msg(qid_2, head_name_list->pid, "<5>");


        int len = Len_List(head_name_list);
        list_t *a = NULL;
        a = head_name_list;
        // wprintw(WIN_RIGHT, "<2>");
        // printf("<<1>>\n");
        for (int i = 0; i < len; i++) {
            char str_with_pid[100];
            sprintf(str_with_pid, "%s$%ld$%s$%d", msg.mtext, msg.mtype, element_name->name, right_status);
            send_msg(qid_2, a->pid, str_with_pid);
            // printf("<i: %d>\n", i);
            // printf("str: %s\n", msg.mtext);
            fflush(stdin);
            sync();
            // printf("<<2>>\n");

            // display_right(,"<1>");
            // wprintw(WIN_RIGHT, "<1>");
            a = GetTail(a);
        }
    }

    end_display(WIN_LEFT_BORDER, WIN_RIGHT_BORDER, WIN_DOWN_BORDER);

    return 0;
}

