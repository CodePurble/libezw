#include <stdio.h>
#include <stdlib.h>
#include "wavelib.h"
#include "ezw.h"

SBtree_node* sb_treeify(int levels, double* input, int rows, int cols)
{
    SBtree_node *root = sb_tree_init_root();
    wave_object w_obj;
    w_obj = wave_init("haar");
    wt2_object* wt2_arr = (wt2_object *) calloc(sizeof(wt2_object), levels);
    double *subcoeffs_A; // LL
    double *subcoeffs_H; // HL
    double *subcoeffs_V; // LH
    double *subcoeffs_D; // HH
    double *curr_wavecoeffs;
    int sub_r, sub_c;

    for(int i = 0; i < levels; i++) {
        wt2_arr[i] = wt2_init(w_obj, "dwt", rows, cols, i+1);
    }

    for(int i = 0; i < levels; i++) {
        curr_wavecoeffs = dwt2(wt2_arr[i], input);
        subcoeffs_A = getWT2Coeffs(wt2_arr[i], curr_wavecoeffs, i+1, "A", &sub_r, &sub_c);
        subcoeffs_H = getWT2Coeffs(wt2_arr[i], curr_wavecoeffs, i+1, "H", &sub_r, &sub_c);
        subcoeffs_V = getWT2Coeffs(wt2_arr[i], curr_wavecoeffs, i+1, "V", &sub_r, &sub_c);
        subcoeffs_D = getWT2Coeffs(wt2_arr[i], curr_wavecoeffs, i+1, "D", &sub_r, &sub_c);
        root = sb_tree_insert(root, LL, subcoeffs_A);
        root = sb_tree_insert(root, HL, subcoeffs_H);
        root = sb_tree_insert(root, LH, subcoeffs_V);
        root = sb_tree_insert(root, HH, subcoeffs_D);
    }

    // clean up
    wave_free(w_obj);
    for(int i = 0; i < levels; i++) {
        wt2_free(wt2_arr[i]);
    }
    free(wt2_arr);
    return(root);
}

