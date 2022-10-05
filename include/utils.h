#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include "smap.h"

static inline void BOLD_CYAN_FG(char *str)
{
    printf("\033[1;38;2;51;177;255m%s\033[0m", str);
}

static inline void DEBUG_INT(char *str, int x)
{
    BOLD_CYAN_FG("[debug] ");
    printf("%s:%d\n", str, x);
}

static inline void DEBUG_DOUBLE(char *str, double x)
{
    BOLD_CYAN_FG("[debug] ");
    printf("%s:%f\n", str, x);
}

static inline void DEBUG_STR(char *str, char *x)
{
    BOLD_CYAN_FG("[debug] ");
    printf("%s:%s\n", str, x);
}

static inline void DEBUG_UINT_PAIR(char *str, unsigned int *x)
{
    BOLD_CYAN_FG("[debug_uint_pair] ");
    printf("%s:(%u %u)\n", str, x[0], x[1]);
}

static inline void DEBUG_ARR_BYTE(unsigned char *x, int len) {
    BOLD_CYAN_FG("[debug_arr_byte] ");
    for(int i = 0; i < len; i++) {
        printf("%x ", x[i]);
    }
    printf("\n");
}

static inline void DEBUG_ARR_F_1(double *x, int len) {
    BOLD_CYAN_FG("[debug_arr_f_1] ");
    for(int i = 0; i < len; i++) {
        printf("%f ", x[i]);
    }
    printf("\n");
}

static inline void DEBUG_ARR_F_2(double **x, int r, int c) {
    for(int i = 0; i < r; i++) {
        BOLD_CYAN_FG("[debug_arr_f_2] ");
        for(int j = 0; j < c; j++) {
            printf("%f ", x[i][j]);
        }
        printf("\n");
    }
}

static inline void DEBUG_ARR_INT(int *x, int r) {
    BOLD_CYAN_FG("[debug_arr_int] ");
    for(int i = 0; i < r; i++) {
        printf("%d ", x[i]);
    }
}

static inline void DEBUG_SMAP_NODE(Smap_tree_node *n) {
    if(n) {
        DEBUG_DOUBLE("coeff", n->coeff);
    }
    else {
        DEBUG_STR("coeff", "NULL");
    }
}

#define DEBUG_BLANK DEBUG_STR("", "")

void read_binary_file(FILE *infile, unsigned char *dest_arr, int rows, int cols);
void write_binary_file(FILE *outfile, unsigned char *src_arr, int rows, int cols);
void double_to_uchar(double *src, unsigned char* dest, int rows, int cols);
double max_double(double *arr, int len);
double** quads_from_arr(double *arr, int rows, int cols);
double* gsl_swap(double *unswapped, int rows, int cols);

double* smap2arr(Smap_tree_node* smap_root, int rows, int cols);
unsigned int* morton_decode(unsigned int morton, unsigned int *inds);

#endif // UTILS_H
