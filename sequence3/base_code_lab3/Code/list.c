/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement *previous;
	struct s_LinkedElement *next;
} LinkedElement;

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement * whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement *sentinel;
	int size;
};


/*-----------------------------------------------------------------*/

List *list_create() {
	List * l = malloc(sizeof(List));
	l->sentinel = malloc(sizeof(LinkedElement));;
	l->size = 0;
	l->sentinel->value = 0;
	l->sentinel->next = l->sentinel;
	l->sentinel->previous = l->sentinel;
	return l;
}

/*-----------------------------------------------------------------*/

List *list_push_back(List *l, int v) {
	LinkedElement * new_element = malloc(sizeof(LinkedElement));
	new_element->value = v;
	new_element->previous = l->sentinel->previous;
	new_element->next = l->sentinel;
	l->sentinel->previous = new_element;
	new_element->previous->next = new_element;
	l->size++;

	return l;
}

/*-----------------------------------------------------------------*/

void list_delete(ptrList *l) {
	LinkedElement * nextToDestroy;
	LinkedElement * currentToDestroy;
	List * list = *l;
	currentToDestroy = list->sentinel->next;
	nextToDestroy = currentToDestroy->next;
	while (nextToDestroy != list->sentinel) {
		free(currentToDestroy);
		currentToDestroy = nextToDestroy;
		nextToDestroy = currentToDestroy->next;
	}
	free(currentToDestroy);
	free(nextToDestroy);
	free(list);
	l = NULL;
}

/*-----------------------------------------------------------------*/

List *list_push_front(List *l, int v) {

	LinkedElement * new_element = malloc(sizeof(LinkedElement));
	new_element->value = v;
	new_element->previous = l->sentinel;
	new_element->next = l->sentinel->next;
	l->sentinel->next = new_element;
	new_element->next->previous = new_element;
	l->size++;
	return l;
}

/*-----------------------------------------------------------------*/

int list_front(List *l) {
	return l->sentinel->previous->value;
}

/*-----------------------------------------------------------------*/

int list_back(List *l) {
	return l->sentinel->next->value;
}

/*-----------------------------------------------------------------*/

List *list_pop_front(List *l) {
	l->sentinel->previous = l->sentinel->previous->previous;
	free(l->sentinel->previous->next);
	l->sentinel->previous->next = l->sentinel;

	return l;
}

/*-----------------------------------------------------------------*/

List *list_pop_back(List *l){
	l->sentinel->next = l->sentinel->next->next;
	free(l->sentinel->next->previous);
	l->sentinel->next->previous = l->sentinel;

	return l;
}

/*-----------------------------------------------------------------*/

List *list_insert_at(List *l, int p, int v) {
	LinkedElement * futurNext;
	LinkedElement * elementToInsert = malloc(sizeof(LinkedElement));
	elementToInsert->value = v;
	futurNext = l->sentinel;

	for (int i = 0; i < p; i++) {
		futurNext = futurNext->next;
	}

	LinkedElement * futurPrevious = futurNext->previous;
	futurNext->previous = elementToInsert;
	elementToInsert->next = futurNext;
	elementToInsert->previous = futurPrevious;
	futurPrevious->next = elementToInsert;

	return l;
}

/*-----------------------------------------------------------------*/

List *list_remove_at(List *l, int p) {
	(void)p;
	return l;
}

/*-----------------------------------------------------------------*/

int list_at(List *l, int p) {
	(void)l;
	return p;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(List *l) {
	(void)l;
	return false;
}

/*-----------------------------------------------------------------*/

int list_size(List *l) {
	(void)l;
	return 0;
}

/*-----------------------------------------------------------------*/

List * list_map(List *l, SimpleFunctor f) {
	(void)f;
	return l;
}


List *list_reduce(List *l, ReduceFunctor f, void *userData) {
	(void)f;
	(void)userData;
	return l;
}

/*-----------------------------------------------------------------*/

List *list_sort(List *l, OrderFunctor f) {
	(void)f;
	return l;
}

