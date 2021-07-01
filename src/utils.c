#include <stdlib.h>
#include "utils.h"

void read_binary_file(FILE *infile, unsigned char *dest_arr, int rows, int cols)
{
    printf("Reading binary file...\n");
    fread(dest_arr, 1, rows*cols, infile);
}

void write_binary_file(FILE *outfile, unsigned char *src_arr, int rows, int cols)
{
    printf("Writing binary file...\n");
    fwrite(src_arr, 1, rows*cols, outfile);
}

void double_to_uchar(double *src, unsigned char* dest, int rows, int cols)
{
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            dest[i*cols + j] = (unsigned char) src[i*cols + j];
        }
    }
}

double max_double(double *arr, int len)
{
    double max = 0.0;
    for(int i = 0; i < len; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

double** quads_from_arr(double *arr, int rows, int cols)
{
    int quadcount = (rows*cols)/4;
    double **quads = (double **) calloc(sizeof(double *), quadcount);

    int count = 0;
    int i = 0;
    while(count < quadcount) {
        double *temp = (double *) calloc(sizeof(double), 4);
        temp[0] = arr[i];
        temp[1] = arr[i+1];
        temp[2] = arr[i+rows];
        temp[3] = arr[i+rows+1];
        quads[count] = temp;
        if((count + 1) % (rows/2) == 0 && count != 0) {
            i += (rows+2);
        }
        else {
            i += 2;
        }
        count++;
    }
    return quads;
}
