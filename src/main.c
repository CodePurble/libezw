#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_wavelet2d.h>
#include "utils.h"
#include "wavelib.h"
#include "sbtree.h"
#include "smap.h"
#include "ezw.h"
#include "queue.h"
#include "bitstream.h"

#if defined(GSLTEST)
int main()
{
    const int dim = 16;
    // double arr[64] = {
    //     1447.38,-136.375,-173,-329,-164,-132.5,-21.5,-189,
    //     90.875,-16.875,24.75,-35,13.5,-54,-37.5,21,
    //     65,-138.5,107.5,-108,-3,35.5,-1.5,-23.5,
    //     34.75,5,-101.25,-7,3.5,3.5,2.5,5.5,
    //     41,122.5,-21.5,-66,41,-81.5,21.5,-13,
    //     -58.5,35,-37.5,-17,27.5,37,-51.5,17,
    //     -65,34.5,-8.5,18.5,-32,-28.5,31.5,-15.5,
    //     31.5,-5.5,6.5,8.5,-3.5,11.5,2.5,-10.5
    // };
    double arr[256] = {
        3248.88,-154.875,54.125,-136.375,91,88.25,-173,-329,-3,30.5,67,47.5,-164,-132.5,-21.5,-189,
        129.125,79.125,-2.125,-103.875,-22.75,-43.25,25.75,-35,33.5,-25,-15.5,28.5,13.5,-54,-37.5,21,
        57.625,90.875,19.625,-16.875,32.75,-12.5,-45,-63.5,-16.5,16.5,-10,34.5,-3,35.5,-1.5,-23.5,
        162.125,26.125,22.125,-46.625,-54.75,-39.75,-22.5,110.75,39,-49.5,-71.5,-39.5,3.5,3.5,2.5,5.5,
        75,-1.75,65,-138.5,-45.5,-17.75,107.5,-108,-27.5,4.5,71.5,-4,33,28,8,1,
        68.75,-31.25,34.5,5,-10.75,84.75,-101.25,-7,0,22.5,-8.5,0,405,-41.5,-43.5,-15.5,
        37.25,7,-3.5,-41,-63.25,22.5,114.5,82,0,-1.5,2,27.5,-37,17,-20.5,0,
        81.75,-11.25,3.5,58.25,60.75,44.75,-57.5,-107.75,-4,-27,-67,-57,3,-5,35.5,137.5,
        3,21.5,0,-15.5,41,122.5,-21.5,-66,3,-23.5,1,15.5,41,-81.5,21.5,-13,
        48.5,-20,-1.5,17.5,-58.5,35,-37.5,-17,-33.5,28,-3.5,0.5,27.5,37,-51.5,17,
        70.5,-19.5,4,29.5,-65,34.5,-8.5,18.5,4.5,19.5,-4,3.5,-32,-28.5,31.5,-15.5,
        1,30.5,-26.5,-25.5,31.5,-5.5,6.5,8.5,9,-4.5,19.5,-25.5,-3.5,11.5,2.5,-10.5,
        -17.5,4.5,27.5,6,-31,-8,5,7,-20.5,-4.5,16.5,-3,42,-37,-3,47,
        0,68.5,-15.5,-12,-39.5,39.5,6.5,-14.5,0,5.5,-20.5,0,-1.5,39.5,-10.5,-18.5,
        0,65.5,-1,23.5,-66,32,-39.5,0,0,-19.5,12,-16.5,-17,-17,-6.5,4,
        -2,27,-1,-27,2,67,-55.5,117.5,0,27,-12,-18,1,5,34.5,-32.5,
    };

    double *array = (double *) calloc(dim*dim, sizeof(double));
    array = gsl_swap(arr, dim, dim);

    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            printf("%0.2f\t", array[i*dim + j]);
        }
        printf("\n");
    }
    return 0;

}
#elif defined(MORTONTEST)
int main()
{
    unsigned int *inds = (unsigned int *) calloc(2, sizeof(unsigned int));
    for(int i = 0; i < 4; i++) {
        inds = morton_decode(i, inds);
        DEBUG_UINT_PAIR("inds", inds);
    }
    return 0;
}
#elif defined(STACKTEST)
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
void printhelp(unsigned char exit)
{
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
        case 'h': {
            printhelp(0);
            return 0;
            break;
        }
        case 'o': {
            outputFile = optarg;
            o_flag = strtol(optarg, &end, 10);
            break;
        }
        case 'd': {
            rows = strtol(optarg, &end, 10);
            cols = rows;
            break;
        }
        case 'i': {
            iter = strtol(optarg, &end, 10);
            break;
        }
        case '?': {
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
    Smap_tree_node *smap_root_copy = (Smap_tree_node *) malloc(sizeof(Smap_tree_node));
    Smap_tree_node *smap_root_approx = (Smap_tree_node *) malloc(sizeof(Smap_tree_node));
    root = sb_treeify(J, inp, rows, cols);
    smap_root = smap_treeify(root, J);
    smap_root_copy = smap_treeify(root, J);
    ezw(outputFile, smap_root_copy, rows, cols, iter);

    Queue *header_q = NULL;
    unsigned char dim_pow;
    header_q = read_bitstream_file(outputFile, header_q, &dim_pow);
    // queue_pretty_print(header_q, MINI_HDR);
    smap_root_approx = reconstruct(dim_pow, header_q);
    double *arr = smap2arr(smap_root_approx, rows, cols);
    arr = gsl_swap(arr, rows, cols);

    unsigned char *arru = calloc(rows*cols, sizeof(unsigned char));
    gsl_wavelet *gsl_obj;
    gsl_wavelet_workspace *ws;

    gsl_obj = gsl_wavelet_alloc(gsl_wavelet_haar, 2);
    ws = gsl_wavelet_workspace_alloc(rows*cols);
    int ret = gsl_wavelet2d_nstransform_inverse(gsl_obj, arr, rows, rows, cols, ws);
    if(ret == GSL_SUCCESS) {
        for (int i = 0; i < rows; ++i) {
            for (int k = 0; k < cols; ++k) {
                arru[i*cols + k] = (unsigned char) arr[i*cols + k];
            }
        }
        const char *fname = "lichten_approx.bin";
        FILE *f = fopen(fname, "wb");
        size_t n = fwrite(arru, 1, rows*cols, f);
        printf("%d\n", (int) n);
        fclose(f);
    }
    else {
        printf("error\n");
    }

    // clean up
    free(arru);
    gsl_wavelet_free(gsl_obj);
    gsl_wavelet_workspace_free(ws);
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
