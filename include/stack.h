#ifndef STACK_H
#define STACK_H
#include "node.h"

// top is first, top->next is next in stack
// bottom->next will always be NULL
// top->a->b->...->bottom->NULL

typedef struct Stack {
    Node *top;
    Node *bottom;
} Stack;

// OPTIMIZE: Maybe pass by reference? Soo many copies!!

Stack *push(Stack *st, void *data);
Node *pop(Stack *st);
void free_stack(Stack *st);
void stack_pretty_print(Stack *st);


#endif // STACK_H
