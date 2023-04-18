#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "skiplist.h"
#include "rng.h"

#define PREV 0
#define NEXT 1


typedef struct s_node Node;

struct s_level {
	Node * next;
	Node * previous;
};

struct s_node {
	int value;
	int level;
	struct s_level ** tab_levels;
};

struct s_SkipList {
	int size;
	Node * sentinelle;
	RNG rng;
};


Node * node_create(int level, int value) {

	Node * myNode = malloc(sizeof(struct s_node));

	myNode->level = level;
	myNode->tab_levels = malloc(sizeof(struct s_level *) * level);

	for (int i = 0; i < level; i++) {
		myNode->tab_levels[i] = malloc(sizeof(struct s_level));
	}

	myNode->value = value;
	
	return myNode;
}


Node * sentinelle_create(int level) {
	Node * sentinelle = node_create(level, 0);
	for (int i = 0; i < level; i++) {
		
		sentinelle->tab_levels[i]->next = sentinelle;
		sentinelle->tab_levels[i]->previous = sentinelle;
	}
	
	return sentinelle;
}


SkipList skiplist_create(int nblevels) {

	SkipList myList = malloc(sizeof(struct s_SkipList));
	myList->size = 0;

	Node * sentinelle = sentinelle_create(nblevels);

	myList->sentinelle = sentinelle;

	myList->rng = rng_initialize(0);

	
	return myList;
}

void skiplist_delete(SkipList d) {
	Node * current_node_to_delete = d->sentinelle;
	Node * next_node_to_delete = current_node_to_delete->tab_levels[0]->next;

	while(next_node_to_delete != d->sentinelle) {
		for (int i = 0; i < current_node_to_delete->level; i++) {
			free(current_node_to_delete->tab_levels[i]);
		}
		free(current_node_to_delete);
		current_node_to_delete = next_node_to_delete;
		next_node_to_delete = current_node_to_delete->tab_levels[0]->next;
	}

	free(current_node_to_delete);

	free(d);
}

unsigned int skiplist_size(SkipList myList) {
	return myList->size;
}

int skiplist_ith(SkipList myList, unsigned int i) {
	Node * current_node = myList->sentinelle;
	for (unsigned int j = 0; j < i; j++) {
		current_node = current_node->tab_levels[0]->next;
	}

	return current_node->value;
}

void skiplist_map(SkipList myList, ScanOperator f, void*user_data) {
	Node * current_node = myList->sentinelle->tab_levels[0]->next;
	for (int i = 0; i < myList->size; i++) {
		f(current_node->value, user_data);
		current_node = current_node->tab_levels[0]->next;
	}
}


SkipList skiplist_insert(SkipList myList, int value) {
	
	unsigned int new_node_level; 
	new_node_level = rng_get_value(&myList->rng, (unsigned int)(myList->sentinelle->level - 1)) + 1;

	Node * newNode = node_create(new_node_level, value);

	Node * nextNode = myList->sentinelle->tab_levels[0]->next;

	if (nextNode->value == value && nextNode != myList->sentinelle) {
		
		return myList;
	}

	while (nextNode->value < newNode->value && nextNode != myList->sentinelle) {
		nextNode = nextNode->tab_levels[0]->next;
	}

	if (nextNode->value == value && nextNode != myList->sentinelle) {
		
		return myList;
	}

	Node * previousNode;

	for (int i = 0; i < newNode->level; i++) {
		while (nextNode->level < i) {
			nextNode = nextNode->tab_levels[0]->next;
		}

		previousNode = nextNode->tab_levels[i]->previous;

		newNode->tab_levels[i]->next = nextNode;
		nextNode->tab_levels[i]->previous = newNode;
		
		newNode->tab_levels[i]->previous = previousNode;
		previousNode->tab_levels[i]->next = newNode;

		nextNode = newNode->tab_levels[0]->next;
	}

	myList->size++;

	return myList;

}