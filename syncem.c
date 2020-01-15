#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

int *handleChild(int *arg);
int getRand(int from, int to);
int waitChild(int id);

pthread_mutex_t konsole;

int main(int argc, char **argv) {
    //int k = 10;
    int n = 1;
    //int r = 0;
    int mode = 0;

    int c;
    while ((c = getopt(argc, argv, "n:lf")) != -1) {
        switch (c) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'l':
                mode = mode + 1;
                break;
            case 'f':
                mode = mode + 2;
                break;
            default:
                printf("usage: ./syncem [-n <# Threads>] [-l|-t]\n");
                return -1;
        }
    }
    if (n > 10) {
        perror("Too many threads shall be created");
        return -1;
    }
    if(mode == 3) {
        perror("Don't use -l and -f in one command");
        return -1;
    }
    
    pthread_t thread_id;
    if (pthread_mutex_init(&konsole, NULL) != 0) {
        printf("ERROR in initializing mutex");
        return -1;
    }
    
    
    list_t *childList = list_init();
    if(childList == NULL) {
        printf("ERROR in list\n");
    }
    // creating the threads and inserting them to the list
    for (int i = 0; i < n; i++) {
        // arg[0] will represent the threadnumber, arg[1] the locking -> 0 no lock, 1 -l and 2 -f
        int *arg = malloc(2*sizeof(int));
        arg[0] = i;
        arg[1] = mode;
        int err = pthread_create(&thread_id, NULL, handleChild, arg);
        if(err)
            printf("ERROR in creating threads\n");
        list_insert(childList, thread_id);
        
    }
    
    // joining the main thread and the created threads
    struct list_elem *currentChild = childList->first;
    int i = 0;
    while (i < n) {
        pthread_join(currentChild->data, NULL);
        currentChild = currentChild->next;
        i++;
    }


    list_finit(childList);
    pthread_mutex_destroy(&konsole);
    return 0;
}

/**
 * This method contains the code that is executed by each child.
 * 
 * @return (Child-PID + k) % 100
 */
int *handleChild(int *arg) {
    //printf("%d %d\n", arg[0], arg[1]);
    int fp = -1;
    char fileName[] = "a.txt";
    fileName[0] = arg[0] + 48;
    //printf("%d %s\n", arg[0], fileName);
    
    fp = open(fileName, O_RDONLY);
    if(fp < 0){
        printf("Fehler beim Öffnen der Datei %s\n", fileName);
        return NULL;
    }
    char *buf = malloc(sizeof(char) * 65);
    int g = 0;
    int counter = 0;
    if(arg[1] == 2)
        pthread_mutex_lock(&konsole);
    while((g = read(fp, buf, 64)) > 0 ) {
        if(arg[1] == 1)
            pthread_mutex_lock(&konsole);
        char reserved[9];
        sprintf(reserved,"[%02d] %03d\t", arg[0], counter++ );
        //printf("[%02d] %03d\t", arg[0], counter++);
        buf[g] = '\0';
        write(1, reserved, 9);
        write(1, buf, g);
        //printf("%s", buf);
        write(1, "\n",1);
        //printf("\n");
        if(arg[1] == 1)
            pthread_mutex_unlock(&konsole);
    }
    if(arg[1] == 2)
        pthread_mutex_unlock(&konsole);
    
    close(fp);
    
    pthread_exit(NULL);
}
