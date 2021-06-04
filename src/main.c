#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "wavelib.h"
#include "sbtree.h"
#include "ezw.h"

#define ROWS 16
#define COLS 16
#define L 8

/* int main() */
/* { */
/*     double arr[L]; */
/*     for(int i = 0; i < L; i++) { */
/*         arr[i] = (double) i + 1; */
/*     } */
/*     SBtree_node *root = sb_tree_init_root(); */
/*     // Layer 1 */
/*     root = sb_tree_insert(root, LL, &arr[0]); */
/*     root = sb_tree_insert(root, HL, &arr[1]); */
/*     root = sb_tree_insert(root, LH, &arr[2]); */
/*     root = sb_tree_insert(root, HH, &arr[3]); */
/*     // Layer 2 */
/*     root = sb_tree_insert(root, LL, &arr[4]); */
/*     root = sb_tree_insert(root, HL, &arr[5]); */
/*     root = sb_tree_insert(root, LH, &arr[6]); */
/*     root = sb_tree_insert(root, HH, &arr[7]); */
/*     sb_tree_print_preorder(root); */
/*     printf("\n"); */
/*     double *res = sb_tree_get_coeff(root, HL, 10); */
/*     if(res) { */
/*         printf("Found %f\n", res[0]); */
/*     } */
/*     else { */
/*         printf("Not found\n"); */
/*     } */
/*     sb_tree_free(root); */
/*     return 0; */
/* } */

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

            int J = 3; // number of decompositions

            wt = wt2_init(obj, "dwt", ROWS, COLS, J);

            for (int i = 0; i < ROWS; ++i) {
                for (int k = 0; k < COLS; ++k) {
                    inp[i*COLS + k] = (double) pix_arr[i*COLS + k];
                    oup[i*COLS + k] = 0.0;
                }
            }
            SBtree_node *root = (SBtree_node *) malloc(sizeof(SBtree_node));
            root = sb_treeify(J, inp, ROWS, COLS);
            sb_tree_print_preorder(root);
            printf("\n");

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
