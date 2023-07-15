#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid_2, pid_3, pid_4, pid_5, pid_6;

    // Create a new process
    pid_2 = fork();

    if (pid_2 < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    } else if (pid_2 == 0) {
        // Child process
        printf("Hello from (PID_2: %d)\tMy parent: (%d)\n", getpid(), getppid());

        pid_4 = fork();

        if (pid_4 < 0) {
            fprintf(stderr, "Fork failed");
            return 1;
        } else if (pid_4 == 0) {
            printf("Hello from (PID_4: %d)\tMy parent: (%d)\n", getpid(), getppid());
        } else {
            pid_5 = fork();

            if (pid_5 < 0) {
                fprintf(stderr, "Fork failed");
                return 1;
            } else if (pid_5 == 0) {
                printf("Hello from (PID_5: %d)\tMy parent: (%d)\n", getpid(), getppid());
            } else {
                // printf("Hello from (PID_2: %d)\n", getpid());
                // PID_2
                wait(NULL); // PID_4
                wait(NULL); // PID_5
            }
        }


    } else {
        // Parent process
        printf("Hello from parent! (PID: %d)\tMy parent: (%d)\n", getpid(), getppid());

        pid_3 = fork();
        if (pid_3 < 0) {
            fprintf(stderr, "Fork failed");
            return 1;
        } else if (pid_3 == 0) {
            pid_6 = fork();

            if (pid_6 < 0) {
                fprintf(stderr, "Fork failed");
                return 1;
            } else if (pid_6 == 0) {
                printf("Hello from (PID_6: %d)\tMy parent: (%d)\n", getpid(), getppid());
                
            } else {
                printf("Hello from (PID_3: %d)\tMy parent: (%d)\n", getpid(), getppid());
                wait(NULL); // PID_6
            }
            

        } else {
            // Wait for the child process to terminate
            wait(NULL); // PID_2
            wait(NULL); // PID_3

        }


    }

    return 0;
}

