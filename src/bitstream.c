#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "bitstream.h"
#include "ezw.h"

mini_header *create_mini_header(unsigned int threshold, Queue *symbol_pairs)
{
    mini_header *m_hdr = NULL;
    if(symbol_pairs) {
        m_hdr = malloc(sizeof(mini_header));
        m_hdr->num_bytes = (unsigned int) (symbol_pairs->len / 4) + (symbol_pairs->len % 4);
        DEBUG_INT("num_bytes", m_hdr->num_bytes);
        // NOTE: the number of indices is 4 times the number of bytes

        unsigned char curr_byte = 0x00;
        unsigned char *bytes = (unsigned char *) calloc(m_hdr->num_bytes, sizeof(unsigned char));
        unsigned short *indices = (unsigned short *) calloc(m_hdr->num_bytes*4, sizeof(unsigned short));

        Node *curr_node = NULL;
        Symbol_ind_pair *curr_pair = NULL;

        m_hdr->threshold_pow = (unsigned short) log2(threshold);
        unsigned int symb_ind = 0;
        unsigned int index_ind = 0;
        while(symbol_pairs->head) {
            curr_byte = 0x00;
            // Pack approx bits into bytes
            // queue_pretty_print(symbol_pairs, SYMB_PAIR);
            curr_node = dequeue(symbol_pairs);
            if(curr_node) {
                for(int i = 0; i < 4; i++) {
                    curr_pair = (Symbol_ind_pair *) curr_node->data;
                    if(curr_pair) {
                        curr_byte = (curr_byte << 2) | curr_pair->symbol;
                        indices[index_ind] = curr_pair->morton_index;
                        index_ind++;
                    }
                }
                bytes[symb_ind] = curr_byte;
                symb_ind++;
            }
        }
        m_hdr->bytes = bytes;
        m_hdr->indices = indices;
    }
    return m_hdr;
}

void write_bitstream_file(const char* filename, enum file_op_mode mode,
        mini_header *m_hdr, unsigned int dim)
{
    if(m_hdr) {
        FILE *fptr = NULL;
        unsigned char dim_pow = log2(dim);
        if(mode == A) {
            fptr = fopen(filename, "ab");
        }
        else if(mode == W) {
            fptr = fopen(filename, "wb");
            if(fwrite(&dim_pow, sizeof(dim_pow), 1, fptr) != 1) {
                fprintf(stderr, "Error while writing file: %s", filename);
                exit(1);
            }
        }
        if(fwrite(&m_hdr->threshold_pow, sizeof(m_hdr->threshold_pow), 1, fptr) != 1) {
            fprintf(stderr, "Error while writing file: %s", filename);
            exit(1);
        }
        if(fwrite(&m_hdr->num_bytes, sizeof(unsigned int), 1, fptr) != 1) {
            fprintf(stderr, "Error while writing file: %s", filename);
            exit(1);
        }
        if(fwrite(m_hdr->bytes, sizeof(m_hdr->bytes[0]), m_hdr->num_bytes, fptr) != m_hdr->num_bytes) {
            fprintf(stderr, "Error while writing file: %s", filename);
            exit(1);
        }
        if(fwrite(m_hdr->indices, sizeof(m_hdr->indices[0]), m_hdr->num_bytes*2, fptr) != m_hdr->num_bytes*2) {
            fprintf(stderr, "Error while writing file: %s", filename);
            exit(1);
        }
        fclose(fptr);
    }
}

Queue* read_bitstream_file(const char* filename, Queue *header_queue, unsigned char *dim_pow)
{
    FILE *fptr = fopen(filename, "rb");

    fread(dim_pow, sizeof(unsigned char), 1, fptr);

    unsigned short threshold_pow;
    while(fread(&threshold_pow, sizeof(unsigned short), 1, fptr) != 0) {
        mini_header *curr_hdr = (mini_header *) malloc(sizeof(mini_header));
        curr_hdr->threshold_pow = threshold_pow;
        fread(&curr_hdr->num_bytes, sizeof(unsigned int), 1, fptr);
        curr_hdr->bytes = calloc(curr_hdr->num_bytes, sizeof(unsigned int));
        fread(curr_hdr->bytes, sizeof(unsigned char), curr_hdr->num_bytes, fptr);
        curr_hdr->indices = calloc(curr_hdr->num_bytes*2, sizeof(unsigned short));
        fread(curr_hdr->indices, sizeof(unsigned short), curr_hdr->num_bytes*2, fptr);
        header_queue = enqueue(header_queue, curr_hdr);
    }
    return header_queue;
}
