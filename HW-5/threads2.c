// threads.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

pthread_mutex_t g_terminal_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t g_numbers_semaphore;
sem_t g_letters_semaphore;


// This function template, accepting a void pointer and returning a void pointer,
// is required for the start_routine function passed to pthread_create. 
// In this example we neither send input nor receive output.
void* print_numbers(void* arg) {
    sem_wait(&g_numbers_semaphore);
    pthread_mutex_lock(&g_terminal_mutex);

    for (int i=0; i<26; ++i) {
        printf("%d\n", i);
        sched_yield();
    }

    pthread_mutex_unlock(&g_terminal_mutex);
    sem_post(&g_letters_semaphore);
    return NULL;
}

void* print_letters(void* arg) {
    sem_wait(&g_letters_semaphore);
    pthread_mutex_lock(&g_terminal_mutex);

    for (int i=0; i<26; ++i) {
        printf("%c\n", 'A' + i);
        sched_yield();
    }

    pthread_mutex_unlock(&g_terminal_mutex);
    return NULL;
}

int main(int argc, char* argv[])
{
    sem_init(&g_numbers_semaphore, 0, 0);
    sem_init(&g_letters_semaphore, 0, 0);

    // Launch the threads
    pthread_t thread1;
    pthread_create(&thread1, NULL, print_numbers, NULL);
    pthread_t thread2;
    pthread_create(&thread2, NULL, print_letters, NULL);

    sem_post(&g_numbers_semaphore);

    // Wait for both threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Main: all threads complete\n");
    return 0;
}
