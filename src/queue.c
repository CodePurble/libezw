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
        q = (Queue *) malloc(sizeof(Queue));
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

void queue_pretty_print(Queue *q, enum q_type t)
{
    if(q) {
        Node *curr = q->head;
        switch(t) {
            case SMAP_TREE_NODE: {
                Smap_tree_node *curr_smap_node = NULL;
                while(curr) {
                    curr_smap_node = (Smap_tree_node *) curr->data;
                    printf("(%f %s) ", curr_smap_node->coeff,
                            smap_symbol_to_str(curr_smap_node->type));
                    curr = curr->next;
                }
                break;
            }
            case INT: {
                int *val = NULL;
                while(curr) {
                    val = (int *) curr->data;
                    printf("%d ", *val);
                    curr = curr->next;
                }
                break;
            }
            case DOUBLE: {
                double *val = NULL;
                while(curr) {
                    val = (double *) curr->data;
                    printf("%f ", *val);
                    curr = curr->next;
                }
                break;
            }
        }
            printf("\n");
    }
    else {
        printf("Queue is empty\n");
    }
}
