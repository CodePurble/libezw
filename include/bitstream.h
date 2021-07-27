#ifndef BITSTREAM_H
#define BITSTREAM_H
#include "queue.h"

typedef struct {
    unsigned short threshold_pow;
    unsigned int num_bytes;
    unsigned char *bytes;
} mini_header;

// WARNING
// The __packed__ attribute will result in unsafe behaviour on non x86
// systems. Use this code with care.
// ref: https://stackoverflow.com/a/8568441
//
// Group 10 symbols together to save space
struct __attribute__((__packed__)) symbols_group {
    unsigned char s0 : 3;
    unsigned char s1 : 3;
    unsigned char s2 : 3;
    unsigned char s3 : 3;
    unsigned char s4 : 3;
    unsigned char s5 : 3;
    unsigned char s6 : 3;
    unsigned char s7 : 3;
    unsigned char s8 : 3;
    unsigned char s9 : 3;
};

struct symbol_set {
    unsigned short threshold;
    unsigned int size;
    struct symbols_group *symbols;
};

void symbol_set_init(struct symbol_set *s_set, unsigned short threshold, unsigned int size);
void symbol_group_insert(struct symbols_group *s_grp, int count, unsigned char symbol);

#endif
