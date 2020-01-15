#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

/*
 * Initialize the list with *first = NULL and *last = NULL
 *
 * RETURN: Pointer to the created list or NULL when error occured
 */
list_t *list_init () {
    list_t *list = (list_t *) malloc(sizeof(list));
    if(list != NULL) {
        list->first = NULL;
        list->last = NULL;
    }
    return list;
}

/*
 * Insert the element to the beginning of the list.
 * Next pointer of the element set to first element of list.
 * If the element is the first element in the list, last pointer is pointing at it.
 *
 * RETURN: pointer to the inserted element or NULL
 */
struct list_elem *list_insert (list_t *list, char *data) {
    struct list_elem *element = (struct list_elem *) malloc(sizeof(struct list_elem));
    if(element == NULL)
        return NULL;
    element->data = data;
    if(list->first == NULL && list->last == NULL) {
        list->first = element;
        list->last = element;
        return element;
    }
    element->next = list->first;
    list->first = element;
    return element;
}

/*
 * Insert the element to the end of the list.
 * Next pointer of the element set to first element of list.
 * If the element is the first element in the list, last pointer is pointing at it.
 *
 * RETURN: pointer to the inserted element or NULL
 */
struct list_elem *list_append (list_t *list, char *data) {
    struct list_elem *element = (struct list_elem *) malloc(sizeof(struct list_elem));
    if(element == NULL)
        return NULL;
    element->data = data;
    if(list->first == NULL && list->last == NULL) {
        list->first = element;
        list->last = element;
        element->next = NULL;
        return element;
    }
    element->next = NULL;
    list->last->next = element;
    list->last = element;
    return element;
}

/*
 * Remove the element from the list.
 * If list is empty after removing, both first and last point to NULL.
 *
 * RETURN: 0 bei Erfolg, -1 bei Fehler
 */
int list_remove (list_t *list, struct list_elem *elem) {
    if(list->first == elem && list->last == elem) {
        list->first = NULL;
        list->last = NULL;
        free(elem);
        return 0;
    }
    if(list->first == elem) {
        list->first = elem->next;
        free(elem);
        return 0;
    }
    struct list_elem *actual = list->first;
    while(actual != NULL) {
        if(actual->next == elem) {
            actual->next = elem->next;
            if(list->last == elem) {
                list->last = actual;
            }
            free(elem);
            list->last->next = NULL;
            return 0;
        }
        actual = actual->next;
    }
    return -1;
}

/*
 * Empty the hole list.
 * First and last point to NULL.
 */
void list_finit (list_t *list) {
    struct list_elem *actual = list->first;
    while(actual != NULL) {
        struct list_elem *next = actual->next;
        list_remove(list, actual);
        actual = next;
    }
    list->first = NULL;
    list->last = NULL;
}

/*
 *
 */
struct list_elem *list_find (list_t *list, char *data, int (*cmp_elem) (const char *, const char *)) {
    struct list_elem *actual = list->first;
    while(actual != NULL) {
        if(cmp_elem(actual->data, data) == 0) {
            return actual;
        } else {
            actual = actual->next;
        }
    }
    return NULL;
}

/*
 * prints the data till a char '\0' gets red.
 */
void print_string (char *data) {
    printf("%s\n", data);
}

/*
 * formating
 */
void list_print (list_t *list, void (*print_elem) (char *)) {
    int i = 1;
    struct list_elem *actual = list->first;
    while(actual != NULL) {
        printf("%d:", i++);
        print_elem(actual->data);
        actual = actual->next;
    }
}
