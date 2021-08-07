#ifndef BITSTREAM_H
#define BITSTREAM_H
#include <stdio.h>
#include "queue.h"

// 0b00111000
#define SYMB_MASK 0x38

typedef struct {
    unsigned short threshold_pow;
    unsigned int num_bytes;
    unsigned char *bytes;
    unsigned short *indices;
} mini_header;

enum file_op_mode {
    A, // append
    W // write
};

// OPTIMIZE: Maybe pass by reference? Soo many copies!!
// OPTIMIZE: Maybe pass by const reference when not mutating?

mini_header *create_mini_header(unsigned int threshold, Queue *symbol_pairs);
void write_bitstream_file(const char* filename, enum file_op_mode mode,
        mini_header *m_hdr, unsigned int dim);
Queue* read_bitstream_file(const char* filename, Queue *header_queue, unsigned char *dim_pow);

#endif
