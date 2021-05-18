#ifndef UTILS_H
#define UTILS_H
#include<stdio.h>

void read_binary_file(FILE *in, unsigned char *dest, int rows, int cols);
void write_binary_file(FILE *out, unsigned char *dest, int rows, int cols);
void peek_binary_file();

#endif
