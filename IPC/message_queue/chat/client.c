#include "chat.h"

// #include "server.c"



extern WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN, *WIN_INPUT;

int main(int argc, char *argv[]) {

    pid_t my_pid = getpid();

    key_t msgkey = ftok("chat_on_message_queue.c", 1000);

    int qid = msgget(msgkey, IPC_CREAT | 0666); // изменить

    if (qid == -1) errExit("msgget");




    // initscr();

    // signal(SIGWINCH, sig_winch);

    // cbreak(); // программа без ENTER в getchar получает символ 
    // noecho(); // отключает отображение символов, вводимых с клавиатуры 
    // curs_set(TRUE); // управляет видимостью курсора
    // refresh();
   
    // keypad(stdscr, TRUE); // Для обработки esc спец символов



    // wrefresh(WIN_RIGHT_BORDER);
    // wrefresh(WIN_LEFT_BORDER);
    // wrefresh(WIN_DOWN_BORDER);


    // wrefresh(WIN_LEFT);
    // wrefresh(WIN_DOWN);
    // wrefresh(WIN_RIGHT);


    init_for_ncurses();
    init_display();
    refresh_display();
    



    char name[80], c;
    INPUT_MESSAGE(name);
    wclear(WIN_INPUT);










    // printf("What is your name ? ");
    // scanf("%s%c", name, &c);

    send_msg(qid, my_pid, name, -1);
    sleep(1);
    refresh_display();


    while(1) {
        char str_message[80], c, tmp[80];

        // printf("%s: ", name);
        // fgets(str_message, sizeof(str_message), stdin);
        INPUT_MESSAGE(str_message);
        wclear(WIN_INPUT);
        // scanf("%c", &c);
        str_message[strlen(str_message) - 1] = '\0';

        send_msg(qid, my_pid, str_message, -1);
    }


    return 0;
}

