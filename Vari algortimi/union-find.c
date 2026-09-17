#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct node {
	int32_t value;
		
	struct set* set;
	struct node* next;
};	

struct set {
	struct node* head;
	struct node* tail;
};	

struct node* node_init(int32_t value)
{
	struct node* node = NULL;
	if ((node = malloc(sizeof(struct node)))) {
		node->value = value;
		node->set = NULL;
		node->next = NULL;
	}	
	return node;
}

struct set* make_set(struct node* node)
{
	struct set* set = NULL;

	if ((set = malloc(sizeof(struct set)))) {
		set->head = node;
		set->tail = node;
	}
	node->set = set;

	return set;
}

struct set* union_set(struct set* s1, struct set* s2) 
{
	struct node* temp = s2->head;

	while (temp != NULL) {
		temp->set = s1;
		temp = temp->next;
	}

 	s1->tail->next = s2->head;
 	s1->tail = s2->tail;
 	s2->tail = s1->tail;
 	s2->head = s1->head;
}

struct node* find_set(struct node* node)
{
	return node->set->head;
}

int main() 
{
	struct node* node[10];
	struct set*  set[10];
	
	for (size_t i = 0; i < 10; ++i) {
		node[i] = node_init(i);
		set[i]  = make_set(node[i]);
	}
	
	union_set(set[4], set[1]);
	union_set(set[1], set[2]);
	
	if (find_set(node[0]) == find_set(node[1])) {
		printf("ok\n");
	}	

	printf("%d\n", find_set(node[2])->value);

	for (size_t i = 0; i < 10; ++i) {
		free(set[i]);
		free(node[i]);
	}
	
	return EXIT_SUCCESS;
}
