#include <stdlib.h>
#include <assert.h>

#include "skiplist.h"
#include "rng.h"

#define PREV 0
#define NEXT 1



typedef struct s_node {
	int value;
	int level;
	Node *** tab_nodes;
} Node;

struct s_SkipList {
	int size;
	Node * sentinelle;
	int random_number_generator;
};


Node * node_create(int level, int value) {

	Node * myNode = malloc(sizeof(struct s_node));

	myNode->level = level;
	Node *** tab_nodes = malloc(sizeof(Node *) * level * 2);
	myNode->tab_nodes = tab_nodes;

	myNode->value = value;

	return myNode;
}


SkipList skiplist_create(int nblevels) {

	SkipList myList;
	myList->size = 0;

	Node * sentinelle = node_create(nblevels, 0);

	myList-> random_number_generator = 0;

	return myList;
}

void skiplist_delete(SkipList d) {
	Node * current_node_to_delete = d->sentinelle;
	Node * next_node_to_delete = current_node_to_delete->tab_nodes[0][NEXT];

	while(next_node_to_delete != d->sentinelle) {
		free(current_node_to_delete);
		current_node_to_delete = next_node_to_delete;
		next_node_to_delete = current_node_to_delete->tab_nodes[0][NEXT];
	}

	free(current_node_to_delete);

	free(&d);
}

unsigned int skiplist_size(SkipList myList) {
	return myList->size;
}

int skiplist_ith(SkipList myList, unsigned int i) {
	Node * current_node = myList->sentinelle;
	for (int j = 0; j < i; j++) {
		current_node = current_node->tab_nodes[0][NEXT];
	}

	return current_node->value;
}

void skiplist_map(SkipList myList, ScanOperator f, void*user_data) {
	Node * current_node = myList->sentinelle->tab_nodes[0][NEXT];
	for (int i = 0; i < myList->size; i++) {
		f(current_node->value, user_data);
		current_node = current_node->tab_nodes[0][NEXT];
	}
}


SkipList skiplist_insert(SkipList myList, int value) {
	int new_node_level = rng_get_value(&myList->random_number_generator, 100);
	Node * myNode = node_create(new_node_level, value);
}