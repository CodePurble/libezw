#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wavelib.h"
#include "utils.h"
#include "ezw.h"
#include "queue.h"
#include "stack.h"
#include "bitstream.h"

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
            if(!curr_smap_node->not_available) {
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
    Queue *subordinate_list = NULL;
    Node *curr_queue_node = NULL;
    Smap_tree_node *curr_smap_node = NULL;

    // Manually add first one
    q = enqueue(q, smap_root);

    while(q->head){
        curr_queue_node = dequeue(q);
        if(curr_queue_node) {
            curr_smap_node = (Smap_tree_node *) curr_queue_node->data;
            for(int i = 0; i < 4; i++) {
                if(curr_smap_node->children[i] && !curr_smap_node->children[i]->not_available) {
                    q = enqueue(q, curr_smap_node->children[i]);
                }
            }

            check_descendants(curr_smap_node, threshold);
            switch(curr_smap_node->type) {
                case SP:
                case SN:
                    subordinate_list = enqueue(subordinate_list, curr_smap_node);
                    curr_smap_node->not_available = 1;
                    // curr_smap_node->coeff = 0.0;
                    break;
                case IZ:
                case ZR:
                    // {
                    //     if((curr_smap_node->parent
                    //                 && curr_smap_node->parent->type != ZR)
                    //             || curr_smap_node->isroot) {
                    //             curr_smap_node->symbol = curr_smap_node->type;
                    //             dominant_list = enqueue(dominant_list, curr_smap_node);
                    //     }
                    // }
                case U:
                    break;
            }
        }
    }
    return subordinate_list;
}

Queue *subordinate_pass(Queue *subordinate_list, int threshold)
{
    queue_pretty_print(subordinate_list, SMAP_TREE_NODE);
    printf("\n");
    Queue *bitstream_pairs = NULL;
    if(subordinate_list) {
        Node *curr_queue_node = subordinate_list->head;
        Smap_tree_node *curr_smap_node = NULL;
        while(curr_queue_node) {
            // need to define new variables for each iteration
            Symbol_ind_pair *pair = (Symbol_ind_pair *) malloc(sizeof(Symbol_ind_pair));
            curr_smap_node = (Smap_tree_node *) curr_queue_node->data;
            // add the third bit to the symbol
            // If 1, the coeff is > 1.5*t else, coeff is <= 1.5*t
            if(curr_smap_node->type == SP) {
                pair->symbol = 0;
            }
            else{
                pair->symbol = 1;
            }
            if(fabs(curr_smap_node->coeff) > 1.5*threshold) {
                pair->symbol = (pair->symbol << 1) | 1;
            }
            else {
                pair->symbol <<= 1;
            }
            pair->morton_index = curr_smap_node->morton_index;
            bitstream_pairs = enqueue(bitstream_pairs, pair);
            curr_queue_node = curr_queue_node->next;
        }
    }
    return bitstream_pairs;
}

void ezw(const char *filename, Smap_tree_node *smap_root, int rows, int cols, unsigned int iter)
{
    // bitplane coding is adopted here
    // https://en.wikipedia.org/wiki/Bit_plane
    // bitplanes can be coded efficiently
    unsigned int threshold = pow(2, (int) floor((int) log2(smap_root->coeff)));

    Queue *dominant_list = NULL;
    Queue *symbol_pairs = NULL;
    mini_header *m_hdr = NULL;
    int i = 0;
    while(i < iter) {
        if(threshold > 0) {
            dominant_list = dominant_pass(smap_root, threshold);

            symbol_pairs = subordinate_pass(dominant_list, threshold);

            m_hdr = create_mini_header(threshold, symbol_pairs);
            if(i == 0) {
                write_bitstream_file(filename, W, m_hdr, rows);
            }
            else {
                write_bitstream_file(filename, A, m_hdr, rows);
            }
            threshold /= 2;
            free_queue(symbol_pairs);
            free(m_hdr);
        }
        else {
            break;
        }
        i++;
    }
}

double* reconstruct(unsigned char dim_pow, Queue *header_q)
{
    // initialise empty trees for filling in with approximations
    double *approximation = (double *) calloc(pow(dim_pow, 2)*pow(dim_pow, 2), sizeof(double));

    int curr_threshold = 0;
    unsigned char lsb = 0;
    if(header_q) {
        while(header_q->head) {
            Node *curr_hdr_node = dequeue(header_q);
            mini_header *curr_hdr = (mini_header *) curr_hdr_node->data;
            unsigned char curr_byte = 0;
            unsigned char *symbols = calloc(2*curr_hdr->num_bytes, sizeof(unsigned char));
            unsigned short *indices = calloc(2*curr_hdr->num_bytes, sizeof(unsigned short));
            // for every index
            for(int i = 0; i < 4*curr_hdr->num_bytes; i++) {
                unsigned int shift = (i%4)*2;
                unsigned char mask = 0xc0;
                curr_byte = curr_hdr->bytes[i/4];
                symbols[i] = ((curr_byte & (mask >> shift)) >> (6 - (i%4)*2));
                indices[i] = curr_hdr->indices[i];
                // NOTE:
                // 11000000 0
                // 00110000 2
                // 00001100 4
                // 00000011 6
            }

            curr_threshold = pow(2, curr_hdr->threshold_pow);
            for(int i = 0; i < 2*curr_hdr->num_bytes; i++) {
                lsb = symbols[i] & 1;
                if(lsb) {
                    approximation[indices[i]] = (1.5*curr_threshold) + (curr_threshold/4.0);
                }
                else {
                    approximation[indices[i]] = (1.5*curr_threshold) - (curr_threshold/4.0);
                }

                symbols[i] >>= 1;

                if(symbols[i] == 1) {
                    approximation[indices[i]] *= -1;
                }
            }
            // DEBUG_ARR_BYTE(symbols, 2*curr_hdr->num_bytes);
            // smap_tree_print_preorder(smap_root, ALL);
            // printf("\n");
            free(symbols);
            free(indices);
        }
    }
    return approximation;
}
