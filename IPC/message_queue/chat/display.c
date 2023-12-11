#include "chat.h"

// extern int GLOBAL_LIST_LEN;

// function for ncurses
void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}




void display_right(WINDOW* WIN_DOWN_BORDER, list_t* names_list) {
    wmove(WIN_DOWN_BORDER, 1, 0);

    for (int i = 0; i < GLOBAL_LIST_LEN; i++) {
        wprintw(WIN_DOWN_BORDER, "%s\n", names_list->name);        
    }
    wrefresh(WIN_DOWN_BORDER);
}




void display(WINDOW* WIN_LEFT_BORDER, WINDOW* WIN_RIGHT_BORDER,
 WINDOW*WIN_DOWN_BORDER, list_t* names_list) {

}


int init_display(WINDOW* WIN_LEFT, WINDOW* WIN_RIGHT, WINDOW* WIN_LEFT_BORDER, WINDOW* WIN_RIGHT_BORDER,
    WINDOW* WIN_DOWN_BORDER, WINDOW* WIN_DOWN) {
    // WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
    // *WIN_DOWN_BORDER, *WIN_DOWN;

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
    
    int down_width = 5;
    WIN_LEFT_BORDER = newwin(max_y - down_width, max_x * 3 / 4, 0, 0);
    WIN_RIGHT_BORDER = newwin(max_y - down_width, max_x / 4, 0, max_x * 3 / 4);
    WIN_DOWN_BORDER = newwin(down_width, max_x, max_y - down_width, 0);


    box(WIN_LEFT_BORDER, '|', '-');
    box(WIN_RIGHT_BORDER, '|', '-');
    box(WIN_DOWN_BORDER, '|', '-');

    WIN_LEFT = derwin(WIN_LEFT_BORDER, max_y - down_width - 2, max_x * 3 / 4 - 2, 1, 1);
    WIN_RIGHT = derwin(WIN_RIGHT_BORDER, max_y - down_width - 2, max_x / 4 - 2, 1, 1);
    WIN_DOWN = derwin(WIN_DOWN_BORDER, down_width - 2, max_x - 2, 1, 1);

    keypad(WIN_LEFT, TRUE);
    keypad(WIN_RIGHT, TRUE);
    keypad(WIN_DOWN, TRUE);

  


    // DRAW - first
    // draw_dirs(WIN_LEFT, namelist_win_left, cnt_dirs_left);
    // draw_dirs(WIN_RIGHT, namelist_win_right, cnt_dirs_right);


    wprintw(WIN_RIGHT, "Members: \n");
    wprintw(WIN_DOWN, "Enter F_10 for exit\n");
    wprintw(WIN_DOWN, "Enter message: \n");
    wprintw(WIN_DOWN, "(max 50 simbol)");
    wmove(WIN_DOWN, 1, strlen("Enter message: \n"));
    

    

    wrefresh(WIN_RIGHT_BORDER);
    wrefresh(WIN_LEFT_BORDER);
    wrefresh(WIN_DOWN_BORDER);


    wrefresh(WIN_LEFT);
    wrefresh(WIN_DOWN);
    wrefresh(WIN_RIGHT);









    // work = while(1)

    // wmove(WIN_RIGHT, 0, 0);
    // wmove(WIN_LEFT, 0, 0);


    // wmove(WIN_DOWN, 0, 0);
    // wrefresh(WIN_DOWN);

    int cnt = 0;
    while (1)
    {
        cnt++;
    }
    



    return 0;
}

void end_display(WINDOW* WIN_LEFT_BORDER, WINDOW* WIN_RIGHT_BORDER, WINDOW* WIN_DOWN_BORDER) {

    delwin(WIN_LEFT_BORDER);
    delwin(WIN_RIGHT_BORDER);
    delwin(WIN_DOWN_BORDER);

    endwin();
    exit(EXIT_SUCCESS);
}


