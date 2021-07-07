#ifndef QUEUE_H
#define QUEUE_H
#include "node.h"

// head is first, head->next is next in queue
// tail->next will always be NULL
// head->a->b->...->tail->NULL

typedef struct Queue {
    Node *head;
    Node *tail;
} Queue;

Queue *enqueue(Queue *q, void *data);
Node *dequeue(Queue *q);
void free_queue(Queue *q);
void queue_pretty_print(Queue *q);

#endif // QUEUE_H
