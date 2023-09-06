#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int pipefd[2], second_pipefd[2];
    pid_t cpid;
    char buf;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(second_pipefd) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    /* Child reads from pipe */
        // pipe 1
        close(pipefd[1]);          /* Close unused write end */

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);     /*printf*/

        write(STDOUT_FILENO, "\n\n\n", 1);
        close(pipefd[0]);


        // pipe2
        close(second_pipefd[0]);
        write(second_pipefd[1], "WORLD!", strlen("WORLD!"));
        close(second_pipefd[1]);          /* Reader will see EOF */


        _exit(EXIT_SUCCESS);

    } else {            /* Parent writes argv[1] to pipe */
        
        // pipe 1
        close(pipefd[0]);          /* Close unused read end */
        // write(pipefd[1], argv[1], strlen(argv[1]));

        printf("<<<\n");
        if (dup2(pipefd[1], 1) == -1){
            write(pipefd[1], "ERRNO\n", strlen("ERRNO\n"));
        } else {
            write(pipefd[1], "NO_ERRNO\n", strlen("NO_ERRNO\n"));

        }
        close(pipefd[1]);          /* Reader will see EOF */
        // system("ls -a");
        printf(">>>");

        write(pipefd[1], "argv[1]", strlen("argv[1]"));


 
 

        // pipe 2
        close(second_pipefd[1]);          /* Close unused write end */

        while (read(second_pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(second_pipefd[0]);

 
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}

