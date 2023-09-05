#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1



/*
Эта функция - дополнения к использованию ф-ии strtok, которые нужны под 
данные задачи, а это - разбитие строки только на 2 подстроки.
Мы получим указатель на 1-ую  и 2-ую подстроку. Иначе NULL

return -1. Значит строку нельзя разбить на делитель
return 0; Success
*/

int split_str(char *str, char **sub_str1, char **sub_str2, char *del) {
    int res;

    char *sub = strtok(str, del);

    if (sub == NULL) {
        *sub_str1 = NULL;
        *sub_str2 = NULL;
        res = -1;
    } else {
        *sub_str1 = sub;
        sub = strtok(NULL, del);
        *sub_str2 = sub;
        res = 0;
    }
    return res;
}



int main(int argc, char *argv[]) {

    
    int pipefd[2], second_pipefd[2];
    pid_t cpid, cpid2;
    char buf;

    if (argc == 1) {
        perror("Not have arguments for cmd\n");        
        _exit(EXIT_FAILURE);
    } 

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(second_pipefd) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    printf("argc: %d\n", argc);
    int len = strlen(argv[1]);
    char str[len];
    strcpy(str, argv[1]);

    printf("str: %s\n", str);

    char *sub_str1 = NULL, *sub_str2 = NULL;
    int res = split_str(str, &sub_str1, &sub_str2, "|");


    printf("str: %s\n", str);
    printf("sub_str1: %s\n", sub_str1);
    printf("sub_str2: %s\n", sub_str2);










    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }




    if (cpid == 0) {    /* Child reads from pipe */
        
        close(pipefd[READ_END]);
        dup2(pipefd[WRITE_END], STDOUT_FILENO);

        close(pipefd[WRITE_END]);

        execlp("ls", "ls", "-l", (char *) NULL);
        _exit(EXIT_SUCCESS);

    } else {            /* Parent writes argv[1] to pipe */
 
        // wait(NULL);                /* Wait for child */

        cpid2 = fork();

        if (0 == cpid2) {

            close(pipefd[WRITE_END]);
            
            dup2(pipefd[READ_END], STDIN_FILENO);

            close(pipefd[READ_END]);

            execlp("wc", "wc", "-l", (char *) NULL);

            _exit(EXIT_SUCCESS);
        }

        close(pipefd[READ_END]);
        close(pipefd[WRITE_END]);

        waitpid(-1, NULL, 0);
        waitpid(-1, NULL, 0);

        // exit(EXIT_SUCCESS);
    }
}

