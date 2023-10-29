/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo;

// the worker bee
pthread_t *thread_array;
pthread_mutex_t mutex;
sem_t sem;

int print_error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(2);
}

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    thread_array = malloc(NUMBER_OF_THREADS * sizeof(pthread_t));
    if (!thread_array) print_error("ERROR: malloc failed");

    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem, 0, 1);

    int ret;
    long t;
    for (t = 0; t < NUMBER_OF_THREADS; t++) {
        //ret = pthread_create(&thread_array[t], NULL, //function, //arguments);
        if (ret) print_error("ERROR: pthread_create failed");
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_join(bee,NULL);
}
