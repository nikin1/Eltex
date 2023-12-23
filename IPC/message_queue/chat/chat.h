#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <time.h>
#include <unistd.h>

#include <sys/wait.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>


#include <assert.h>


#define PMODE 0666

#define SERVICE_PRIORITY 10
#define MESSAGE_PRIORITY 5

extern WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN;


struct my_msgbuf {
    long mtype;
    char mtext[80];
    long mpid;  // I need to add for my case
    // mpid - это я добавил для того, чтобы клиент отправлял свой pid
};

typedef struct list {
   	int value;
    int pid;
    char name[10];
    struct list *next;
} list_t;


int get_msg(int qid, int msgtype, struct my_msgbuf* msg, int var_nowait);
void errExit(char *str);
void send_msg(int qid, int msgtype, char *str_to_msg);
void send_msg_all_users(int qid, list_t *head_name_list, char *str_to_msg);




int GetHead(list_t *A);
list_t* GetTail(list_t *A);
int IsEmpty(list_t *A);
int check_set(int *set, int index, int x);
int Len_List(list_t *H);
int Check_Polindrom(list_t *H);
list_t *Search_Element(list_t *H, int k);
list_t *Search_Element_on_PID(list_t *H, int pid);
void DelDuplic(list_t *head);
list_t *Remove(list_t *A, list_t *head);
list_t *Append(list_t *A, int pid, char *str);
list_t *Create(int pid, char *str);
void Copy_Data_out_MSG_BUF(list_t *dest, struct my_msgbuf source);



// display
// extern int GLOBAL_LIST_LEN;

void display_right(WINDOW* WIN_RIGHT, list_t* header_list, int pid);
void display_right_for_client(WINDOW* WIN_RIGHT, char* name, int pid);
void display_left(WINDOW* WIN_LEFT, char* name, char *str_message);
void end_display(WINDOW* WIN_LEFT_BORDER, WINDOW* WIN_RIGHT_BORDER, WINDOW* WIN_DOWN_BORDER);
void init_for_ncurses();
int init_display();
int refresh_display();
void sig_winch(int signo);
int input_message_on_display();