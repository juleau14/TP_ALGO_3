#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "skiplist.h"
#include "rng.h"

#define PREV 0
#define NEXT 1

typedef struct s_node Node;

struct s_node {
	int value;
	int level;
	Node * tab_nodes[][2];
};

struct s_SkipList {
	int size;
	Node * sentinelle;
	RNG rng;
};


Node * node_create(int level, int value) {

	CHANGER LE SYSTEME DE NIVEAU, ESSAYER DE FAIRE UNE STRUCT COMME SUIT :
	STRUCT NIVEAU {
		INT LEVEL;
		NODE * NEXT;
		NODE * PREV;
	}
	ET LE NODE CONTIENT UN TABLEAU : STRUCT NIVEAU MESNIVEAUX[NBLVL]

	Node * myNode = malloc(sizeof(struct s_node));

	myNode->level = level;
	myNode->tab_nodes = malloc(sizeof(Node *) * level * 2);

	myNode->value = value;

	return myNode;
}


Node * sentinelle_create(int level) {
	Node * sentinelle = node_create(level, 0);
	for (int i = 0; i < level; i++) {
		printf("%d\n", i);
		sentinelle->tab_nodes[i][PREV] = sentinelle;
		sentinelle->tab_nodes[i][NEXT] = sentinelle;
	}
	return sentinelle;
}


SkipList skiplist_create(int nblevels) {

	printf("Création de la liste\n");

	SkipList myList = malloc(sizeof(struct s_SkipList));
	myList->size = 0;

	printf("Creation sentinelle\n");
	Node * sentinelle = sentinelle_create(nblevels);
	printf("youhou\n");

	myList->sentinelle = sentinelle;

	myList->rng = rng_initialize(0);

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

	free(d);
}

unsigned int skiplist_size(SkipList myList) {
	return myList->size;
}

int skiplist_ith(SkipList myList, unsigned int i) {
	Node * current_node = myList->sentinelle;
	for (unsigned int j = 0; j < i; j++) {
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
	printf("Insertion de la valeur %d\n", value);
	unsigned int new_node_level; 
	new_node_level = rng_get_value(&myList->rng, (unsigned int)(myList->sentinelle->level));

	Node * newNode = node_create(new_node_level, value);

	Node * nextNode = myList->sentinelle->tab_nodes[0][NEXT];

	while (nextNode->value < newNode->value && nextNode != myList->sentinelle) {
		nextNode = nextNode->tab_nodes[0][NEXT];
	}

	Node * previousNode = nextNode->tab_nodes[0][PREV];

	for (int i = 0; i < newNode->level; i++) {
		while (nextNode->level < i) {
			nextNode = nextNode->tab_nodes[0][NEXT];
		}

		previousNode = nextNode->tab_nodes[i][PREV];

		newNode->tab_nodes[i][NEXT] = nextNode;
		nextNode->tab_nodes[i][PREV] = newNode;
		
		newNode->tab_nodes[0][PREV] = previousNode;
		previousNode->tab_nodes[0][NEXT] = newNode;

		nextNode = newNode->tab_nodes[0][NEXT];
	}

	return myList;

}