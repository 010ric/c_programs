#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int main ( int argc , char *argv [] , char *envp []) {
    list_t *list;
    if((list = list_init()) == NULL) {
        perror("ERROR - No list initialized");
        exit(-1);
    }
    for(int i = 1; i< argc; i++) {
        if(strcmp(argv[i], "-a") == 0) {
            list_append(list, argv[++i]);
        }
        if(strcmp(argv[i], "-i") == 0) {
            list_insert(list, argv[++i]);
        }
        if(strcmp(argv[i], "-r") == 0) {
            struct list_elem *found = list_find(list, argv[++i], strncmp);
            if(found != NULL)
                list_remove(list, found);
        }
    }
    list_print (list , print_string);
    return 0;
}


