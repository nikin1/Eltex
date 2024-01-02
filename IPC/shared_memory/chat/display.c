#include "chat.h"

WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN, *WIN_INPUT;


// function for ncurses
void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}




void display_right(WINDOW* WIN_RIGHT, char* data) {
    wclear(WIN_RIGHT);
    wprintw(WIN_RIGHT, "Members: \n");
    wmove(WIN_RIGHT, 1, 0);



    char tmp[1024];
    strncpy(tmp, data, strlen(data));

    char *token = strtok(tmp, "\n");
    while (token != NULL) {
        wprintw(WIN_RIGHT, "%s\n", token);
        token = strtok(NULL, "\n");
    }

    wrefresh(WIN_RIGHT);
}





void display_left(WINDOW* WIN_LEFT, char* data) {
    // wprintw(WIN_LEFT, "%s: %s\n", name, str_message);
    wclear(WIN_LEFT);

    char tmp[1024];
    strncpy(tmp, data, strlen(data));

    char *token = strtok(tmp, "\n");
    while (token != NULL) {
        wprintw(WIN_LEFT, "%s\n", token);
        token = strtok(NULL, "\n");
    }

    wrefresh(WIN_LEFT);
}

void init_for_ncurses() {

    initscr();

    signal(SIGWINCH, sig_winch);

    cbreak(); // программа без ENTER в getchar получает символ 
    noecho(); // отключает отображение символов, вводимых с клавиатуры 
    curs_set(TRUE); // управляет видимостью курсора
    refresh();
   
    keypad(stdscr, TRUE); // Для обработки esc спец символов

}

int init_display() {
    // WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
    // *WIN_DOWN_BORDER, *WIN_DOWN;

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
    
    int x_for_input = max_x - 2 - strlen("Enter message:"); 
    WIN_INPUT = derwin(WIN_DOWN_BORDER, 1,  x_for_input, 2, strlen("Enter message:") + 1);

    keypad(WIN_LEFT, TRUE);
    keypad(WIN_RIGHT, TRUE);
    keypad(WIN_DOWN, TRUE);
    keypad(WIN_INPUT, TRUE);




    wprintw(WIN_RIGHT, "Members: \n");
    wprintw(WIN_DOWN, "Enter F_6 for clean chat | F_7 for exit | F_8 for update | F_9 for enter message\n");
    wprintw(WIN_DOWN, "Enter message: \n");
    wprintw(WIN_DOWN, "(max 50 simbol)");
    wmove(WIN_DOWN, 1, strlen("Enter message: \n"));
    
    return 0;
}


int refresh_display() {
    

    wrefresh(WIN_RIGHT_BORDER);
    wrefresh(WIN_LEFT_BORDER);
    wrefresh(WIN_DOWN_BORDER);


    wrefresh(WIN_LEFT);
    wrefresh(WIN_DOWN);
    wrefresh(WIN_RIGHT);
    wrefresh(WIN_INPUT);

}

int input_message_on_display(char *str_message) {
    
    int status = 0;
    int ch;
    int y = 0, x = 0;
    int flag_break = 0;
    unsigned int index = 0;
    wmove(WIN_INPUT, y, x);
    while(1) {

        ch = wgetch(WIN_INPUT);
        getyx(WIN_INPUT, y, x);        

        switch (ch) {
        case KEY_BACKSPACE:
            wdelch(WIN_INPUT);
            // обработать \n, а точнее удаление строки
            // !!! for str_message need to do !!!
            break;

        case KEY_LEFT:
            wmove(WIN_INPUT, y, x - 1);
            if (index > 0) index--;
            break;

        case KEY_RIGHT:
            wmove(WIN_INPUT, y, x + 1);
            index++;
            break;

        case KEY_F(8):
            flag_break = 1;
            break;
        
        case KEY_F(9): // KEY_ENTER :(
            flag_break = 1;
            status = 1;
            break;

        case KEY_F(7):
            flag_break = 1;
            status = -1;
            break;

        case KEY_F(6): 
            flag_break = 1;
            status = 2;
            break;



        default:
            wechochar(WIN_INPUT, ch);
            str_message[index++] = ch;
            break;
        }

        if (flag_break) break;
    }
    // if (index != 0) {
        // str_message[index++] = '\n';
        // str_message[index] = '\0';
    // } else {
        str_message[index] = '\0';
    // }


    return status;
}



void end_display() {

    delwin(WIN_LEFT_BORDER);
    delwin(WIN_RIGHT_BORDER);
    delwin(WIN_DOWN_BORDER);

    endwin();
    exit(EXIT_SUCCESS);
}


