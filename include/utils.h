#ifndef UTILS_H
#define UTILS_H
#include<stdio.h>

void read_binary_file(FILE *infile, unsigned char *dest_arr, int rows, int cols);
void write_binary_file(FILE *outfile, unsigned char *src_arr, int rows, int cols);
void peek_binary_file();

#endif
