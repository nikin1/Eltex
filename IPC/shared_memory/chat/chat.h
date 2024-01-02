#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// ipc
#include <sys/ipc.h>
#include <sys/shm.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

// ncurses
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>


// Semaphore
#include <semaphore.h>



#include <assert.h>


#define PMODE 0666
#define SHM_SIZE 1024

#define SERVICE_PRIORITY 10
#define MESSAGE_PRIORITY 5

extern WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN;



void display_right(WINDOW* WIN_RIGHT, char* data);
void display_left(WINDOW* WIN_LEFT, char* data);
void end_display();
void init_for_ncurses();
int init_display();
int refresh_display();
void sig_winch(int signo);
int input_message_on_display();

