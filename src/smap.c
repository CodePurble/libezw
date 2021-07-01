#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smap.h"
#include "utils.h"

Smap_tree_node* smap_tree_init_node(double coeff)
{
    Smap_tree_node *new_node = (Smap_tree_node *) malloc(sizeof(Smap_tree_node));
    new_node->coeff = coeff;
    new_node->type = U;
    new_node->c1 = NULL;
    new_node->c2 = NULL;
    new_node->c3 = NULL;
    new_node->c4 = NULL;
    new_node->isroot = 0;
    return new_node;
}

Smap_tree_node* smap_tree_init_root(
        double val,
        double c2val,
        double c3val,
        double c4val
        )
{
    Smap_tree_node *new_node = smap_tree_init_node(val);
    new_node->isroot = 1;
    Smap_tree_node *c2 = smap_tree_init_node(c2val);
    Smap_tree_node *c3 = smap_tree_init_node(c3val);
    Smap_tree_node *c4 = smap_tree_init_node(c4val);
    new_node->c2 = c2;
    new_node->c3 = c3;
    new_node->c4 = c4;
    return new_node;
}

Smap_tree_node* smap_tree_insert_quad(
        Smap_tree_node *root,
        double *cvals,
        int child_level,
        int parent_t
        )
{
    Smap_tree_node *curr = root;
    Smap_tree_node *c1 = smap_tree_init_node(cvals[0]);
    Smap_tree_node *c2 = smap_tree_init_node(cvals[1]);
    Smap_tree_node *c3 = smap_tree_init_node(cvals[2]);
    Smap_tree_node *c4 = smap_tree_init_node(cvals[3]);
    // Move into the level just above one where you want to insert
    switch(parent_t) {
        case 1:
            for(int i = 1; i < child_level; i++) {
                curr = curr->c1;
            }
            break;
        case 2:
            for(int i = 1; i < child_level; i++) {
                curr = curr->c2;
            }
            break;
        case 3:
            for(int i = 1; i < child_level; i++) {
                curr = curr->c3;
            }
            break;
        case 4:
            for(int i = 1; i < child_level; i++) {
                curr = curr->c4;
            }
            break;
    }
    curr->c1 = c1;
    curr->c2 = c2;
    curr->c3 = c3;
    curr->c4 = c4;
    return root;
}

Smap_tree_node *smap_treeify(
        SBtree_node *sb_root,
        int levels,
        int rows,
        int cols
        )
{
    if(levels == (int) log2(rows)) {
        Smap_tree_node *smap_root = smap_tree_init_root(
            sb_root->ll->coeffs[0],
            sb_root->hl->coeffs[0],
            sb_root->lh->coeffs[0],
            sb_root->hh->coeffs[0]
            );
        double *curr_coeffs[3] = {NULL};
        int dim = 2;
        double **quads = (double **) calloc(sizeof(double *), (dim*dim)/4);

        // Second level is inserted manually because there are only 3 children.
        // Everything else will have 4
        curr_coeffs[0] = sb_tree_get_coeff(sb_root, HL, dim);
        curr_coeffs[1] = sb_tree_get_coeff(sb_root, LH, dim);
        curr_coeffs[2] = sb_tree_get_coeff(sb_root, HH, dim);

        quads = quads_from_arr(curr_coeffs[0], dim, dim);
        smap_root = smap_tree_insert_quad(smap_root, quads[0], dim, 2);

        quads = quads_from_arr(curr_coeffs[1], dim, dim);
        smap_root = smap_tree_insert_quad(smap_root, quads[0], dim, 3);

        quads = quads_from_arr(curr_coeffs[2], dim, dim);
        smap_root = smap_tree_insert_quad(smap_root, quads[0], dim, 4);

        // Rest of the levels
        for(int i = 3; i <= levels; i++) {
            dim = pow(2, i-1);
            curr_coeffs[0] = sb_tree_get_coeff(sb_root, HL, i);
            curr_coeffs[1] = sb_tree_get_coeff(sb_root, LH, i);
            curr_coeffs[2] = sb_tree_get_coeff(sb_root, HH, i);

            quads = quads_from_arr(curr_coeffs[0], dim, dim);
            /* DEBUG_ARR_F_2(quads, (dim*dim)/4, 4); */
            for(int j = 0; j < dim; j++) {
                DEBUG_INT("j", j);
                smap_root = smap_tree_insert_quad(smap_root, quads[i], i-1, j+1);
            }
        }
        return smap_root;
    }
    else {
        return NULL;
    }
}

void smap_tree_print_preorder(Smap_tree_node *root)
{
    if(root) {
        printf("%f | ", root->coeff);
    }
    if(root->c1) {
        smap_tree_print_preorder(root->c1);
    }
    if(root->c2) {
        smap_tree_print_preorder(root->c2);
    }
    if(root->c3) {
        smap_tree_print_preorder(root->c3);
    }
    if(root->c4) {
        smap_tree_print_preorder(root->c4);
    }
}
