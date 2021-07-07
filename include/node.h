#ifndef NODE_H
#define NODE_H

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

Node *create_node(void *data);
void free_node(Node *n);

#endif // NODE_H
