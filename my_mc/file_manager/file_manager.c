#include "file_manager.h"


int pars_dir(char *str_dir, struct dirent ***namelist) {

    int cnt_dir = scandir(str_dir, namelist, NULL, alphasort);
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
    wmove(WIN, 0, 0);
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
