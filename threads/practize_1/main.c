#include <pthread.h>


void* thread_func(void* arg) {
    // Thread functionality goes here
    return NULL;
}

int main() {

    pthread_t thread_id;
    
    // Create a new thread
    pthread_create(&thread_id, NULL, thread_func, NULL);
    
    // Wait for the thread to finish
    pthread_join(thread_id, NULL);

    return 0;
}
