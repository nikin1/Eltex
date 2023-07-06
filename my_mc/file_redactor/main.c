#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>

#include <assert.h>

#define CNT_BYTE_READ 5
extern WINDOW *stdscr;

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

void read_file(int file_descriptor, char *buf) {

    char buffer_for_read[256];

    int res = read(file_descriptor, buffer_for_read, CNT_BYTE_READ);

    strncpy(buf, buffer_for_read, res);
    buf[res] = '\0';
}

int main(int argc, char ** argv) {
    initscr();
    
    signal(SIGWINCH, sig_winch);
    cbreak(); 
    noecho();
    curs_set(TRUE);

    attron(A_BOLD);
    // move(5, 15);
    int file_descriptor = open("file2.txt", O_CREAT|O_RDWR, S_IRWXU);
    FILE* fp;
    while(1) {
        char buf[256];
        read_file(file_descriptor, buf);
        printw("%s", buf);
        if (buf[0] == '\0') break;
    }


    // printw("Hello, brave new curses world!\n");


    attroff(A_BOLD);
    // attron(A_BLINK);
    // move(7, 16);
    // printw("Press any key to continue...");
    refresh();

    keypad(stdscr, TRUE); // Для обработки esc спец символов
    
    int ch;
    int y = 0, x = 0;
    wmove(stdscr, y, x);
    while( (ch = wgetch(stdscr)) != KEY_F(3)) {
        getyx(stdscr, y, x);

        if (ch == KEY_BACKSPACE) {
            delch();
            // обработать \n
        } else if (ch == KEY_LEFT) {
            wmove(stdscr, y, x - 1);
        } else if (ch == KEY_RIGHT) {
            wmove(stdscr, y, x + 1);
        } else if (ch == KEY_UP) {
            wmove(stdscr, y - 1, x);
        } else if (ch == KEY_DOWN) {
            wmove(stdscr, y + 1, x);
        } else if (ch == KEY_F(1)) {
            fp = fopen("file2.txt", "rw");
            assert(fp != NULL);
            putwin(stdscr, fp);
        } 
        
        else {
            wechochar(stdscr, ch);
        }

        // switch (ch)
        // {
        // case KEY_BACKSPACE:
            
        //     break;
        
        // default:
        //     break;
        // }
    }
    close(file_descriptor);
    if(fp != NULL)    fclose(fp);

    endwin();
    exit(EXIT_SUCCESS);
    return 0;
}

