#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "node.h"
#include "utils.h"

Queue *enqueue(Queue *q, void *data)
{
    if(!q) {
        q = malloc(sizeof(Queue));
        q->head = NULL;
        q->tail = NULL;
    }

    Node *new_node = create_node(data);

    if(!q->head) {
        q->head = new_node;
        q->tail = new_node;
    }
    else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    return q;
}

Node *dequeue(Queue *q)
{
    Node *node = NULL;
    if(q->head) {
        node = q->head;
        if(q->head == q->tail) {
            q->head = NULL;
            q->tail = NULL;
        }
        else {
            q->head = q->head->next;
        }
    }
    return node;
}

void free_queue(Queue *q)
{
    while(q->head) {
        free_node(dequeue(q));
    }
    free(q);
}

// only for testing, will segfault otherwise
void queue_pretty_print(Queue *q)
{
    Node *curr = q->head;
    while(curr) {
        printf("%d ", *(int *) curr->data);
        curr = curr->next;
    }
    printf("\n");
}
