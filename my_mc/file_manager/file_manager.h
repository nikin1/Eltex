#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


int pars_dir(char *str_dir, struct dirent ***namelist);
void clear_dirent_struct(struct dirent **namelist, int n);
void draw_dirs(WINDOW *WIN, struct dirent **namelist, int n);
int work_win(WINDOW *WIN, struct dirent **namelist, int cnt_dirs, int *index_dir_for_enter);

