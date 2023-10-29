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
struct task task_queue[QUEUE_SIZE];
int front = 0, rear = -1, size = 0;

// the worker bee
pthread_t thread_array[NUMBER_OF_THREADS];

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
    pthread_mutex_lock(&mutex);
    if (!(size == QUEUE_SIZE)) {
        rear = (rear + 1) % QUEUE_SIZE;
        task_queue[rear] = t;
        size++;
    } else {
        pthread_mutex_unlock(&mutex);
        return 1;
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

// remove a task from the queue
task dequeue()
{
    struct task worktodo;
    pthread_mutex_lock(&mutex);
    if (!(size == 0)) {
        worktodo = task_queue[front];
        front = (front + 1) % QUEUE_SIZE;
        size--;
    } else {
        print_error("ERROR: Forbidden operation. Thread attempted to dequeue from empty queue!")
    }
    pthread_mutex_unlock(&mutex);
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    struct task worktodo;
    // execute the task
    while(TRUE) {
        sem_wait(&sem);
        worktodo = dequeue();
        execute(worktodo.function, worktodo.data);
    }
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
    struct task worktodo;
    int ret;
    worktodo.function = somefunction;
    worktodo.data = p;
    ret = enqueue(worktodo);
    if (!ret) {
        sem_post(&sem);
    }

    return ret;
}

// initialize the thread pool
void pool_init(void)
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem, 0, 0);
    int ret;
    for (int t = 0; t < NUMBER_OF_THREADS; t++) {
        ret = pthread_create(&thread_array[t], NULL, worker, NULL);
        if (ret) print_error("ERROR: pthread_create failed");
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_cancel(thread_array[i]);
        pthread_join(thread_array[i], NULL);
    }
    sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);
}
