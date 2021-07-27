#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wavelib.h"
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
            // DEBUG_DOUBLE("coeff", curr_smap_node->coeff);
            // DEBUG_STR("type", smap_symbol_to_str(curr_smap_node->type));
            if(curr_smap_node->type == SP || curr_smap_node->type == SN) {
                is_zr = 0;
                break;
            }
            else {
                for(int i = 0; i < 4; i++) {
                    if(curr_smap_node->children[i]) {
                        backtrack = push(backtrack, curr_smap_node->children[i]);
                        q = enqueue(q, curr_smap_node->children[i]);
                    }
                }
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
            // DEBUG_DOUBLE("backtracking coeff", curr_smap_node->coeff);
            // DEBUG_STR("type", smap_symbol_to_str(curr_smap_node->type));
            // DEBUG_STR("", "");
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

    // Manually add first one
    q = enqueue(q, smap_root);

    while(q->head){
        curr_queue_node = dequeue(q);
        if(curr_queue_node) {
            curr_smap_node = (Smap_tree_node *) curr_queue_node->data;
            for(int i = 0; i < 4; i++) {
                if(curr_smap_node->children[i]) {
                    q = enqueue(q, curr_smap_node->children[i]);
                }
            }
            check_descendants(curr_smap_node, threshold);
            switch(curr_smap_node->type) {
                case SP:
                case SN:
                case IZ:
                    dominant_list = enqueue(dominant_list, curr_smap_node);
                    break;
                case ZR:
                    {
                        if((curr_smap_node->parent
                                    && curr_smap_node->parent->type != ZR)
                                || curr_smap_node->isroot) {
                                dominant_list = enqueue(dominant_list, curr_smap_node);
                        }
                    }
                case U:
                    break;
            }
        }
    }
    return dominant_list;
}

Queue *subordinate_pass(Queue *dominant_list, int threshold)
{
    Queue *bitstream_elements = NULL;
    if(dominant_list) {
        Node *curr_queue_node = dominant_list->head;
        Smap_tree_node *curr_smap_node = NULL;
        while(curr_queue_node) {
            unsigned char *symbol = (unsigned char *) malloc(sizeof(unsigned char));
            curr_smap_node = (Smap_tree_node *) curr_queue_node->data;
            if(curr_smap_node->type == SP || curr_smap_node->type == SN) {
                curr_smap_node->not_available = 1;
                // add the third bit to the symbol
                if((fabs(curr_smap_node->coeff) >= 1.5*threshold) &&
                        (fabs(curr_smap_node->coeff) < 2*threshold)) {
                    DEBUG_STR("spsn", "1");
                    *symbol = curr_smap_node->type << 1 | 0x1;
                }
                else {
                    DEBUG_STR("spsn", "0");
                    *symbol = curr_smap_node->type << 1;
                }
                curr_smap_node->coeff = 0.0;
            }
            else {
                DEBUG_STR("non", "0");
                *symbol = curr_smap_node->type << 1;
            }
            bitstream_elements = enqueue(bitstream_elements, symbol);
            curr_queue_node = curr_queue_node->next;
        }
    }
    return bitstream_elements;
}

void ezw(Smap_tree_node *smap_root, unsigned char iter)
{
    // bitplane coding is adopted here
    // https://en.wikipedia.org/wiki/Bit_plane
    // bitplanes can be coded efficiently
    unsigned int threshold = pow(2, (int) floor((int) log2(smap_root->coeff)));

    Queue *dominant_list = NULL;
    Queue *symbols = NULL;
    mini_header *m_hdr = NULL;

    int i = 0;
    while(i < iter) {
        if(threshold > 0) {
            BOLD_CYAN_FG("Iteration\n");
            dominant_list = dominant_pass(smap_root, threshold);
            queue_pretty_print(dominant_list, SMAP_TREE_NODE);
            symbols = subordinate_pass(dominant_list, threshold);
            queue_pretty_print(symbols, INT);
            m_hdr = create_mini_header(threshold, symbols);
            write_bitstream_file("foo.bin", W, m_hdr, 8);
            exit(0);
            threshold /= 2;
            BOLD_CYAN_FG("...\n\n");
        }
        else {
            break;
        }
        i++;
    }
}

