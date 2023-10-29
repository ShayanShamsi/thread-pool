/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

int print_error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(2);
}

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        fprintf(stderr, "where <n> is the number of tasks to execute.\n");
    }
    int n = argv[1];
    if (n <= 0) print_error("ERROR: The number of tasks has to be positive.");

    // create some work to do
    struct data *work = malloc(n * sizeof(struct data));

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for (int i = 0; i < n; i++) {
        work[i].a = i;
        work[i].b = -i;
        while(pool_submit(&add,&work) != 0) ;
    }

    // may be helpful 
    sleep(3);

    pool_shutdown();

    free(work);

    return 0;
}
