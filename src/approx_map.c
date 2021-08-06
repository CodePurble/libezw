#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "approx_map.h"
#include "utils.h"

Approx_map_node *amap_tree_init_node(unsigned char symbol)
{
    Approx_map_node *new_node = (Approx_map_node *) malloc(sizeof(Approx_map_node));
    new_node->coeff = symbol;
    new_node->parent = NULL;
    new_node->children[0] = NULL;
    new_node->children[1] = NULL;
    new_node->children[2] = NULL;
    new_node->children[3] = NULL;
    new_node->isroot = 0;
    return new_node;
}

Approx_map_node *amap_tree_init_root(unsigned char val,
                                    unsigned char c1val,
                                    unsigned char c2val,
                                    unsigned char c3val)
{
    Approx_map_node *new_node = amap_tree_init_node(val);
    Approx_map_node *c2 = amap_tree_init_node(c1val);
    Approx_map_node *c3 = amap_tree_init_node(c2val);
    Approx_map_node *c4 = amap_tree_init_node(c3val);
    new_node->parent = NULL;
    new_node->children[1] = c2;
    new_node->children[2] = c3;
    new_node->children[3] = c4;
    for(int i = 1; i < 4; i++) {
        new_node->children[i]->parent = new_node;
    }
    new_node->isroot = 1;
    return new_node;
}

/**
 * Assume the first 4 Approx_map_nodes are already in the queue
 * So the nodes given here:
 *        a
 *        |
 *   -----------
 *  /     |     \
 *  b     c     d
 *  should already be in the queue
 *  Maintain an external queue
 *  'a' need not be in the queue as it is the first to be enqueued
 *  so, the intial queue must be like this:
 *  [b, c, d]
 *
 *  Breadth first insertion:
 *  1) Dequeued value is inserted into the Smap_tree
 *  2) The newly added nodes are enqueued
 *
 *  New quads are inserted as children to the head of the queue, which is
 *  then dequeued. The newly inserted nodes are enqueued in order.
**/
Queue *amap_tree_insert_quad(Queue *q, unsigned char *cvals)
{
    int start = 0;
    Node *head = dequeue(q);
    Approx_map_node *curr = (Approx_map_node *) head->data;
    // DEBUG_DOUBLE("inserting quad at", curr->coeff);
    // DEBUG_ARR_F_1(cvals, 4);
    // DEBUG_BLANK;
    if(curr->isroot) {
        start = 1;
    }
    for(int i = start; i < 4; i++) {
        curr->children[i] = amap_tree_init_node(cvals[i]);
        curr->children[i]->parent = curr;
        q = enqueue(q, curr->children[i]);
    }
    // queue_pretty_print(q);
    // printf("\n");
    return q;
}

Approx_map_node *amap_treeify(SBtree_node *sb_root, int levels)
{
    Queue *q = NULL;
    Approx_map_node *amap_root = amap_tree_init_root(sb_root->ll->coeffs[0],
                                                    sb_root->hl->coeffs[0],
                                                    sb_root->lh->coeffs[0],
                                                    sb_root->hh->coeffs[0]);

    // First level, enqueue the 3 valid children
    for(int i = 1; i < 4; i++) {
        q = enqueue(q, amap_root->children[i]);
    }

    // Coeffs of all subbands in current level
    unsigned char *curr_coeffs[3] = {NULL};
    int dim;
    int num_quads = 0;
    // Rest of the levels
    for(int i = 2; i <= levels; i++) {
        dim = pow(2, i-1);
        num_quads = (dim*dim)/4;
        unsigned char **quads = (unsigned char **) calloc(sizeof(unsigned char *), num_quads);

        curr_coeffs[0] = sb_tree_get_coeff(sb_root, HL, i);
        curr_coeffs[1] = sb_tree_get_coeff(sb_root, LH, i);
        curr_coeffs[2] = sb_tree_get_coeff(sb_root, HH, i);

        for(int k = 0; k < 3; k++) {
            quads = quads_from_arr(curr_coeffs[k], dim, dim);
            for(int j = 0; j < num_quads; j++) {
                q = amap_tree_insert_quad(q, quads[j]);
            }
        }
        free(quads);
    }
    free(q);
    return amap_root;
}

void amap_tree_print_levelorder(Approx_map_node *root, enum print_conf p) {
    Queue *q = NULL;
    q = enqueue(q, root);
    while(q->head) {
        Node *qnode = dequeue(q);
        Approx_map_node *curr_amap = (Approx_map_node *) qnode->data;
        for(int i = 0; i < 4; i++) {
            if(curr_amap->children[i]) {
                q = enqueue(q, curr_amap->children[i]);
            }
        }
        switch(p) {
            case COEFF:
                printf("%f\n", curr_amap->coeff);
                break;
            case TYPE:
                printf("%s\n", amap_symbol_to_str(curr_amap->type));
                break;
            case ALL:
                printf("(%f, %s)\n", curr_amap->coeff, amap_symbol_to_str(curr_amap->type));
                break;
        }
    }
}

double *amap2levelorder(Approx_map_node *root, int rows, int cols)
{
    double *arr = (double *) calloc(rows*cols, sizeof(double));
    int i = 0;
    Queue *q = NULL;
    q = enqueue(q, root);
    while(q->head) {
        Node *qnode = dequeue(q);
        Approx_map_node *curr_amap = (Approx_map_node *) qnode->data;
        for(int i = 0; i < 4; i++) {
            if(curr_amap->children[i]) {
                q = enqueue(q, curr_amap->children[i]);
            }
        }
        arr[i] = curr_amap->coeff;
        i++;
    }
    return arr;
}

void amap_tree_print_preorder(Approx_map_node *root, enum print_conf p)
{
    if(root) {
        switch(p) {
            case COEFF:
                printf("%f\n", root->coeff);
                break;
            case TYPE:
                printf("%s\n", amap_symbol_to_str(root->type));
                break;
            case ALL:
                printf("(%f, %s)\n", root->coeff, amap_symbol_to_str(root->type));
                break;
        }
    }
    if(root->children[0]) {
        amap_tree_print_preorder(root->children[0], p);
    }
    if(root->children[1]) {
        amap_tree_print_preorder(root->children[1], p);
    }
    if(root->children[2]) {
        amap_tree_print_preorder(root->children[2], p);
    }
    if(root->children[3]) {
        amap_tree_print_preorder(root->children[3], p);
    }
}
