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
