#include <stdio.h>
#include <pthread.h>
int A = 1000;
pthread_mutex_t M1 = PTHREAD_MUTEX_INITIALIZER;

// Function executed by each thread
void* thread_func(void* arg) {

    pthread_mutex_lock(&M1);
    // Print thread ID

    printf("Thread ID: %ld\tA: %d\n", pthread_self(), A);
    A += 1000;
   // Perform thread tasks...
    pthread_mutex_unlock(&M1);


    // Exit the thread
    pthread_exit(NULL);
}

int main() {
    int num_threads = 1000;
    pthread_t threads[num_threads];


    // Initialize the mutex
    if (pthread_mutex_init(&M1, NULL) != 0) {
        fprintf(stderr, "Failed to initialize mutex");
        return 1; // Return error code
    }

    for (int i = 0; i < num_threads; i++) {
        // Create each thread
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
            fprintf(stderr, "Failed to create thread %d", i);
            return 1; // Return error code
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&M1);

    return 0; // Return success code
}

