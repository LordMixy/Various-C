#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NODES 30

struct node {
	int32_t value;
	struct node* next;
};	

struct node* node_init(int32_t value) 
{
	struct node* node;
	if ((node = malloc(sizeof(struct node)))) {
		node->value = value;
		node->next = NULL;
	}
	return node;
}

void node_insert(struct node** node, int32_t value) 
{
	struct node* to_insert = node_init(value);

	to_insert->next = *node; 
	*node = to_insert;
}

struct graph {
	size_t size;
	struct node* adjacencies[MAX_NODES];	
};

void graph_insert_edge(struct graph** graph, int32_t i, int32_t j)
{
	node_insert(&(*graph)->adjacencies[i], j);
	node_insert(&(*graph)->adjacencies[j], i);
}

struct graph* graph_init(size_t size) 
{
	struct graph* graph;
	if ((graph = malloc(sizeof(struct graph)))) {
		graph->size = size;
	}
	return graph;
}

void graph_print(struct graph* graph) 
{
	for (size_t i = 0; i < graph->size; ++i) {
		printf("%lu -> \n", i);
		for (struct node* head = graph->adjacencies[i]; head != NULL; head = head->next) {
			printf("\t%d\n", head->value);
		}
	}
}

struct queue {
	size_t  head;
	size_t  tail;
	int32_t data[MAX_NODES];
};	 

void queue_enqueue(struct queue* queue, int32_t value)
{
	queue->data[queue->tail] = value;
	if (queue->tail == MAX_NODES) {
		queue->tail = 0;
	} else {
		queue->tail++;
	}
}

int32_t queue_dequeue(struct queue* queue)
{
	int32_t value = queue->data[queue->head];
	if (queue->head == MAX_NODES) {
		queue->head = 0;
	} else {
		queue->head++;
	}
	return value;
}

bool queue_is_empty(struct queue* queue)
{
	return queue->head == queue->tail;
}

void BFS(struct graph* graph)
{
	bool* explored = calloc(graph->size, sizeof(bool));
	struct queue queue = { .tail = 0, .head = 0 };
	
	explored[0] = true;
	queue_enqueue(&queue, 0);

	while (!queue_is_empty(&queue)) 
	{
		int32_t v = queue_dequeue(&queue);

		for (struct node* w = graph->adjacencies[v]; w != NULL; w = w->next) {
			if (!explored[w->value]) {
				explored[w->value] = true;
				queue_enqueue(&queue, w->value);
			}
		}
	}
	
	free(explored);
}

int main()
{
	struct graph* graph = graph_init(5);
	graph_insert_edge(&graph, 0, 1);
	graph_insert_edge(&graph, 0, 5);
	graph_insert_edge(&graph, 1, 2);
	graph_insert_edge(&graph, 1, 3);
	graph_insert_edge(&graph, 1, 4);

	BFS(graph);
			
	return EXIT_SUCCESS;
}
