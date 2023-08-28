#include "file_manager.h"


// function for ncurses
void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
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
  
    // PARS
    int cnt_dirs_left = pars_dir(".", &namelist_win_left);
    int cnt_dirs_right = pars_dir(".", &namelist_win_right);



    // DRAW - first
    draw_dirs(WIN_LEFT, namelist_win_left, cnt_dirs_left);
    draw_dirs(WIN_RIGHT, namelist_win_right, cnt_dirs_right);





    wrefresh(WIN_LEFT_BORDER);
    wrefresh(WIN_RIGHT_BORDER);

    wrefresh(WIN_LEFT);
    wrefresh(WIN_RIGHT);



    // work = while(1)

    wmove(WIN_LEFT, 0, 0);
    wmove(WIN_RIGHT, 0, 0);
    WINDOW *tmp_win = WIN_LEFT;
    struct dirent **tmp_namelist = namelist_win_left;
    int tmp_cnt_dirs = cnt_dirs_left;

    /*Индекс нужен для перехода в другую папку я буду знать 
    в каком индексе в моем списке она нах-ся*/
    int index_dir_for_enter;


    // эта переменная собирает весь путь прохождения по папкам
    char name_dir_right[1024] = ".";
    char name_dir_left[1024] = ".";

    while (1) {
        int stat = work_win(tmp_win, tmp_namelist, tmp_cnt_dirs, &index_dir_for_enter);
        // если функция вышла - значит нужно сделать какое-то изменение




        if (1 == stat) { /*F(10) - exit*/
            break;

        } else if (2 == stat) { /*Enter*/

            if (tmp_namelist == namelist_win_right) {
                wclear(WIN_RIGHT);
                
                // for dir
                strcat(name_dir_right, "/");
                strcat(name_dir_right, namelist_win_right[index_dir_for_enter]->d_name);

                // wprintw(WIN_LEFT, "<<%s>>", name_dir);

                clear_dirent_struct(namelist_win_right, cnt_dirs_right);

                /* TEST*/
                // wclear(WIN_LEFT);
                // wprintw(WIN_LEFT, "<<%s>>", name_dir_right);
                // wrefresh(WIN_LEFT);


                cnt_dirs_right = pars_dir(name_dir_right, &namelist_win_right);
                
                draw_dirs(WIN_RIGHT, namelist_win_right, cnt_dirs_right);
                wrefresh(WIN_RIGHT);

                tmp_win = WIN_RIGHT;
                tmp_namelist = namelist_win_right;
                tmp_cnt_dirs = cnt_dirs_right;

            } else if (tmp_namelist == namelist_win_left) {
                // аналогичная история

                wclear(WIN_LEFT);

                strcat(name_dir_left, "/");
                strcat(name_dir_left, namelist_win_left[index_dir_for_enter]->d_name);

                clear_dirent_struct(namelist_win_left, cnt_dirs_left);

                cnt_dirs_left = pars_dir(name_dir_left, &namelist_win_left);

                draw_dirs(WIN_LEFT, namelist_win_left, cnt_dirs_left);
                wrefresh(WIN_LEFT);


                tmp_win = WIN_LEFT;
                tmp_namelist = namelist_win_left;
                tmp_cnt_dirs = cnt_dirs_left;

            }
                
        } else if (3 == stat) { /*TAB*/

            if (tmp_win == WIN_LEFT) {
                tmp_win = WIN_RIGHT;
                tmp_namelist = namelist_win_right;
                tmp_cnt_dirs = cnt_dirs_right;
            } else {
                tmp_win = WIN_LEFT;
                tmp_namelist = namelist_win_left;
                tmp_cnt_dirs = cnt_dirs_left;
            }

            //Этот move просто показывает курсор, и не двигается
            int x, y;
            getyx(tmp_win, y, x);
            wmove(tmp_win, y, x);

            

        } else {
            exit(EXIT_FAILURE);
        }

    }

    clear_dirent_struct(namelist_win_left, cnt_dirs_left);
    clear_dirent_struct(namelist_win_right, cnt_dirs_right);

    delwin(WIN_LEFT_BORDER);
    delwin(WIN_RIGHT_BORDER);

    endwin();
    exit(EXIT_SUCCESS);

    return 0;
}



