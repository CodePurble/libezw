#ifndef QUEUE_H
#define QUEUE_H
#include "smap.h"

// head is first, head->next is next in queue
// tail->next will always be NULL
// head->a->b->...->tail->NULL

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *head;
    Node *tail;
} Queue;

Node *create_node(void *data);
Queue *enqueue(Queue *q, void *data);
Node *dequeue(Queue *q);
void queue_pretty_print(Queue *q);

#endif // QUEUE_H
