#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wavelib.h"
#include "sbtree.h"

SBtree_node* sb_tree_init_root()
{
    SBtree_node* root = (SBtree_node *) malloc(sizeof(SBtree_node));
    root->ll = NULL;
    root->hl = NULL;
    root->lh = NULL;
    root->hh = NULL;
    root->coeffs = NULL;
    root->type = ROOT;
    return root;
}

// Returns NULL if "root" is uninitialized
// Perform NULL check before using
SBtree_node* sb_tree_insert(SBtree_node *root, enum sb_type t, double *coeffs)
{
    if(root) {
        SBtree_node *curr = root;
        SBtree_node *new = (SBtree_node *) malloc(sizeof(SBtree_node));
        new->type = t;
        new->coeffs = coeffs;
        new->ll = NULL;
        new->hl = NULL;
        new->lh = NULL;
        new->hh = NULL;

        // Move to next level only if all subbands in current level are filled
        while(curr && curr->ll && curr->hl && curr->lh && curr->hh) {
            curr = curr->ll;
        }
        switch(t) {
            case LL:
                curr->ll = new;
                break;
            case HL:
                curr->hl = new;
                break;
            case LH:
                curr->lh = new;
                break;
            case HH:
                curr->hh = new;
                break;
            case ROOT:
                // do nothing
                break;
        }
        return root;
    }
    else {
        return NULL;
    }
}

// Returns NULL if a set of subband coeffs is not found
// Remember to check for NULL when using
double* sb_tree_get_coeff(SBtree_node *root, enum sb_type t, int level)
{
    if(level > 0) {
        SBtree_node *curr = root;
        for(int i = 1; i < level; i++) {
            if(curr) {
                curr = curr->ll;
            }
            else {
                return NULL;
            }
        }
        switch(t) {
            case LL:
                return(curr->ll->coeffs);
                break;
            case HL:
                return(curr->hl->coeffs);
                break;
            case LH:
                return(curr->lh->coeffs);
                break;
            case HH:
                return(curr->hh->coeffs);
                break;
            case ROOT:
                return NULL;
                break;
            default:
                return NULL;
                break;
        }
    }
    else {
        return NULL;
    }
}

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

    /* for(int i = 0; i < levels; i++) { */
    /*     curr_wavecoeffs = dwt2(wt2_arr[i], input); */
    /*     subcoeffs_A = getWT2Coeffs(wt2_arr[i], curr_wavecoeffs, i+1, "A", &sub_r, &sub_c); */
    /*     subcoeffs_H = getWT2Coeffs(wt2_arr[i], curr_wavecoeffs, i+1, "H", &sub_r, &sub_c); */
    /*     subcoeffs_V = getWT2Coeffs(wt2_arr[i], curr_wavecoeffs, i+1, "V", &sub_r, &sub_c); */
    /*     subcoeffs_D = getWT2Coeffs(wt2_arr[i], curr_wavecoeffs, i+1, "D", &sub_r, &sub_c); */
    /*     root = sb_tree_insert(root, LL, subcoeffs_A); */
    /*     root = sb_tree_insert(root, HL, subcoeffs_H); */
    /*     root = sb_tree_insert(root, LH, subcoeffs_V); */
    /*     root = sb_tree_insert(root, HH, subcoeffs_D); */
    /* } */
    for(int i = levels; i > 0; i--) {
        curr_wavecoeffs = dwt2(wt2_arr[i-1], input);
        subcoeffs_A = getWT2Coeffs(wt2_arr[i-1], curr_wavecoeffs, i, "A", &sub_r, &sub_c);
        /* dispWT2Coeffs(subcoeffs_A, sub_r, sub_c); */
        subcoeffs_H = getWT2Coeffs(wt2_arr[i-1], curr_wavecoeffs, i, "H", &sub_r, &sub_c);
        /* dispWT2Coeffs(subcoeffs_H, sub_r, sub_c); */
        subcoeffs_V = getWT2Coeffs(wt2_arr[i-1], curr_wavecoeffs, i, "V", &sub_r, &sub_c);
        /* dispWT2Coeffs(subcoeffs_V, sub_r, sub_c); */
        subcoeffs_D = getWT2Coeffs(wt2_arr[i-1], curr_wavecoeffs, i, "D", &sub_r, &sub_c);
        /* dispWT2Coeffs(subcoeffs_D, sub_r, sub_c); */
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

/* double* sb_tree_get_quad( */
/*         SBtree_node *root, */
/*         int block_num, */
/*         int levels, */
/*         int rows, */
/*         int cols */
/*         ) */
/* { */
/*     double quad[2][2]; */
/*     enum sb_type subbands[4] = {LL, HL, LH, HH}; */
/*     int maxlevels = log2(rows); */
/*     if(block_num < 0 || levels > maxlevels || levels <= 0) { */
/*         return NULL; */
/*     } */
/*     else { */
/*         SBtree_node *curr = root; */
/*         SBtree_node *refnode = root; */
/*         int currlevel = levels; */
/*         int curredge; */
/*         for(int i = 0; i < block_num; i++) { */
/*             curredge = rows / (pow(2, currlevel)); */
/*         } */
/*     } */
/* } */

// Recursively delete tree
// Steps:
// Save LL subband of current node
// Delete all subbands except LL for current node
// Delete the current node
// Recursively call on saved LL band (the new "root")
void sb_tree_free(SBtree_node *root)
{
    if(root) {
        SBtree_node *temp_ll = root->ll;
        free(root->hl);
        free(root->lh);
        free(root->hh);
        free(root);
        sb_tree_free(temp_ll);
    }
}

// DOES NOT print newline at the end, leaves trailing whitespace
void sb_tree_print_preorder(SBtree_node *root)
{
    if(root && root->coeffs) {
        printf("%f\n", root->coeffs[0]);
    }
    if(root->ll) {
        sb_tree_print_preorder(root->ll);
    }
    if(root->hl) {
        sb_tree_print_preorder(root->hl);
    }
    if(root->lh) {
        sb_tree_print_preorder(root->lh);
    }
    if(root->hh) {
        sb_tree_print_preorder(root->hh);
    }
}
