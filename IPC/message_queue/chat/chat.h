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

#define SERVICE_PRIORITY 1
#define MESSAGE_PRIORITY 2




struct msgbuf {
    long mtype;
    char mtext[80];
    long mpid;  // I need to add for my case
    // mpid - это я добавил для того, чтобы клиент отправлял свой pid
};

int get_msg(int qid, int msgtype, struct msgbuf *msg);
void errExit(char *str);
void send_msg(int qid, int msgtype, char *str_to_msg, long msg_pid);


typedef struct list {
   	int value;
    int pid;
    char name[80];
    struct list *next;
} list_t;



int GetHead(list_t *A);
list_t* GetTail(list_t *A);
int IsEmpty(list_t *A);
int check_set(int *set, int index, int x);
int Len_List(list_t *H);
int Check_Polindrom(list_t *H);
list_t *Search_Element(list_t *H, int k);
void DelDuplic(list_t *head);
list_t *Remove(list_t *A, list_t *head);
list_t *Append(list_t *A, int pid, char *str);
list_t *Create(int pid, char *str);
void Copy_Data_out_MSG_BUF(list_t *dest, struct msgbuf source);



// display
extern int GLOBAL_LIST_LEN;

void display_right(WINDOW* WIN_DOWN_BORDER, list_t* names_list);
void end_display(WINDOW* WIN_LEFT_BORDER, WINDOW* WIN_RIGHT_BORDER, WINDOW* WIN_DOWN_BORDER);
int init_display(WINDOW* WIN_LEFT, WINDOW* WIN_RIGHT, WINDOW* WIN_LEFT_BORDER, WINDOW* WIN_RIGHT_BORDER,
    WINDOW* WIN_DOWN_BORDER, WINDOW* WIN_DOWN);
