#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ipc
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/types.h>

// fork
#include <sys/wait.h>
#include <unistd.h>

// Semaphore
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include <errno.h>

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

extern int errno;
sem_t *sem;

int main(int argc, char *argv[]) {
    key_t key;
    int mode;
    int errnum;
    int value = 0;

    // semaphore
    char path[] = "/home/santonet/PROJECT/Eltex/IPC/shared_memory";
    const char path_for_sem[] = "/shared_memory";


    sem = sem_open(path_for_sem, O_CREAT, 0644, value);
    if (sem == SEM_FAILED) {
        printf("FAIL_SEM\n");

        printf("error: %d\n", errno);
        errnum = errno;
        printf("errno: %s\n", strerror(errnum));
    }
    
    
    sem_post(sem);
    int sem_value = 0;
    int res = sem_getvalue(sem, &sem_value);
    printf("sem: %d\n", sem_value);



    /* make the key: */
    if ((key = ftok(path, 1002)) == -1) /*Here the file must exist */ {
        perror("ftok");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");

    } else if (pid == 0) {
        // Child process
        int shmid;
        char *data;

        printf("Hello from child! (PID: %d)\n", getpid());



        /*  create the segment: */
        if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
            perror("shmget");
            exit(1);
        }

        /* attach to the segment to get a pointer to it: */
        if ((data = shmat(shmid, NULL, 0)) == (void *)-1) {
            perror("shmat");
            exit(1);
        }

        // sleep(2);
        sem_wait(sem);
        // printf("data1: %s\n", data);
        strncpy(data, "Hi", SHM_SIZE);
        printf("data2: %s\n", data);
        sem_post(sem);
        
        /* detach from the segment: */
        if (shmdt(data) == -1) {
            perror("shmdt");
            exit(1);
        }

    } else {
        // Parent process
        printf("Hello from parent! (PID: %d)\n", getpid());

        int shmid;
        char *data;

        /*  create the segment: */
        if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
            perror("shmget");
            exit(1);
        }

        /* attach to the segment to get a pointer to it: */
        if ((data = shmat(shmid, NULL, 0)) == (void *)-1) {
            perror("shmat");
            exit(1);
        }


        sem_wait(sem);

        // printf("data1: %s\n", data);
        strncpy(data, "Hello!", SHM_SIZE);
        printf("data2: %s\n", data);

        sem_post(sem);

        
        /* detach from the segment: */
        if (shmdt(data) == -1) {
            perror("shmdt");
            exit(1);
        }


        // Wait for the child process to terminate
        wait(NULL);
    }



    sem_close(sem);
    sem_unlink(path_for_sem);


    return 0;
}



