#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "wavelib.h"
#include "sbtree.h"
#include "smap.h"
#include "ezw.h"
#include "queue.h"
#include "bitstream.h"

// #define L

/* #define STACKTEST */

#if defined(STACKTEST)
#include "stack.h"
int main()
{
    Stack *st = NULL;
    Node *n = NULL;
    int arr[10];
    for(int i = 0; i < 10; i++) {
        arr[i] = i;
        st = push(st, &arr[i]);
    }
    stack_pretty_print(st);
    for(int i = 0; i < 10; i++) {
        n = pop(st);
        DEBUG_INT("top", *(int *) n->data);
    }
    stack_pretty_print(st);
    return 0;
}
#elif defined(QTEST)
#include "queue.h"
int main()
{
    Queue *q = NULL;
    Node *n = NULL;
    int arr[10];
    for(int i = 0; i < 10; i++) {
        arr[i] = i;
        q = enqueue(q, &arr[i]);
    }
    queue_pretty_print(q);
    for(int i = 0; i < 10; i++) {
        n = dequeue(q);
        DEBUG_INT("head", *(int *) n->data);
    }
    queue_pretty_print(q);
}
#elif defined(SBTEST)
int main()
{
    double LL3 = 57.0;
    double HL3 = -37.0;
    double LH3 = -29.0;
    double HH3 = 30.0;
    double LL2[4] = {
        57.0, -37.0,
        -29.0, 30.0
    };
    double HL2[4] = {
        39.0, -20.0,
        -17.0, 33.0
    };
    double LH2[4] = {
        14.0, 6.0,
        10.0, 19.0
    };
    double HH2[4]= {
        15.0, 13.0,
        -7.0, 9.0
    };
    double LL1[16] = {
        57.0, -37.0, 39.0, -20.0,
        -29.0, 30.0, 17.0, 33.0,
        14.0, 6.0, 15.0, 13.0,
        10.0, 19.0, -7.0, 9.0,
    };
    double HL1[16] = {
        3.0, 7.0, 9.0, 10.0,
        8.0, 2.0, 1.0, 6.0,
        9.0, -4.0, 2.0, 3.0,
        -7.0, 14.0, 12.0, -9.0,
    };
    double LH1[16] = {
        12.0, 15.0, 33.0, 20.0,
        0.0, 7.0, 2.0, 4.0,
        4.0, 1.0, 10.0, 3.0,
        5.0, 6.0, 0.0, 0.0,
    };
    double HH1[16] = {
        -2.0, 3.0, 1.0, 0.0,
        4.0, -1.0, 1.0, 1.0,
        2.0, 0.0, 1.0, 0.0,
        3.0, 1.0, 2.0, 1.0,
    };
    SBtree_node *root = sb_tree_init_root();

    // layer 1
    root = sb_tree_insert(root, LL, &LL3);
    root = sb_tree_insert(root, HL, &HL3);
    root = sb_tree_insert(root, LH, &LH3);
    root = sb_tree_insert(root, HH, &HH3);
    // layer 2
    root = sb_tree_insert(root, LL, &LL2[0]);
    root = sb_tree_insert(root, HL, &HL2[0]);
    root = sb_tree_insert(root, LH, &LH2[0]);
    root = sb_tree_insert(root, HH, &HH2[0]);
    // layer 3
    root = sb_tree_insert(root, LL, &LL1[0]);
    root = sb_tree_insert(root, HL, &HL1[0]);
    root = sb_tree_insert(root, LH, &LH1[0]);
    root = sb_tree_insert(root, HH, &HH1[0]);

    sb_tree_print_preorder(root);
    printf("\n");
    sb_tree_free(root);
    return 0;
}
#else
void printhelp(unsigned char exit) {
    FILE *f = stdout;
    if(exit != 0) {
        f = stderr;
    }
    fprintf(f, "USAGE:\n\tmain [options] imagefile\n\n");
    fprintf(f, "MANDATORY OPTIONS:\n\t-o output file\n\t-d image dimension\n\n");
    fprintf(f, "OPTIONAL OPTIONS:\n\t-i iterations (defaults to 3)\n\t-h show this help and exit\n\n");
    fprintf(f, "EXAMPLE:\n\tHere, assume img.png is a 16x16 image\n\t$ main -o compressed.ezw -d 16 img.png\n");
}

int main(int argc, char **argv)
{
    int opt, rows, cols;
    int o_flag = -1;
    const char *outputFile;
    const char *inputFile;
    char *end;
    int iter = 3;
    while((opt = getopt(argc, argv, "o:d:i::h")) != -1) {
        switch(opt) {
        case 'h':
            {
                printhelp(0);
                return 0;
                break;
            }
        case 'o':
            {
                outputFile = optarg;
                o_flag = strtol(optarg, &end, 10);
                break;
            }
        case 'd':
            {
                rows = strtol(optarg, &end, 10);
                cols = rows;
                break;
            }
        case 'i':
            {
                iter = strtol(optarg, &end, 10);
                break;
            }
        case '?':
            {
                if(optopt != 'o' && optopt != 'd') {
                    fprintf(stderr, "Unknown option -%c.\n\n", optopt);
                    printhelp(1);
                    return 1;
                }
                break;
            }
        }
    }

    if(o_flag == -1) {
        fprintf(stderr, "-o is a mandatory option\n\n");
        printhelp(1);
        return 1;
    }
    if(rows == -1) {
        fprintf(stderr, "-d is a mandatory option\n\n");
        printhelp(1);
        return 1;
    }

    if(optind < argc) {
        inputFile = argv[optind];
    }

    FILE *img_bin = fopen(inputFile, "rb");
    unsigned char *pix_arr = calloc(rows*cols, 1); // array of 8 bit vals

    if(!img_bin) {
        fprintf(stderr, "Unable to open file: %s\n", inputFile);
        return 1;
    }
    else {
        read_binary_file(img_bin, pix_arr, rows, cols);
        fclose(img_bin);
    }

    wave_object obj;
    wt2_object wt;
    double *inp, *oup, *inv_out;
    int N = rows*cols;

    char *name = "haar";
    obj = wave_init(name);// Initialize the wavelet
    inp = (double *) calloc(N, sizeof(double));
    oup = (double *) calloc(N, sizeof(double));
    inv_out = (double *) calloc(N, sizeof(double));

    // number of decompositions = max possible for the given dimensions
    int J = (int) log2(rows);

    wt = wt2_init(obj, "dwt", rows, cols, J);

    for (int i = 0; i < rows; ++i) {
        for (int k = 0; k < cols; ++k) {
            inp[i*cols + k] = (double) pix_arr[i*cols + k];
            oup[i*cols + k] = 0.0;
        }
    }

    SBtree_node *root = (SBtree_node *) malloc(sizeof(SBtree_node));
    Smap_tree_node *smap_root = (Smap_tree_node *) malloc(sizeof(Smap_tree_node));
    root = sb_treeify(J, inp, rows, cols);
    smap_root = smap_treeify(root, J);
    ezw(outputFile, smap_root, rows, cols, iter);

    Queue *header_q = NULL;
    header_q = read_bitstream_file(outputFile, header_q);
    // queue_pretty_print(header_q, MINI_HDR);
    // smap_tree_print_preorder(smap_root, ALL);

    // clean up
    sb_tree_free(root);
    wave_free(obj);
    wt2_free(wt);
    free(inp);
    free(oup);
    free(pix_arr);
    free(inv_out);
    return 0;
}
#endif // WAVE
