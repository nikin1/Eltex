#include "chat.h"

extern WINDOW *WIN_LEFT, *WIN_RIGHT, *WIN_LEFT_BORDER, *WIN_RIGHT_BORDER,
*WIN_DOWN_BORDER, *WIN_DOWN, *WIN_INPUT;


sem_t *sem;

int main() {

    key_t key1, key2;
    int mode = 0644;
    int errnum;
    int value = 0;

    int shmid1, shmid2;
    char *data_left, *data_right;

    char path[] = "/home/santonet/PROJECT/Eltex/IPC/shared_memory/chat";
    const char path_for_sem[] = "/chat";

    /* make the key: */
    if ((key1 = ftok(path, 1009)) == -1) /*Here the file must exist */ {
        perror("ftok");
        exit(1);
    }
    if ((key2 = ftok(path, 1014)) == -1) /*Here the file must exist */ {
        perror("ftok");
        exit(1);
    }



    /*  create the segment: */
    if ((shmid1 = shmget(key1, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }
    if ((shmid2 = shmget(key2, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }





    /* attach to the segment to get a pointer to it: */
    if ((data_left = shmat(shmid1, NULL, 0)) == (void *)-1) {
        perror("shmat");
        exit(1);
    }
    if ((data_right = shmat(shmid2, NULL, 0)) == (void *)-1) {
        perror("shmat");
        exit(1);
    }


    sem = sem_open(path_for_sem, O_CREAT, mode, value);

    if (sem == SEM_FAILED) {
        printf("FAIL_SEM\n");

        printf("error: %d\n", errno);
        errnum = errno;
        printf("errno: %s\n", strerror(errnum));
    }
    
    
    sem_post(sem); // sem = 1











    init_for_ncurses();
    init_display();
    refresh_display();


    char name[20];
    input_message_on_display(name);
    wclear(WIN_INPUT);

    sem_wait(sem);
    strcat(data_right, name);
    strcat(data_right, "\n");

    display_right(WIN_RIGHT, data_right);

    sem_post(sem);
    while (1) {

        display_right(WIN_RIGHT, data_right);
        display_left(WIN_LEFT, data_left);


        char str_message[80], c, full_msg[128];
        int status = input_message_on_display(str_message);
        wclear(WIN_INPUT);
        if (status == -1) {
            break;
        } else if (status == 2) {
            // для отладки
            memset(data_left, '\0', SHM_SIZE);
            memset(data_right, '\0', SHM_SIZE);
        }


        if (str_message[0] != '\0') {
            sem_wait(sem);
            snprintf(full_msg, 128, "%s:%s\n", name, str_message);
            strcat(data_left, full_msg);
            sem_post(sem);
        }
        data_left[strlen(data_left)] = '\0';
        data_right[strlen(data_right)] = '\0';
    }

    end_display();
    /* detach from the segment: */
    if (shmdt(data_left) == -1) {
        perror("shmdt");
        exit(1);
    }
    /* detach from the segment: */
    if (shmdt(data_right) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shmid2, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    if (shmctl(shmid1, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }





    sem_close(sem);
    sem_unlink(path_for_sem);

    return 0;
}