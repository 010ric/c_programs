#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "list.h"

void *handleThread(void *val);
int getRand(int from, int to);

int r = 0;
int k = 10;

int main(int argc, char **argv) {
    int n = 1;

    srand(time(NULL));

    int c;
    while ((c = getopt(argc, argv, "k:n:r")) != -1) {
        switch (c) {
            case 'k':
                k = atoi(optarg);
                break;
            case 'n':
                n = atoi(optarg);
                break;
            case 'r':
                r = 1;
                break;
            default:
                printf("usage: ./forkmany [-k <K>] [-n <N>] [-r]\n");
                return -1;
        }
    }

    time_t curtime;
    time(&curtime);
    printf("Start: %s", ctime(&curtime));
    fflush(stdout);
    pthread_t *threads = malloc(n * sizeof(pthread_t));
    list_t *threadList = list_init();
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, &handleThread, NULL);
        list_append(threadList, i);
    }

    // wait till all child processes have terminated
    struct list_elem *currentThread = threadList->first;
    while (currentThread != NULL) {
        pthread_join(threads[currentThread->data], NULL);
        currentThread = currentThread->next;
    }

    time(&curtime);
    printf("Ende: %s", ctime(&curtime));
    fflush(stdout);

    list_finit(threadList);
    free(threads);
    return 0;
}

/**
 * This method contains the code that is executed by each thread.
 * 
 * @return (Child-PID + k) % 100
 */
void *handleThread(void *val) {
    int toCount = r ? getRand(k / 2, 1.5 * k) : k;
    for (int i = 1; i <= toCount; i++) {
        printf("%10u\t%d\t%ld\n", (unsigned int) pthread_self(), getpid(), i);
        sleep(1);
    }
    return NULL;
}

/**
 * This method returns a (pseudo)random integer in the range [from; to] by using rand().
 */
int getRand(int from, int to) {
    if (from > to)
        return -1;
    return from + (rand() % (to - from + 1));
}