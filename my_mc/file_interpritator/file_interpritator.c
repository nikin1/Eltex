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

return cnt_cmd - 1 or 2
*/

int split_str(char *str, char **sub_str1, char **sub_str2, char *del) {
    int cnt_cmd = 0;

    char *sub = strtok(str, del);
    //printf("sub: %s\n", sub);
    *sub_str1 = sub;
    sub = strtok(NULL, del);
    *sub_str2 = sub;

    if (*sub_str2 == NULL) return 1;
    return 2;
}

void my_strcpy(char *dest, char *src) {
    for (int i = 0; i < strlen(src) - 1; i++) {
        dest[i] = src[i];
    }
}


/*Эта функция удаляет ненужные пробелы c начала и с конца строки*/
void del_space(char *str) {
    int delta = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            // str[i] = '\0';
            delta++; /*будет смещение строки*/
        } else {
            break;
        }
    }

    for (int i = strlen(str) - 1; i >= 0; i--) {
        if (str[i] == ' ') {
            str[i] = '\0';
        } else {
            break;
        }
    }


    if (delta > 0) {
        // //printf("STR_BEFORE: |%s|\n", str);
        for (int i = 0; i + delta < strlen(str); i++) {
            str[i] = str[i + delta]; 
        }
        str[strlen(str) - delta] = '\0';
        // //printf("STR_END: |%s|\n", str);

    }
}


/*Подготовка строк. Разбиение одной cтроки на флаги и команду*/  
int get_str_for_exec(char *str, char **sub_cmd, char **sub_flag)  {

    // string
    char *sub_cmd_ptr, *sub_flag_ptr;
    int res_flags = split_str(str, &sub_cmd_ptr, &sub_flag_ptr, " ");
    //!!! по факту не идельно он возьмет аргументы, только думаю если 1 аргумент

    // char *sub_cmd = sub_cmd_ptr;
    *sub_cmd = malloc(1024 * sizeof(char));
    strcpy(*sub_cmd, sub_cmd_ptr);


    del_space(*sub_cmd);
    // //printf("sub_cmd: %s\n", *sub_cmd);
    
    if (2 == res_flags) {
        *sub_flag = malloc(1024 * sizeof(char));
        // *sub_flag[0] = '-';
        // strcat(*sub_flag, sub_flag_ptr);
        strcpy(*sub_flag, sub_flag_ptr);
        del_space(*sub_flag);
        // //printf("sub_flag: %s\n", *sub_flag);
    }
    //string 

    return res_flags;
}

int main(int argc, char *argv[]) {

    
    int pipefd[2], second_pipefd[2];

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

    // //printf("argc: %d\n", argc);
    int len = strlen(argv[1]);
    char str[len];
    strcpy(str, argv[1]);

    // //printf("str: %s\n", str);

    char *sub_str1 = NULL, *sub_str2 = NULL;
    int cnt_cmd = split_str(str, &sub_str1, &sub_str2, "|");

    // //printf("CNT_CMD: %d\n", cnt_cmd);

    // //printf("str: %s\n", str);
    // //printf("sub_str1: %s\n", sub_str1);
    // //printf("sub_str2: %s\n", sub_str2);

    /*
        Если 1 команда, то 1 fork, если 2 - то 2 forka
    */


    pid_t cpid, cpid2;
    char buf;


    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }




    if (cpid == 0) {    /* Child reads from pipe */
        
        close(pipefd[READ_END]);
        if (cnt_cmd == 2)   dup2(pipefd[WRITE_END], STDOUT_FILENO);

        close(pipefd[WRITE_END]);

        char *sub_cmd, *sub_flag;

        int res_flags = get_str_for_exec(sub_str1, &sub_cmd, &sub_flag);



        if (2 == res_flags) { // если есть флаги
            // //printf("^sub_cmd: |%s|\n", sub_cmd);
            // //printf("^sub_flag: |%s|\n", sub_flag);

            execlp(sub_cmd, sub_cmd, sub_flag, (char *) NULL);
            free(sub_flag);
        } else {
            // //printf("<<<>>>\n");
            // //printf("^sub_cmd: |%s|\n", sub_cmd);

            execlp(sub_cmd, sub_cmd, (char *) NULL);
        }
        free(sub_cmd);


        _exit(EXIT_SUCCESS);



    } else {            /* Parent writes argv[1] to pipe */
 
        // wait(NULL);                /* Wait for child */
        if (2 == cnt_cmd) {
            cpid2 = fork();

            if (0 == cpid2) {

                close(pipefd[WRITE_END]);
                
                dup2(pipefd[READ_END], STDIN_FILENO);

                close(pipefd[READ_END]);


                char *sub_cmd, *sub_flag;
                int res_flags = get_str_for_exec(sub_str2, &sub_cmd, &sub_flag);


                if (2 == res_flags) { // если еsсть флаги
                    // //printf(">sub_cmd: |%s|\n", sub_cmd);
                    // //printf(">sub_flag: |%s|\n", sub_flag);

                    execlp(sub_cmd, sub_cmd, sub_flag, (char *) NULL);
                    free(sub_flag);
                } else {
                    // //printf("<<<>>>\n");
                    // //printf("wwww\n");
                    // //printf(">sub_cmd: |%s|\n", sub_cmd);

                    execlp(sub_cmd, sub_cmd,(char *) NULL);
                    // execlp(sub_cmd, sub_cmd, (char *) NULL);
                }
                free(sub_cmd);


                _exit(EXIT_SUCCESS);
            }            
        }

        close(pipefd[READ_END]);
        close(pipefd[WRITE_END]);

        waitpid(-1, NULL, 0);
        waitpid(-1, NULL, 0);

        // exit(EXIT_SUCCESS);
    }
}


