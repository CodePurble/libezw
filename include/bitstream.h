#ifndef BITSTREAM_H
#define BITSTREAM_H
#include <stdio.h>
#include "queue.h"

typedef struct {
    unsigned short threshold_pow;
    unsigned int num_bytes;
    unsigned char *bytes;
} mini_header;

enum file_op_mode {
    A, // append
    W // write
};

mini_header *create_mini_header(unsigned int threshold, Queue *symbols);
void write_bitstream_file(const char* filename, enum file_op_mode mode,
        mini_header *m_hdr, unsigned int dim);

#endif
