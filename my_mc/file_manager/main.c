#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>


#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>


void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}


int pars_dir(char *str_dir, struct dirent ***namelist) {
    DIR *DIRECTORY;

    DIRECTORY = opendir(str_dir); 

    closedir(DIRECTORY);

    int cnt_dir;

    cnt_dir = scandir(".", namelist, NULL, alphasort);
    if (cnt_dir == -1) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }


    return cnt_dir;
}


    
void clear_dirent_struct(struct dirent **namelist, int n) {
    for (int i = 0; i < n; i++) {
        free(namelist[i]);       
    }
    free(namelist);

}

void draw_dirs(WINDOW *WIN, struct dirent **namelist, int n) {
    for (int i = 0; i < n; i++) {
        if (namelist[i]->d_type == DT_DIR) wprintw(WIN, "/");
        wprintw(WIN ,"%s\n", namelist[i]->d_name);
    }
}

int work_win(WINDOW *WIN, struct dirent **namelist, int cnt_dirs, int *index_dir_for_enter) {
    int flag_exit = 0;

    while(1) {
        int ch = wgetch(WIN);

        int x, y;
        getyx(WIN, y, x);
        switch (ch) {
            case KEY_UP:
                wmove(WIN, y - 1, x);
                break;
            case KEY_DOWN:
                if (y < cnt_dirs - 1) wmove(WIN, y + 1, x);
                break;

            /*Whis is Enter*/
            case '\n':
                if (namelist[y]->d_type == DT_DIR) {
                  flag_exit = 2;
                  *index_dir_for_enter = y;
                } 
                break;

            /*TAB in ascii */
            case 9:   
                // wmove(WIN, y + 1, x);
                flag_exit = 3;
                break;

            case KEY_F(10):
                flag_exit = 1;
                break;
            // default:
            //     echochar(ch);
            //     break;
        }

        if (flag_exit) break;
        
    }

    return flag_exit;
}

int main() {
    WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER;
    initscr();

    signal(SIGWINCH, sig_winch);

    cbreak(); // программа без ENTER в getchar получает символ 
    noecho(); // отключает отображение символов, вводимых с клавиатуры 
    curs_set(TRUE); // управляет видимостью курсора
    refresh();
   
    keypad(stdscr, TRUE); // Для обработки esc спец символов


    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);


    /* newwin - Первые два параметра - количествo строк и столбцов
    в создаваемом окне, а вторые два - положение верхнего левого угла
    нового окна (строка и столбец) относительно окна stdscr*/
    
    WIN_LEFT_BORDER = newwin(max_y, max_x / 2, 0, 0);
    WIN_RIGHT_BORDER = newwin(max_y, max_x / 2, 0, max_x / 2);

    box(WIN_LEFT_BORDER, '|', '-');
    box(WIN_RIGHT_BORDER, '|', '-');

    WIN_LEFT = derwin(WIN_LEFT_BORDER, max_y - 2, (max_x / 2) - 2, 1, 1);
    WIN_RIGHT = derwin(WIN_RIGHT_BORDER, max_y - 2, (max_x / 2) - 2, 1, 1);
    keypad(WIN_LEFT, TRUE);
    keypad(WIN_RIGHT, TRUE);

    struct dirent **namelist_win_left, **namelist_win_right;
  
    // pars
    int cnt_dirs_left = pars_dir(".", &namelist_win_left);
    int cnt_dirs_right = pars_dir(".", &namelist_win_right);



    // draw
    // wprintw(WIN_LEFT, "Hello, brave new curses world!\n");
    draw_dirs(WIN_LEFT, namelist_win_left, cnt_dirs_left);
    draw_dirs(WIN_RIGHT, namelist_win_right, cnt_dirs_right);





    wrefresh(WIN_LEFT_BORDER);
    wrefresh(WIN_RIGHT_BORDER);

    wrefresh(WIN_LEFT);
    wrefresh(WIN_RIGHT);
    // refresh();
    // move(1,1);
    // printw("Press any key to continue...\n");



    // work = while(1)

    wmove(WIN_LEFT, 0, 0);
    WINDOW *tmp_win = WIN_LEFT;
    struct dirent **tmp_namelist = namelist_win_left;
    int tmp_cnt_dirs = cnt_dirs_left;

    /*Индекс нужен для перехода в другую папку я буду знать 
    в каком индексе в моем списке она нах-ся*/
    int index_dir_for_enter;
    while (1) {
        int stat = work_win(tmp_win, tmp_namelist, tmp_cnt_dirs, &index_dir_for_enter);

        if (1 == stat) {
            break;

        } else if (2 == stat) { /*Enter*/


            // Переделать, дофиксить !!!!


            tmp_cnt_dirs = pars_dir(tmp_namelist[index_dir_for_enter]->d_name, 
            &tmp_namelist);
            // draw_dirs();


            


            if (tmp_namelist == namelist_win_right) {
                
                cnt_dirs_right = pars_dir(namelist_win_right[index_dir_for_enter]->d_name, 
                &namelist_win_right);
                
                draw_dirs(WIN_RIGHT, namelist_win_right, cnt_dirs_right);

                wrefresh(WIN_RIGHT);

                tmp_win = WIN_RIGHT;
                tmp_namelist = namelist_win_right;
                tmp_cnt_dirs = cnt_dirs_right;

            } else if (tmp_namelist == namelist_win_left) {
                
                cnt_dirs_left = pars_dir(namelist_win_left[index_dir_for_enter]->d_name,
                &namelist_win_left);

                draw_dirs(WIN_LEFT, namelist_win_left, cnt_dirs_left);
                wrefresh(WIN_LEFT);


                tmp_win = WIN_LEFT;
                tmp_namelist = namelist_win_left;
                tmp_cnt_dirs = cnt_dirs_left;

            }
                
        } else if (3 == stat) { /*TAB*/

            // Плохо переносится курсор. Или добавить move или что-то другое
            if (tmp_win == WIN_LEFT) {
                tmp_win = WIN_RIGHT;
                tmp_namelist = namelist_win_right;
                tmp_cnt_dirs = cnt_dirs_right;
            } else {
                tmp_win = WIN_LEFT;
                tmp_namelist = namelist_win_left;
                tmp_cnt_dirs = cnt_dirs_left;
            }
        } else {
            exit(EXIT_FAILURE);
        }

    }
    // wmove(WIN, 0, 0);


    // getch();
    clear_dirent_struct(namelist_win_left, cnt_dirs_left);
    clear_dirent_struct(namelist_win_right, cnt_dirs_right);

    delwin(WIN_LEFT_BORDER);
    delwin(WIN_RIGHT_BORDER);

    endwin();
    exit(EXIT_SUCCESS);




    return 0;
}



