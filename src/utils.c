#include "utils.h"

void read_binary_file(FILE *infile, unsigned char* dest, int rows, int cols)
{
    fread(dest, 1, rows*cols, infile);
}

void write_binary_file(FILE *outfile, unsigned char *src, int rows, int cols)
{
    fwrite(src, 1, rows*cols, outfile);
}
