#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "node.h"
#include "smap.h"
#include "utils.h"

Queue *enqueue(Queue *q, void *data)
{
    // If no queue
    if(!q) {
        q = malloc(sizeof(Queue));
        q->head = NULL;
        q->tail = NULL;
    }

    Node *new_node = create_node(data);

    // Only for first element
    if(!q->head) {
        q->head = new_node;
        q->tail = new_node;
    }
    else {
        // add new node to linked-list, and
        // move the tail
        q->tail->next = new_node;
        q->tail = new_node;
    }
    return q;
}

Node *dequeue(Queue *q)
{
    Node *node = NULL;
    if(q->head) {
        node = q->head; // save the head
        if(q->head == q->tail) {
            // queue is empty
            q->head = NULL;
            q->tail = NULL;
        }
        else {
            // move head to next in queue
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
    Smap_tree_node *curr_smap_node = NULL;
    while(curr) {
        curr_smap_node = (Smap_tree_node *) curr->data;
        printf("%f ", curr_smap_node->coeff);
        curr = curr->next;
    }
    printf("\n");
}
