#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2    // Defines the number of threads as 2
#define NUM_UPDATES 2000000  // Defines the number of updates for each thread

struct shared_data {
    int value; // Shared variable to store result
};

struct shared_data *counter;  // Declares a pointer to struct shared_data to hold the shared counter
int bonus_count = 0;  // Declares a variable to count the number of times the bonus is received
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // Declares and initializes a mutex lock for thread synchronization

// Function for the first thread
void *thread1(void *arg) {
    // Loop for performing updates
    for (int i = 0; i < NUM_UPDATES; i++) {
        pthread_mutex_lock(&lock);  // Initiate the lock before entering the critical section
        counter->value++; // Increments the shared counter in the critical section
        // Checks if the counter is divisible by 100
        if (counter->value % 100 == 0) {
            counter->value += 100;  // Increments the counter by 100
            bonus_count++;  // Increments the bonus count
        }
        pthread_mutex_unlock(&lock);  // Releases the lock after exiting the critical section
    }
    printf("Thread 1: Completed %d updates and received bonus %d times. Counter = %d\n", NUM_UPDATES, bonus_count, counter->value);
    return NULL;  // Returns NULL to indicate successful completion of the thread
}

// Function for the second thread
void *thread2(void *arg) {
    // Loop for performing updates
    for (int i = 0; i < NUM_UPDATES; i++) {
        pthread_mutex_lock(&lock);  // Initiates the lock before entering the critical section
        counter->value++; // Increments the shared counter in the critical section
        pthread_mutex_unlock(&lock);  // Releases the lock after exiting the critical section
    }
    printf("Thread 2: Completed %d updates. Counter = %d\n", NUM_UPDATES, counter->value);
    return NULL;  // Returns NULL to indicate successful completion of the thread
}

int main() {
    pthread_t threads[NUM_THREADS];  // Declares an array to hold thread IDs

    counter = (struct shared_data *)malloc(sizeof(struct shared_data));  // Allocates memory for the shared counter
    counter->value = 0;  // Initializes the shared counter to zero

    pthread_attr_t attr;  // Declares a thread attribute variable
    pthread_attr_init(&attr);  // Initializes the thread attribute
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);  // Sets the detachstate attribute to joinable

    // Creates Thread 1 & Thread 2
    pthread_create(&threads[0], &attr, thread1, NULL);
    pthread_create(&threads[1], &attr, thread2, NULL);

    pthread_attr_destroy(&attr);  // Destroys the thread attribute after thread creation

    // Joins Thread 1 & Thread 2 to the main thread
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Prints the final value of the shared counter from the main thread
    printf("Main: Counter = %d\n", counter->value);

    free(counter);  // Frees the memory allocated for the shared counter
    pthread_mutex_destroy(&lock);  // Destroys the mutex lock

    return 0;  // Returns 0 to indicate successful execution of the program
}
