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
                    curr_smap_node->symbol = curr_smap_node->type;
                    dominant_list = enqueue(dominant_list, curr_smap_node);
                    break;
                case ZR:
                    {
                        if((curr_smap_node->parent
                                    && curr_smap_node->parent->type != ZR)
                                || curr_smap_node->isroot) {
                                curr_smap_node->symbol = curr_smap_node->type;
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
    Queue *bitstream_pairs = NULL;
    if(dominant_list) {
        Node *curr_queue_node = dominant_list->head;
        Smap_tree_node *curr_smap_node = NULL;
        while(curr_queue_node) {
            // need to define new variables for each iteration
            Symbol_ind_pair *pair = (Symbol_ind_pair *) malloc(sizeof(Symbol_ind_pair));
            curr_smap_node = (Smap_tree_node *) curr_queue_node->data;
            if(curr_smap_node->type == SP || curr_smap_node->type == SN) {
                curr_smap_node->not_available = 1;
                // add the third bit to the symbol
                if((fabs(curr_smap_node->coeff) >= 1.5*threshold)) {//&&
                        // (fabs(curr_smap_node->coeff) < 2*threshold)) {
                    pair->symbol = curr_smap_node->type << 1 | 0x1;
                }
                else {
                    pair->symbol = curr_smap_node->type << 1;
                }
                curr_smap_node->coeff = 0.0;
            }
            else {
                pair->symbol = curr_smap_node->type << 1;
            }
            pair->morton_index = curr_smap_node->morton_index;
            curr_smap_node->symbol = pair->symbol;
            bitstream_pairs = enqueue(bitstream_pairs, pair);
            curr_queue_node = curr_queue_node->next;
        }
    }
    return bitstream_pairs;
}

void ezw(const char *filename, Smap_tree_node *smap_root, int rows, int cols, unsigned char iter)
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
            // BOLD_CYAN_FG("Iteration\n");
            dominant_list = dominant_pass(smap_root, threshold);
            // queue_pretty_print(dominant_list, SMAP_TREE_NODE);

            symbols = subordinate_pass(dominant_list, threshold);
            // queue_pretty_print(symbols, INT);

            m_hdr = create_mini_header(threshold, symbols);
            if(i == 0) {
                write_bitstream_file(filename, W, m_hdr, rows);
            }
            else {
                write_bitstream_file(filename, A, m_hdr, rows);
            }
            threshold /= 2;
            // BOLD_CYAN_FG("...\n\n");
            free_queue(symbols);
            free(m_hdr);
        }
        else {
            break;
        }
        i++;
    }
}

Smap_tree_node* reconstruct(unsigned char dim_pow, Queue *header_q)
{
    // initialise empty trees for filling in with approximations
    double *zeros = (double *) calloc(pow(2, dim_pow), sizeof(double));
    SBtree_node *reconst_root = sb_treeify(dim_pow, zeros, pow(2, dim_pow), pow(2, dim_pow));
    Smap_tree_node *smap_root = smap_treeify(reconst_root, dim_pow);

    Smap_tree_node *curr_smap = NULL;
    Queue *smap_q = NULL;
    int curr_threshold = 0;
    unsigned char lsb = 0;
    if(header_q) {
        while(header_q->head) {
            Node *curr_hdr_node = dequeue(header_q);
            mini_header *curr_hdr = (mini_header *) curr_hdr_node->data;
            unsigned char curr_byte = 0;
            unsigned char *symbols = calloc(2*curr_hdr->num_bytes, sizeof(unsigned char));
            for(int i = 0; i < 2*curr_hdr->num_bytes; i++) {
                curr_byte = curr_hdr->bytes[i/2];

                // NOTE: extract from MSB half first
                // eg: 0b11111001
                // extracts 0b111 first, then 0b001, which is the order we want
                symbols[i] = (((curr_byte << (i%2)*3) & SYMB_MASK) >> 3);
            }

            smap_q = enqueue(smap_q, smap_root);
            curr_threshold = pow(2, curr_hdr->threshold_pow);
            for(int i = 0; i < 2*curr_hdr->num_bytes; i++) {
                Node *curr_smap_node = dequeue(smap_q);
                curr_smap = (Smap_tree_node *) curr_smap_node->data;
                for(int i = 0; i < 4; i++) {
                    if(curr_smap->children[i]) {
                        smap_q = enqueue(smap_q, curr_smap->children[i]);
                    }
                }
                if(!curr_smap->not_available) {
                    lsb = symbols[i] & 1;
                    symbols[i] >>= 1;
                    switch(symbols[i]) {
                        case SP:
                            {
                                if(lsb) {
                                    curr_smap->coeff = (1.5*curr_threshold) + (curr_threshold/4.0);
                                }
                                else {
                                    curr_smap->coeff = curr_threshold;
                                }
                                curr_smap->not_available = 1;
                                break;
                            }
                        case SN:
                            {
                                if(lsb) {
                                    curr_smap->coeff = (-1.5*curr_threshold) + (curr_threshold/4.0);
                                }
                                else {
                                    curr_smap->coeff = -curr_threshold;
                                }
                                curr_smap->not_available = 1;
                                break;
                            }
                        default:
                            {
                                curr_smap->coeff = 0.0;
                                // curr_smap->not_available = 1;
                                break;
                            }
                    }
                }
            }
            // DEBUG_ARR_BYTE(symbols, 2*curr_hdr->num_bytes);
            // smap_tree_print_preorder(smap_root, ALL);
            // printf("\n");
            free(symbols);
            free(smap_q);
            smap_q = NULL;
        }
    }
    free(zeros);
    sb_tree_free(reconst_root);
    return smap_root;
}
