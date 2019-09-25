#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

long thread_count;
long long number_throws;
volatile long long number_hits;
void usage(char* prog_name);
void get_args(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    get_args(argc, argv);
    printf("%d %d\n", thread_count, number_throws);
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
