#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

int num_threads = 3;

int CNT = 1000;
int a[5];
int status_a[5];
int FLAG_THE_END = 0;

pthread_mutex_t M1 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// чтобы погрузчик получил бы сигнал, когда потоки покупателей закончаться

pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_key_t key;
// once and key. Для установления в таблице переменных для потока покупателей

int search_open_shop() {
    // choice = 1 - Loader | 0 - buyer

    for (int i = 0; i < 5; i++) {
        if (0 == status_a[i]) {
            status_a[i] = 1;
            return i;
        } 
    }

    return -1;
}

void open_shop(int index) {
    status_a[index] = 0;
} 

void create_key() {
    pthread_key_create(&key, NULL);
    printf("Key created\n");
}



// Function executed by each thread
void* buyer_func(void* arg) {

    pthread_once(&once, create_key);
    int* value = malloc(sizeof(int));
    *value = *((int*)arg);
    pthread_setspecific(key, value);


    while (1) {

        pthread_mutex_lock(&M1);

        int index = search_open_shop();

        pthread_mutex_unlock(&M1);

        printf("shop[%d]: %d\t|\t", index, a[index]);

        *value = *((int*)pthread_getspecific(key));    

        *value = *value - a[index];
        a[index] = 0;

        pthread_setspecific(key, value);
        // check value

        printf("Buyer[%ld]_money: %d\t---\tshop[%d]: %d\n",  pthread_self(), *value, index, a[index]);


        if (*value <= 0) break;
        sleep(2);
        open_shop(index);
    }
    FLAG_THE_END = 1;
    // pthread_cond_signal(&cond);


    // Exit the thread
    pthread_exit(NULL);
}


// Function executed by each thread
void* loader_func(void* arg) {

    while (1) {
        pthread_mutex_lock(&M1);
        // Print thread ID
        // printf("Thread ID: %ld\tA: %d\n", pthread_self(), CNT);

        int index = search_open_shop();
        pthread_mutex_unlock(&M1);


        a[index] += 500;

        // Perform thread tasks...
        printf("loader_to_add shop[%d]: %d\n", index, a[index]);
        open_shop(index);
        sleep(3);

        // pthread_cond_wait(&cond, &M1);
        if (FLAG_THE_END) break;

    }


    // Exit the thread
    pthread_exit(NULL);
}


int main() {

    pthread_t threads_bayers[num_threads];
    pthread_t thread_loader;

    a[0] = 1000;
    a[1] = 1000;
    a[2] = 1000;
    a[3] = 1000;
    a[4] = 1000;

    status_a[0] = 0;
    status_a[1] = 0;
    status_a[2] = 0;
    status_a[3] = 0;
    status_a[4] = 0;

    int thread_args = 10000;


    // INITIALIZE
    // Initialize the mutex
    if (pthread_mutex_init(&M1, NULL) != 0) {
        fprintf(stderr, "Failed to initialize mutex");
        return 1; // Return error code
    }

    for (int i = 0; i < num_threads; i++) {
        // Create each thread
        if (pthread_create(&threads_bayers[i], NULL, buyer_func, &thread_args) != 0) {
            fprintf(stderr, "Failed to create thread %d", i);
            return 1; // Return error code
        }
    }
    if (pthread_create(&thread_loader, NULL, loader_func, NULL) != 0) {
        fprintf(stderr, "Failed to create thread loader");
        return 1; // Return error code
    }





    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads_bayers[i], NULL);
    }
    pthread_join(thread_loader, NULL);


    // Destroy the mutex
    pthread_mutex_destroy(&M1);

    return 0; // Return success code
}

