#include <stdlib.h>
#include "node.h"

Node *create_node(void *data)
{
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_node(Node *n)
{
    if(n->data) {
        free(n->data);
    }
    free(n);
}
