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

typedef struct s_SubList {
	LinkedElement * front;
	LinkedElement * back;
} SubList;

SubList list_split(SubList l);
SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f);
SubList list_mergesort(SubList l, OrderFunctor f);

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
	return l->sentinel->next->value;
}

/*-----------------------------------------------------------------*/

int list_back(List *l) {
	return l->sentinel->previous->value;
}

/*-----------------------------------------------------------------*/

List *list_pop_front(List *l) {
	l->sentinel->next = l->sentinel->next->next;
	free(l->sentinel->next->previous);
	l->sentinel->next->previous = l->sentinel;
	l->size--;

	return l;
}

/*-----------------------------------------------------------------*/

List *list_pop_back(List *l){
	l->sentinel->previous = l->sentinel->previous->previous;
	free(l->sentinel->previous->next);
	l->sentinel->previous->next = l->sentinel;
	l->size--;

	return l;
}

/*-----------------------------------------------------------------*/

List *list_insert_at(List *l, int p, int v) {
	LinkedElement * futurNext;
	LinkedElement * elementToInsert = malloc(sizeof(LinkedElement));
	elementToInsert->value = v;
	futurNext = l->sentinel->next;

	for (int i = 0; i < p; i++) {
		futurNext = futurNext->next;
	}

	LinkedElement * futurPrevious = futurNext->previous;
	futurNext->previous = elementToInsert;
	elementToInsert->next = futurNext;
	elementToInsert->previous = futurPrevious;
	futurPrevious->next = elementToInsert;

	l->size++;

	return l;
}

/*-----------------------------------------------------------------*/

List *list_remove_at(List *l, int p) {
	LinkedElement * elementToRemove = l->sentinel->next;
	for (int i = 0; i < p; i++) {
		elementToRemove = elementToRemove->next;
	}
	elementToRemove->previous->next = elementToRemove->next;
	elementToRemove->next->previous = elementToRemove->previous;
	free(elementToRemove);
	l->size--;
	return l;
}

/*-----------------------------------------------------------------*/

int list_at(List *l, int p) {
	LinkedElement * current = l->sentinel->next;
	for (int i = 0;i < p; i++) {
		current = current->next;
	}

	return current->value;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(List *l) {
	bool result = false;
	if (l->size == 0) {
		result = true;
	}
	return result;
}

/*-----------------------------------------------------------------*/

int list_size(List *l) {
	return l->size;
}

/*-----------------------------------------------------------------*/

List * list_map(List *l, SimpleFunctor f) {
	for (LinkedElement * e = l->sentinel->next; e != l->sentinel; e = e->next) {
		e->value = f(e->value);
	}
	return l;
}


List *list_reduce(List *l, ReduceFunctor f, void *userData) {
	for (LinkedElement * e = l->sentinel->next; e != l->sentinel; e = e->next) {
		f(e->value, userData);
	}
	return l;
}

/*-----------------------------------------------------------------*/

List *list_sort(List *l, OrderFunctor f) {
	SubList subList;
	subList.front = l->sentinel->next;
	subList.back = l->sentinel->previous;
	subList.front->previous = NULL;
	subList.back->next = NULL;
	subList = list_mergesort(subList, f);
	l->sentinel->next = subList.front;
	l->sentinel->next->previous = l->sentinel;
	l->sentinel->previous = subList.back;
	l->sentinel->previous->next = l->sentinel;
	return l;
}


SubList list_split(SubList l) {
	LinkedElement * iterator1 = l.front;
	LinkedElement * iterator2 = l.front;

	while (iterator2 -> next != NULL) {
		iterator1 = iterator1->next;
		iterator2 = iterator2->next->next;
	}

	SubList result;
	result.front = iterator1;
	result.back = iterator1->next;
	// iterator1->next = NULL;
	// iterator2->previous = NULL;

	return result;
}


SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f) {
	SubList result;

	if (f(leftlist.back->value, rightlist.front->value)) {
		leftlist.back->next = rightlist.front;
		rightlist.front->previous = leftlist.back;
		result.front = leftlist.front;
		result.back = rightlist.back;
	} else {
		rightlist.back->next = leftlist.front;
		leftlist.front->previous = rightlist.back;
		result.front = rightlist.front;
		result.back = leftlist.back;
	}

	return result;
}


SubList list_mergesort(SubList l, OrderFunctor f) {
	if (l.back == l.front) {
		return l;
	} else {
		SubList splitedList = list_split(l);
		SubList leftList;
		SubList rightList;
		leftList.front = l.front;
		leftList.back = splitedList.front;
		rightList.front = splitedList.back;
		rightList.back = l.back;

		return list_merge(list_mergesort(leftList, f), list_mergesort(rightList, f), f);
	}
}