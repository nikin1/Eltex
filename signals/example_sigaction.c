#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void handler(int sig) {
    printf("This is handler of signals\n");
}

int main() {
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    // sa.__sigaction_handler = 
    sa.sa_handler = handler;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(-1);
    }

    while (1) {
        
    }

    return 0;
}