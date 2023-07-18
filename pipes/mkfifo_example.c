#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    const char *fifoPath = "myfifo";
    char buf;

    pid_t cpid;


    if (argc != 3) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }



    if (mkfifo(fifoPath, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    

    if (cpid == 0) {  /* Child reads from pipe */
        // printf("Hello from child\n");
        
        // pipe 1
        int fd_read = open(fifoPath, O_RDONLY);
        
        write(STDOUT_FILENO, "First pipe: ", strlen("First pipe: "));

        while (read(fd_read, &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);         

        write(STDOUT_FILENO, "\n", 1);

        close(fd_read);


        // pipe 2
        int fd_write = open(fifoPath, O_WRONLY);
        
        write(fd_write, argv[2], strlen(argv[2]));

        close(fd_write);

        _exit(EXIT_SUCCESS);

    } else {  /* Parent writes argv[1] to pipe */
        // printf("Hello from parent\n");

        // pipe 1

        int fd_write = open(fifoPath, O_WRONLY);
        
        write(fd_write, argv[1], strlen(argv[1]));

        close(fd_write);

        // pipe 2
        int fd_read = open(fifoPath, O_RDONLY);
        
        write(STDOUT_FILENO, "Second pipe: ", strlen("Second pipe: "));

        while (read(fd_read, &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);         

        write(STDOUT_FILENO, "\n", 1);

        close(fd_read);

        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }


    return 0;
}
