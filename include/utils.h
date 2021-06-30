#ifndef UTILS_H
#define UTILS_H
#include<stdio.h>
#define DEBUG_INT(x, y) printf("%s:%d\n", x, y)
#define DEBUG_DOUBLE(x, y) printf("%s:%f\n", x, y)
#define DEBUG_STR(x, y) printf("%s:%s\n", x, y)

void read_binary_file(FILE *infile, unsigned char *dest_arr, int rows, int cols);
void write_binary_file(FILE *outfile, unsigned char *src_arr, int rows, int cols);
void double_to_uchar(double *src, unsigned char* dest, int rows, int cols);
double max_double(double *arr, int len);
double** quads_from_arr(double *arr, int rows, int cols);

#endif
