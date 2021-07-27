#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "bitstream.h"

#define SYMB_MASK 0x07


mini_header *create_mini_header(unsigned int threshold, Queue *symbols)
{
    mini_header *m_hdr = malloc(sizeof(mini_header));
    m_hdr->num_bytes = (symbols->len / 2) + (symbols->len % 2);
    unsigned char curr_byte = 0xff;
    unsigned char *bytes = (unsigned char *) calloc(m_hdr->num_bytes, sizeof(unsigned char));
    Node *node_pair[2] = {NULL};
    m_hdr->threshold_pow = log2(threshold);
    unsigned int ind = 0;
    while(symbols->head) {
        curr_byte = 0xff;
        // Pack pairs of symbols into bytes
        for(int i = 0; i < 2; i++) {
            node_pair[i] = dequeue(symbols);
            if(node_pair[i]) {
                curr_byte = (curr_byte << 3) | *(unsigned char *) node_pair[i]->data;
            }
        }
        bytes[ind] = curr_byte;
        ind++;
    }
    m_hdr->bytes = bytes;
    DEBUG_INT("t", m_hdr->threshold_pow);
    DEBUG_INT("n", m_hdr->num_bytes);
    DEBUG_ARR_BYTE(m_hdr->bytes, m_hdr->num_bytes);
    return m_hdr;
}

void symbol_group_insert(struct symbols_group *s_grp, int count, unsigned char symbol)
{
    switch(count) {
        case 0:
            s_grp->s0 = symbol;
            break;
        case 1:
            s_grp->s1 = symbol;
            break;
        case 2:
            s_grp->s2 = symbol;
            break;
        case 3:
            s_grp->s3 = symbol;
            break;
        case 4:
            s_grp->s4 = symbol;
            break;
        case 5:
            s_grp->s5 = symbol;
            break;
        case 6:
            s_grp->s6 = symbol;
            break;
        case 7:
            s_grp->s7 = symbol;
            break;
        case 8:
            s_grp->s8 = symbol;
            break;
        case 9:
            s_grp->s9 = symbol;
            break;
        default: return;
    }
}
