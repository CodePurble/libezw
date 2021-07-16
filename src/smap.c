#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smap.h"
#include "utils.h"

Smap_tree_node *smap_tree_init_node(double coeff)
{
    Smap_tree_node *new_node = (Smap_tree_node *) malloc(sizeof(Smap_tree_node));
    new_node->coeff = coeff;
    new_node->type = U;
    new_node->children[0] = NULL;
    new_node->children[1] = NULL;
    new_node->children[2] = NULL;
    new_node->children[3] = NULL;
    new_node->isroot = 0;
    return new_node;
}

Smap_tree_node *smap_tree_init_root(double val,
                                    double c1val,
                                    double c2val,
                                    double c3val)
{
    Smap_tree_node *new_node = smap_tree_init_node(val);
    Smap_tree_node *c2 = smap_tree_init_node(c1val);
    Smap_tree_node *c3 = smap_tree_init_node(c2val);
    Smap_tree_node *c4 = smap_tree_init_node(c3val);
    new_node->children[1] = c2;
    new_node->children[2] = c3;
    new_node->children[3] = c4;
    new_node->isroot = 1;
    return new_node;
}

/**
 * Assume the first 4 Smap_tree_nodes are already in the queue
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
Queue *smap_tree_insert_quad(Queue *q, double *cvals)
{
    int start = 0;
    Node *head = dequeue(q);
    Smap_tree_node *curr = (Smap_tree_node *) head->data;
    // DEBUG_DOUBLE("inserting quad at", curr->coeff);
    // DEBUG_ARR_F_1(cvals, 4);
    // DEBUG_BLANK;
    if(curr->isroot) {
        start = 1;
    }
    for(int i = start; i < 4; i++) {
        curr->children[i] = smap_tree_init_node(cvals[i]);
        q = enqueue(q, curr->children[i]);
    }
    // queue_pretty_print(q);
    // printf("\n");
    return q;
}

Smap_tree_node *smap_treeify(SBtree_node *sb_root, int levels)
{
    Queue *q = NULL;
    Smap_tree_node *smap_root = smap_tree_init_root(sb_root->ll->coeffs[0],
                                                    sb_root->hl->coeffs[0],
                                                    sb_root->lh->coeffs[0],
                                                    sb_root->hh->coeffs[0]);

    // First level, enqueue the 3 valid children
    for(int i = 1; i < 4; i++) {
        q = enqueue(q, smap_root->children[i]);
    }

    // Coeffs of all subbands in current level
    double *curr_coeffs[3] = {NULL};
    int dim;
    int num_quads = 0;
    // Rest of the levels
    for(int i = 2; i <= levels; i++) {
        dim = pow(2, i-1);
        num_quads = (dim*dim)/4;
        double **quads = (double **) calloc(sizeof(double *), num_quads);

        curr_coeffs[0] = sb_tree_get_coeff(sb_root, HL, i);
        curr_coeffs[1] = sb_tree_get_coeff(sb_root, LH, i);
        curr_coeffs[2] = sb_tree_get_coeff(sb_root, HH, i);

        quads = quads_from_arr(curr_coeffs[0], dim, dim);
        for(int j = 0; j < num_quads; j++) {
            q = smap_tree_insert_quad(q, quads[j]);
        }
        quads = quads_from_arr(curr_coeffs[1], dim, dim);
        for(int j = 0; j < num_quads; j++) {
            q = smap_tree_insert_quad(q, quads[j]);
        }
        quads = quads_from_arr(curr_coeffs[2], dim, dim);
        for(int j = 0; j < num_quads; j++) {
            q = smap_tree_insert_quad(q, quads[j]);
        }

        free(quads);
    }
    free(q);
    return smap_root;
}

void smap_tree_print_preorder(Smap_tree_node *root, enum print_conf p)
{
    if(root) {
        switch(p) {
            case COEFF:
                printf("%f\n", root->coeff);
                break;
            case TYPE:
                printf("%s\n", smap_symbol_to_str(root->type));
                break;
            case FULL:
                printf("(%f, %s)\n", root->coeff, smap_symbol_to_str(root->type));
                break;
        }
    }
    if(root->children[0]) {
        smap_tree_print_preorder(root->children[0], p);
    }
    if(root->children[1]) {
        smap_tree_print_preorder(root->children[1], p);
    }
    if(root->children[2]) {
        smap_tree_print_preorder(root->children[2], p);
    }
    if(root->children[3]) {
        smap_tree_print_preorder(root->children[3], p);
    }
}

char* smap_symbol_to_str(enum smap_symbol s)
{
    switch(s) {
        case SP:
            return "SP";
        case SN:
            return "SN";
        case IZ:
            return "IZ";
        case ZR:
            return "ZR";
        case U:
            return "U";
    }
}
