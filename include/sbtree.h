#ifndef SBTREE_H
#define SBTREE_H

enum sb_type {
    LL,
    HL,
    LH,
    HH,
    ROOT
};

typedef struct SBtree_node {
    struct SBtree_node *ll;
    struct SBtree_node *hl;
    struct SBtree_node *lh;
    struct SBtree_node *hh;
    double *coeffs;
    enum sb_type type;
} SBtree_node;

// OPTIMIZE: Maybe pass by reference? Soo many copies!!

SBtree_node* sb_tree_init_root();
SBtree_node* sb_tree_insert(SBtree_node *root, enum sb_type t, double *coeffs);
SBtree_node* sb_treeify(int levels, double *input, int rows, int cols);
double* sb_tree_get_coeff(SBtree_node *root, enum sb_type t, int level);
void sb_tree_free(SBtree_node *root);
void sb_tree_print_preorder(SBtree_node *root);
// double* sb_tree_get_quad(
//         SBtree_node *root,
//         int block_num,
//         int levels,
//         int rows,
//         int cols
//         );
#endif // SBTREE_H
