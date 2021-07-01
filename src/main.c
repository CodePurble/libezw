#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "utils.h"
#include "wavelib.h"
#include "sbtree.h"
#include "smap.h"

#define ROWS 8
#define COLS 8
#define L 8

#define QTEST

#ifdef QTEST
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
int main(int argc, char **argv)
{
    bool return_error = false;

    if(argc == 1) {
        fprintf(stderr, "Error: Requires at least one argument\n");
        return_error = true;
    }
    else if(argc == 2) {
        unsigned char *pix_arr = calloc(ROWS*COLS, 1); // array of 8 bit vals
        FILE *img_bin = fopen(argv[1], "rb");
        if(!img_bin) {
            fprintf(stderr, "Unable to open file: %s\n", argv[1]);
            return_error = true;
        }
        else {
            read_binary_file(img_bin, pix_arr, ROWS, COLS);
            fclose(img_bin);
            free(pix_arr);
        }
    }
    else if(argc == 3) {
        FILE *img_bin = fopen(argv[1], "rb");
        FILE *coeff_bin = fopen(argv[2], "wb");
        unsigned char *pix_arr = calloc(ROWS*COLS, 1); // array of 8 bit vals

        if(!img_bin) {
            fprintf(stderr, "Unable to open file: %s\n", argv[1]);
            return_error = true;
        }
        else {
            read_binary_file(img_bin, pix_arr, ROWS, COLS);
            fclose(img_bin);
        }

        if(!coeff_bin) {
            fprintf(stderr, "Unable to open file: %s\n", argv[2]);
            return_error = true;
        }
        else {
            wave_object obj;
            wt2_object wt;
            double *inp, *oup, *inv_out;
            int N = ROWS*COLS;

            char *name = "haar";
            obj = wave_init(name);// Initialize the wavelet
            inp = (double *) calloc(N, sizeof(double));
            oup = (double *) calloc(N, sizeof(double));
            inv_out = (double *) calloc(N, sizeof(double));

            // number of decompositions = max possible for the given dimensions
            int J = (int) log2(ROWS);

            wt = wt2_init(obj, "dwt", ROWS, COLS, J);

            for (int i = 0; i < ROWS; ++i) {
                for (int k = 0; k < COLS; ++k) {
                    inp[i*COLS + k] = (double) pix_arr[i*COLS + k];
                    oup[i*COLS + k] = 0.0;
                }
            }
            SBtree_node *root = (SBtree_node *) malloc(sizeof(SBtree_node));
            Smap_tree_node *smap_root = (Smap_tree_node *) malloc(sizeof(Smap_tree_node));
            root = sb_treeify(J, inp, ROWS, COLS);
            smap_root = smap_treeify(root, J, ROWS, COLS);
            smap_tree_print_preorder(smap_root);

            // clean up
            sb_tree_free(root);
            wave_free(obj);
            wt2_free(wt);
            free(inp);
            free(oup);
            free(pix_arr);
            free(inv_out);
            fclose(coeff_bin);
        }
    }

    if(return_error) {
        return 1;
    }
    else {
        return 0;
    }
}
#endif // WAVE
