#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void handler(int sig) {
    printf("This is handler of signals\n");
}

int main() {

    printf("my_pid: %d\n", getpid());
    struct sigaction sa;
    
    sigset_t set;
    int result_sig;

    // add SIGUSR1 in set 
    sigemptyset(&set);                                                             
    if(sigaddset(&set, SIGUSR1) == -1) {                                           
        perror("Sigaddset error");                                                  
    }  



    // sigaction
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    // sa.__sigaction_handler = 
    sa.sa_handler = handler;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(-1);
    }

    sigprocmask(SIG_BLOCK, &set, NULL);
    int i = 0;
    while (1) {
        int sig_rv = sigwait(&set, &result_sig);
        if (sig_rv != 0) printf("ERROR in sigwait");

        printf("result_sig: %d\n", result_sig);
        printf("i: %d\n", i);
        i++;
    }

    return 0;
}

