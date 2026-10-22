// threads.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// Global variables
/* TODO: Define global variables here. */

// This function template, accepting a void pointer and returning a void pointer,
// is required for the start_routine function passed to pthread_create. 
// In this example we neither send input nor receive output.
void* print_numbers(void* arg) {
    for (int i=0; i<26; ++i) {
        printf("%d\n", i);
        sched_yield();
    }
    return NULL;
}

void* print_letters(void* arg) {
    for (int i=0; i<26; ++i) {
        printf("%c\n", 'A' + i);
        sched_yield();
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    // Launch the threads
    pthread_t thread1;
    pthread_create(&thread1, NULL, print_numbers, NULL);
    pthread_t thread2;
    pthread_create(&thread2, NULL, print_letters, NULL);

    // Wait for both threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Main: all threads complete\n");
    return 0;
}
