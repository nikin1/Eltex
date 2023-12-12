#include "chat.h"

WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN;


int main() {


    init_display(WIN_LEFT, WIN_RIGHT, WIN_LEFT_BORDER, WIN_RIGHT_BORDER,
        WIN_DOWN_BORDER, WIN_DOWN);
    refresh_display(WIN_LEFT, WIN_RIGHT, WIN_LEFT_BORDER, WIN_RIGHT_BORDER,
        WIN_DOWN_BORDER, WIN_DOWN);
    


    end_display(WIN_LEFT_BORDER, WIN_RIGHT_BORDER, WIN_DOWN_BORDER);


    return 0;
}