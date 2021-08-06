#ifndef QUEUE_H
#define QUEUE_H
#include "node.h"

// head is first, head->next is next in queue
// tail->next will always be NULL
// head->a->b->...->tail->NULL

typedef struct Queue {
    Node *head;
    Node *tail;
    int len;
} Queue;

enum q_type {
    SMAP_TREE_NODE,
    MINI_HDR,
    INT,
    DOUBLE
};

// OPTIMIZE: Maybe pass by reference? Soo many copies!!

Queue *enqueue(Queue *q, void *data);
Node *dequeue(Queue *q);
void free_queue(Queue *q);
void queue_pretty_print(Queue *q, enum q_type t);

#endif // QUEUE_H

