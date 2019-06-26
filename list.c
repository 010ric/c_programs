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
 * Fügt ein Element an den Anfang der Liste an.
 * Der Zeiger next des Elements wird auf den Zeiger von first der list gesetzt.
 * Wenn das Element das erste in der Liste ist, wird last von list auf dieses Element gesetzt.
 *
 * RETURN: Pointer auf das eingefügte Element bzw. NULL bei Fehler
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
 * Fügt ein Element an das Ende der Liste an.
 * Der Zeiger next des Elements wird auf NULL gesetzt.
 * Wenn das Element das erste in der Liste ist, wird first von list auf dieses Element gesetzt.
 *
 * RETURN: Pointer auf das eingefügte Element bzw. NULL bei Fehler
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
 * Entfernt das Element aus der Liste.
 * Wenn das Element das einzige in der Liste ist, wird first und last der Liste auf NULL gesetzt.
 * Wenn das Element das erste in der Liste ist, so wird dieses entfernt und first der Liste auf das nachfolgende gesetzt.
 * Ansonsten wird die Liste so lange durchgegangen, bis das Element gefunden wird.
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
 * Leert die Liste.
 * Setzt first und last der Liste auf NULL.
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
 * Sucht in der Liste nach dem Element, das nach cmp_elem denselben Inhalt hat wie data.
 *
 * RETURN: Element, das dem gesuchten entspricht oder NULL, falls kein Treffer
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
 * Gibt den Inhalt des Speichers ab data bis zu einem '\0' aus.
 */
void print_string (char *data) {
    printf("%s\n", data);
}

/*
 * Gibt die Liste im geforderten Format aus. Dazu wird die Variable i als Elementnummer benutzt.
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
