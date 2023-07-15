#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;

    // Create a new process
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Hello from child! (PID: %d)\n", getpid());
    } else {
        // Parent process
        printf("Hello from parent! (PID: %d)\n", getpid());

        // Wait for the child process to terminate
        wait(NULL);
    }

    return 0;
}