#include "utils.h"

void read_binary_file(FILE *in, unsigned char* dest, int rows, int cols)
{
    fread(dest, 1, rows*cols, in);
}
