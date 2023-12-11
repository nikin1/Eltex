#include "chat.h"

int GLOBAL_LIST_LEN = 0; 
int main() {

    WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
    *WIN_DOWN_BORDER, *WIN_DOWN;
    int GLOBAL_LIST_LEN = 0;

    // init_display(WIN_LEFT, WIN_RIGHT, WIN_LEFT_BORDER, WIN_RIGHT_BORDER,
    // WIN_DOWN_BORDER, WIN_DOWN);

    list_t *head_name_list = NULL;
    list_t *name_list = NULL;

    key_t msgkey = ftok("chat_on_message_queue.c", 1000);

    int qid = msgget(msgkey, IPC_CREAT | PMODE); // изменить
    if (qid == -1) errExit("msgget");
  
    // struct msgbuf msg;

    // get_msg(qid, SERVICE_PRIORITY, &msg);





    while (1) {

        struct msgbuf msg;
        get_msg(qid, SERVICE_PRIORITY, &msg);

        // add msg.text to list structure

        // strcpy(&head_name_list, msg.mtext);

        // Copy_Data_out_MSG_BUF(name, msg);
        
        if (head_name_list == NULL) {
            head_name_list = Create(msg.mpid, msg.mtext);
            name_list = head_name_list;
        } else {
            Append(name_list, msg.mpid, msg.mtext);
            name_list = name_list->next;
        }

        printf("Name: %s\n", name_list->name);

        // display
        // display_right(WIN_DOWN_BORDER, head_name_list);



        // pid_t mypid = getpid();
        // send user
        // send_msg(qid, msg.mpid, "status: good", mypid);
        

    }

    end_display(WIN_LEFT_BORDER, WIN_RIGHT_BORDER, WIN_DOWN_BORDER);


    return 0;
}

