#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "ezw.h"
#include "queue.h"
#include "stack.h"

enum smap_symbol encode_coeff(float coeff, int threshold)
{
    enum smap_symbol symbol;
    if(fabs(coeff) >= threshold) {
        if(coeff >= 0) {
            symbol = SP;
        }
        else {
            symbol = SN;
        }
    }
    else {
        symbol = IZ;
    }
    return symbol;
}

void check_descendants(Smap_tree_node *parent, int threshold)
{
    unsigned char is_zr = 1;
    if(!parent) {
        is_zr = -1;
    }
    else {
        Stack *backtrack = NULL;
        Queue *q = NULL;

        q = enqueue(q, parent);
        backtrack = push(backtrack, parent);

        Node *curr_node;
        Smap_tree_node *curr_smap_node;
        while(q->head) {
            curr_node = dequeue(q);
            curr_smap_node = (Smap_tree_node *) curr_node->data;
            curr_smap_node->type = encode_coeff(curr_smap_node->coeff, threshold);
            DEBUG_DOUBLE("coeff", curr_smap_node->coeff);
            DEBUG_STR("type", smap_symbol_to_str(curr_smap_node->type));
            if(curr_smap_node->type == SP || curr_smap_node->type == SN) {
                is_zr = 0;
                break;
            }
            else {
                for(int i = 0; i < 4; i++) {
                    if(curr_smap_node->children[i]) {
                        DEBUG_DOUBLE("enqueing child", curr_smap_node->children[i]->coeff);
                        backtrack = push(backtrack, curr_smap_node->children[i]);
                        q = enqueue(q, curr_smap_node->children[i]);
                    }
                }
                DEBUG_STR("", "");
            }
        }

        // Trace back the tree and encode them accordingly
        // If zerotree is detected, then everything is a zerotree root
        // otherwise, it is an isolated zero
        while(backtrack->top) {
            curr_node = pop(backtrack);
            curr_smap_node = (Smap_tree_node *) curr_node->data;
            if(is_zr) {
                curr_smap_node->type = ZR;
            }
            else {
                if(curr_smap_node->type == U || curr_smap_node->type == ZR) {
                    curr_smap_node->type = IZ;
                }
            }
            DEBUG_DOUBLE("backtracking coeff", curr_smap_node->coeff);
            DEBUG_STR("type", smap_symbol_to_str(curr_smap_node->type));
            DEBUG_STR("", "");
        }
    }
}

// Valens EZW
Queue *dominant_pass(Smap_tree_node *smap_root, int threshold)
{
    Queue *q = NULL;
    Queue *dominant_list = NULL;
    Node *curr_queue_node = NULL;
    Smap_tree_node *curr_smap_node = NULL;

    // bitplane coding is adopted here
    // https://en.wikipedia.org/wiki/Bit_plane
    // bitplanes can be coded efficiently
    /* int threshold = pow(2, (int) floor((int) log2(max))); */

    // Manually add first 4 coeffs to the queue
    q = enqueue(q, smap_root);
    for(int i = 1; i < 4; i++) {
        q = enqueue(q, smap_root->children[i]);
    }

    while(q->head){
        curr_queue_node = dequeue(q);
        if(curr_queue_node) {
            curr_smap_node = (Smap_tree_node *) curr_queue_node->data;
            if(curr_smap_node->type == U) {
                // DEBUG_STR("U", "");
                q = enqueue(q, curr_smap_node);
                dominant_list = enqueue(dominant_list, curr_smap_node);
                check_descendants(curr_smap_node, threshold);
            }
        }
    }
    return dominant_list;
}

