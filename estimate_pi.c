#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

long thread_count;
long long number_throws;
volatile long long number_hits;

void* toss(void* tosses);

void usage(char* prog_name);
void get_args(int argc, char* argv[]);


int main(int argc, char* argv[]) {
    long thread;
    pthread_t* thread_handles;
    long toss_per_thread;
    double pi_estimate;

    /* get args and set rng */
    get_args(argc, argv);
    srand(time(0));

    /* set thread handles and total tosses per thread */
    toss_per_thread = number_throws/thread_count;
    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));


    /* toss and wait for tosses */
    for (thread = 0; thread < thread_count; thread++) 
        pthread_create(&thread_handles[thread], NULL, toss, (void*) toss_per_thread);
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    /* finalize monte carlo guess then print it */
    pi_estimate = 4*number_hits/(double) number_throws;
    printf("pi guess: %.4f\n", pi_estimate);
    
    free(thread_handles);
}

/*
 * Function: toss
 * Purpose: have each thread perform it's own toss
 * In args: tosses
 * Global out: number_hits
 */
void* toss(void* tosses) {
    double x;
    double y;
    double distance_squared;

    for (long i = 0; i < (long) tosses; i++) {
        /* generate x and y --- note that this is not a balanced random number generator due to the nature of rand()*/
        x = (double) rand() / RAND_MAX;
        x = -1 + x * 2;

        y = (double) rand() / RAND_MAX;
        y = -1 + y * 2;

        distance_squared = x*x + y*y;
        if (distance_squared <= 1) number_hits++;
    }

    return NULL;
}

/*
 * Function:    get_args
 * Purpose:     get command line args
 * In args:     argc, argv
 * Globals out: thread_count, number_throws
 */
void get_args(int argc, char* argv[]) {
    if (argc != 3) usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count < 0) usage(argv[0]);
    number_throws = strtol(argv[2], NULL, 10);
    if (number_throws % thread_count != 0 || number_throws < 0) usage(argv[0]);
}

/*
 * Function:    usage
 * Purpose:     print usage if program run incorrectly
 * In args:     prog_name
 */
void usage(char* prog_name) {
    fprintf(stderr, "usage: %s <number of threads> <number of throws>\n", prog_name);
    fprintf(stderr, "number of throws must be divisible by threads\n");
    fprintf(stderr, "number of throws and threads must be greater than 0\n");
    exit(1);
}
