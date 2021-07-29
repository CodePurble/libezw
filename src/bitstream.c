#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "bitstream.h"

#define SYMB_MASK 0x07


mini_header *create_mini_header(unsigned int threshold, Queue *symbols)
{
    mini_header *m_hdr = malloc(sizeof(mini_header));
    m_hdr->num_bytes = (unsigned int) (symbols->len / 2) + (symbols->len % 2);
    unsigned char curr_byte = 0xff;
    unsigned char *bytes = (unsigned char *) calloc(m_hdr->num_bytes, sizeof(unsigned char));
    Node *node_pair[2] = {NULL};
    m_hdr->threshold_pow = (unsigned short) log2(threshold);
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

void write_bitstream_file(const char* filename, enum file_op_mode mode,
        mini_header *m_hdr, unsigned int dim)
{
    FILE *fptr = NULL;
    unsigned char dim_pow = log2(dim);
    DEBUG_INT("num_bytes", m_hdr->num_bytes);
    if(mode == A) {
        fptr = fopen(filename, "ab");
    }
    else if(mode == W) {
        fptr = fopen(filename, "wb");
    }
    if(fwrite(&dim_pow, sizeof(dim_pow), 1, fptr) != 1) {
        fprintf(stderr, "Error while writing file: %s", filename);
        exit(1);
    }
    if(fwrite(&m_hdr->threshold_pow, sizeof(m_hdr->threshold_pow), 1, fptr) != 1) {
        fprintf(stderr, "Error while writing file: %s", filename);
        exit(1);
    }
    // if(fwrite(&m_hdr->num_bytes, sizeof(m_hdr->num_bytes), 1, fptr) != 1) {
    if(fwrite(&m_hdr->num_bytes, sizeof(unsigned int), 1, fptr) != 1) {
        fprintf(stderr, "Error while writing file: %s", filename);
        exit(1);
    }
    if(fwrite(m_hdr->bytes, sizeof(m_hdr->bytes[0]), m_hdr->num_bytes, fptr) != m_hdr->num_bytes) {
        fprintf(stderr, "Error while writing file: %s", filename);
        exit(1);
    }
    fclose(fptr);
}
