#ifndef UTILS_H
#define UTILS_H
#include<stdio.h>

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

#define DEBUG_BLANK DEBUG_STR("", "")

void read_binary_file(FILE *infile, unsigned char *dest_arr, int rows, int cols);
void write_binary_file(FILE *outfile, unsigned char *src_arr, int rows, int cols);
void double_to_uchar(double *src, unsigned char* dest, int rows, int cols);
double max_double(double *arr, int len);
double** quads_from_arr(double *arr, int rows, int cols);

#endif // UTILS_H
