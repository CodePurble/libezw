#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack *push(Stack *st, void *data)
{
    if(!st) {
        st = malloc(sizeof(Stack));
        st->top = NULL;
        st->bottom = NULL;
    }

    Node *new_node = create_node(data);

    if(!st->top) {
        st->top = new_node;
        st->bottom = new_node;
    }
    else {
        new_node->next = st->top;
        st->top = new_node;
    }
    return st;
}

Node *pop(Stack *st)
{
    Node *node = NULL;
    if(st->top) {
        node = st->top;
        if(st->top == st->bottom) {
            st->top = NULL;
            st->bottom = NULL;
        }
        else {
            st->top = st->top->next;
        }
    }
    return node;
}

/* void free_stack(Stack *st) */
/* { */
/* } */
/*  */
void stack_pretty_print(Stack *st)
{
    Node *curr = st->top;
    while(curr) {
        printf("%d ", *(int *) curr->data);
        curr = curr->next;
    }
    printf("\n");
}

