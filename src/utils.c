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
