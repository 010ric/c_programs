#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"
#include <pthread.h>
#include <string.h>

void print_time_step (int time , int thread_num) {
    static int first_time = 1;
    int i;
    if (first_time) {
        printf ("  Time |  1  2  3  4  5  6  7  8  9  10\n");
        printf ("-------+--------------------------------\n" ) ;
        first_time = 0;
    }
    printf ("%06d |", time);
    if ( thread_num ) {
        for (i = 1; i < thread_num; i++)
            printf ("   ");
        printf ("  %d\n" , thread_num); }
    else
            printf ("\n");
}

int clockl = 0;

void roundrobin(list_t *list, int timestep, int timequantum) {
    list_t *runningthreads = list_init();
    struct list_elem *actualrunning = NULL;
    while(1) {
        if(list_length(list) == 0 && list_length(runningthreads)==0) {
            break;
        }
        struct list_elem *searcher = list->first;
        while(searcher != NULL) {
            struct list_elem *next = searcher->next;
            if(searcher->startzeit <= clockl && searcher->runtime < searcher->zielzeit) {
                list_remove(list, searcher);
                list_append2(runningthreads, searcher);
            }
            searcher = next;
        }
        actualrunning = runningthreads->first;
        if(actualrunning != NULL) {
            int clockalt = clockl;
            while((clockl - clockalt) < timequantum) {
                actualrunning->runtime += timestep;
                print_time_step(clockl, actualrunning->number);
                clockl +=timestep;
            }
            list_remove(runningthreads, actualrunning);
            if(actualrunning->runtime < actualrunning->zielzeit)
                list_append2(list, actualrunning);
            actualrunning = NULL;
        } else {
            print_time_step(clockl, 0);
            clockl +=timestep;
        }
    }
}
void priorityroundrobin(list_t *list, int timestep, int timequantum) {
    list_t *runningthreads = list_init();
    struct list_elem *actualrunning = NULL;
    int prioact = 10;
    while(1) {
        if(list_length(list) == 0 && list_length(runningthreads) == 0) {
            break;
        }
        struct list_elem *searcher = list->first;
        while(searcher != NULL) {
            struct list_elem *next = searcher->next;
                if(searcher->startzeit <= clockl && searcher->runtime < searcher->zielzeit) {
                    prioact = searcher->priority;
                    list_remove(list, searcher);
                    struct list_elem *pos = runningthreads->first;
                    while(pos != NULL && pos->priority <= searcher->priority)
                        pos = pos->next;
                    searcher->next = NULL;
                    list_insertBeforePosition(runningthreads, searcher, pos);
                }
            searcher = next;
        }
        actualrunning = runningthreads->first;
        if(actualrunning != NULL) {
            prioact = actualrunning->priority;
            int clockalt = clockl;
            while((clockl - clockalt) < timequantum) {
                actualrunning->runtime += timestep;
                print_time_step(clockl, actualrunning->number);
                clockl +=timestep;
            }
            list_remove(runningthreads, actualrunning);
            if(actualrunning->runtime < actualrunning->zielzeit)
                list_append2(list, actualrunning);
            actualrunning = NULL;
        } else {
            if(prioact == 10) {
                print_time_step(clockl, 0);
                clockl +=timestep;
            }
            prioact = 10;
        }
    }
}
void shortestremainingtimenext(list_t *list, int timestep, int timequantum) {
    list_t *runningthreads = list_init();
    struct list_elem *actualrunning = NULL;
    while(1) {
        if(list_length(list) == 0 && list_length(runningthreads) == 0) {
            break;
        }
        struct list_elem *searcher = list->first;
        while(searcher != NULL) {
            struct list_elem *next = searcher->next;
            if(searcher->startzeit <= clockl && searcher->runtime < searcher->zielzeit) {
                list_remove(list, searcher);
                struct list_elem *pos = runningthreads->first;
                while(pos != NULL && (searcher->zielzeit-searcher->runtime) >= (pos->zielzeit-pos->runtime))
                    pos = pos->next;
                searcher->next = NULL;
                list_insertBeforePosition(runningthreads, searcher, pos);
            }
            searcher = next;
            
        }
        actualrunning = runningthreads->first;
        if(actualrunning != NULL) {
            int clockalt = clockl;
            while((clockl - clockalt) < timequantum) {
                actualrunning->runtime += timestep;
                print_time_step(clockl, actualrunning->number);
                clockl +=timestep;
            }
            list_remove(runningthreads, actualrunning);
            if(actualrunning->runtime < actualrunning->zielzeit)
                list_append2(list, actualrunning);
            actualrunning = NULL;
        } else {
            print_time_step(clockl, 0);
            clockl +=timestep;
        }
    }
}

int main(int argc, char *argv[]) {
    list_t *threadList = list_init();
    if(threadList == NULL) {
        printf("ERROR in initializing list\n");
        return -1;
    }
    int n = atoi(argv[2]);
    int t = atoi(argv[4]);
    int q = atoi(argv[6]);
    if(n > 10)
        perror("Too many Threads");
    for(int i = 0; i<n; i++) {
        struct list_elem *eingabe = (struct list_elem *) malloc(sizeof(struct list_elem));
        if(eingabe == NULL) {
            printf("Error in allocating memory for element");
            return -1;
        }
        int prio = 0;
        int startzeit = 0;
        int laufzeit = 0;
        scanf("%d %d %d", &prio, &startzeit, &laufzeit);
        if(prio < 1 || prio > 10)
            perror("Wrong priority");
        if(startzeit > 100000)
            perror("Too late starttime");
        if(laufzeit > 30000)
            perror("Too long runtime");
        eingabe->number = i+1;
        eingabe->priority = prio;
        eingabe->startzeit = startzeit;
        eingabe->zielzeit = laufzeit;
        list_append2(threadList, eingabe);
    }
    if(strncmp(argv[8], "RR", 2) == 0) {
        roundrobin(threadList, t, q);
    }
    if(strncmp(argv[8], "PRR", 3) == 0) {
        priorityroundrobin(threadList, t, q);
    }
    if(strncmp(argv[8], "SRTN", 2) == 0) {
        shortestremainingtimenext(threadList, t, q);
    }
    return 0;
}
