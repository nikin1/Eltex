#include "chat.h"
// #include "display_main.c"


extern WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN, *WIN_INPUT;

// int GLOBAL_LIST_LEN = 0; 
int main() {

    // int GLOBAL_LIST_LEN = 0;

    pid_t pid;
    
    // init_display(WIN_LEFT, WIN_RIGHT, WIN_LEFT_BORDER, WIN_RIGHT_BORDER,
    // WIN_DOWN_BORDER, WIN_DOWN);

    list_t *head_name_list = NULL;
    list_t *name_list = NULL;

    key_t msgkey = ftok("chat_on_message_queue.c", 1000);

    int qid = msgget(msgkey, IPC_CREAT | PMODE); // изменить
    if (qid == -1) errExit("msgget");
    
    // struct msgbuf msg;

    // get_msg(qid, SERVICE_PRIORITY, &msg);


    int file_descriptor = open("file.txt", O_CREAT|O_RDWR, S_IRWXU);


    const char buffer[] = "Hello world!\n"; 
    size_t size = sizeof(char) * strlen(buffer); 
            


    // init_display(&WIN_LEFT, &WIN_RIGHT, &WIN_LEFT_BORDER, &WIN_RIGHT_BORDER,
    //     &WIN_DOWN_BORDER, &WIN_DOWN);


    // initscr();

    // signal(SIGWINCH, sig_winch);

    // cbreak(); // программа без ENTER в getchar получает символ 
    // noecho(); // отключает отображение символов, вводимых с клавиатуры 
    // curs_set(TRUE); // управляет видимостью курсора
    // refresh();
   
    // keypad(stdscr, TRUE); // Для обработки esc спец символов


    // int max_y, max_x;
    // getmaxyx(stdscr, max_y, max_x);


    // /* newwin - Первые два параметра - количествo строк и столбцов
    // в создаваемом окне, а вторые два - положение верхнего левого угла
    // нового окна (строка и столбец) относительно окна stdscr*/
    
    // int down_width = 5;
    // WIN_LEFT_BORDER = newwin(max_y - down_width, max_x * 3 / 4, 0, 0);
    // WIN_RIGHT_BORDER = newwin(max_y - down_width, max_x / 4, 0, max_x * 3 / 4);
    // WIN_DOWN_BORDER = newwin(down_width, max_x, max_y - down_width, 0);


    // box(WIN_LEFT_BORDER, '|', '-');
    // box(WIN_RIGHT_BORDER, '|', '-');
    // box(WIN_DOWN_BORDER, '|', '-');

    // WIN_LEFT = derwin(WIN_LEFT_BORDER, max_y - down_width - 2, max_x * 3 / 4 - 2, 1, 1);
    // WIN_RIGHT = derwin(WIN_RIGHT_BORDER, max_y - down_width - 2, max_x / 4 - 2, 1, 1);
    // WIN_DOWN = derwin(WIN_DOWN_BORDER, down_width - 2, max_x - 2, 1, 1);

    // keypad(WIN_LEFT, TRUE);
    // keypad(WIN_RIGHT, TRUE);
    // keypad(WIN_DOWN, TRUE);

  


    // DRAW - first
    // draw_dirs(WIN_LEFT, namelist_win_left, cnt_dirs_left);
    // draw_dirs(WIN_RIGHT, namelist_win_right, cnt_dirs_right);


    // wprintw(WIN_RIGHT, "Members: \n");
    // wprintw(WIN_DOWN, "Enter F_10 for exit\n");
    // wprintw(WIN_DOWN, "Enter message: \n");
    // wprintw(WIN_DOWN, "(max 50 simbol)");
    // wmove(WIN_DOWN, 1, strlen("Enter message: \n"));
    

    // wrefresh(WIN_RIGHT_BORDER);
    // wrefresh(WIN_LEFT_BORDER);
    // wrefresh(WIN_DOWN_BORDER);


    // wrefresh(WIN_LEFT);
    // wrefresh(WIN_DOWN);
    // wrefresh(WIN_RIGHT);


    // wmove(WIN_LEFT, 10, 10);
    // wprintw(WIN_LEFT, "Hi");


    // wrefresh(WIN_RIGHT_BORDER);
    // wrefresh(WIN_LEFT_BORDER);
    // wrefresh(WIN_DOWN_BORDER);


    // wrefresh(WIN_LEFT);
    // wrefresh(WIN_DOWN);
    // wrefresh(WIN_RIGHT);

    init_for_ncurses();
    init_display();
    refresh_display();

    while (1) {
        // ssize_t rv = write(file_descriptor, buffer, size);


        // printf("Ha");

        struct my_msgbuf msg;
        
        int msgtype = 0, flag_no_wait = 0;
        get_msg(qid, 0, &msg, flag_no_wait);   // 0 - WAIT. -5 , т.к. Сначала будет искать 5, потом 4,3,2,1. От большего к меньшему
        // int res = msgrcv(qid, &msg, sizeof(msg), 0, MSG_NOERROR);        


        // В моей очереди PID - это msgtype
        list_t *element_name = Search_Element_on_PID(head_name_list, msg.mtype);
        // Смотрим - есть ли пользователь с таким PID
        if (element_name == NULL)  { 
            // printf("Hhhhh....\n");
            if (head_name_list == NULL) {
                head_name_list = Create(msg.mtype, msg.mtext);
                name_list = head_name_list;
                // printf("Name: %s\n", name_list->name);
            } else {
                Append(name_list, msg.mtype, msg.mtext);
                name_list = name_list->next;
            }


            display_right(WIN_RIGHT, head_name_list, msg.mtype);
        } else {
            // printf("Alalala.");
            // printf("type: %ld\n", msg.mtype);

            // list_t *element_name = Search_Element_on_PID(head_name_list, msg.mpid);
            display_left(WIN_LEFT, element_name, msg.mtext);
        }

    }

    end_display(WIN_LEFT_BORDER, WIN_RIGHT_BORDER, WIN_DOWN_BORDER);
    

    return 0;
}

