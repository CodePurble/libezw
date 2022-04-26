#include <stdlib.h>
#include <math.h>
#include "utils.h"

void read_binary_file(FILE *infile, unsigned char *dest_arr, int rows, int cols)
{
    if(fread(dest_arr, 1, rows*cols, infile) != rows*cols) {
        fprintf(stderr, "Error while reading binary file\n");
        exit(1);
    }
    else {
        printf("Read binary file...\n");
    }
}

void write_binary_file(FILE *outfile, unsigned char *src_arr, int rows, int cols)
{
    fwrite(src_arr, 1, rows*cols, outfile);
    printf("Wrote binary file...\n");
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

// NOTE: decompose row and col of current element from Morton scan (level order) output
// NOTE: by "un-interleaving" the bits of the Morton index
// https://en.wikipedia.org/wiki/Z-order_curve
unsigned int* morton_decode(unsigned int morton, unsigned int *inds)
{
    const unsigned int EVEN_MASK = 2;
    const unsigned int ODD_MASK = 1;
    unsigned int row_bit = 0;
    unsigned int col_bit = 0;
    inds[0] = inds[1] = 0;
    for(int i = 0; i < sizeof(morton)*4; i++) {
        row_bit = (morton & EVEN_MASK) >> 1;
        col_bit = morton & ODD_MASK;
        row_bit <<= i;
        col_bit <<= i;
        inds[0] |= row_bit;
        inds[1] |= col_bit;
        morton >>= 2;
    }
    return inds;
}

double* smap2arr(Smap_tree_node* smap_root, int rows, int cols)
{
    double *arr = (double *) calloc(rows*cols, sizeof(double));
    unsigned int *inds = calloc(2, sizeof(unsigned int));
    unsigned int i = 0;
    Queue *q = NULL;
    q = enqueue(q, smap_root);
    while(q->head) {
        Node *qnode = dequeue(q);
        Smap_tree_node *curr_smap = (Smap_tree_node *) qnode->data;
        for(int j = 0; j < 4; j++) {
            if(curr_smap->children[j]) {
                q = enqueue(q, curr_smap->children[j]);
            }
        }
        inds = morton_decode(i, inds);
        arr[inds[0]*cols + inds[1]] = curr_smap->coeff;
        i++;
    }
    free_queue(q);
    return arr;
}

double* gsl_swap(double *arr, int rows, int cols)
{
    int *level_collection = (int *) calloc(rows, sizeof(int));

    for(int i = 0; i < rows; i++) {
        level_collection[i] = 1 + (int) log2(rows/(i+1));
    }

    level_collection[0] = log2(rows/(1));
    level_collection[1] = level_collection[0];

    int ratio, swap_count;
    double temp;
    for(int i = 0; i < rows; i++) {
        ratio = (rows/pow(2, level_collection[i]));
        swap_count = ratio;
        for(int j = 0; j < cols; j++) {
            if(i == 0 || i == 1) {
                break;
            }
            else {
                if(swap_count) {
                    temp = arr[i*cols + j];
                    arr[i*cols + j] = arr[(i - ratio)*cols + (j + ratio)];
                    arr[(i - ratio)*cols + (j + ratio)] = temp;
                    swap_count--;
                }
            }
        }
    }
    free(level_collection);
    return arr;
}
