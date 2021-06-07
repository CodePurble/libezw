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

SBtree_node* sb_tree_init_root();

SBtree_node* sb_tree_insert(SBtree_node *root, enum sb_type t, double *coeffs);

double* sb_tree_get_coeff(SBtree_node *root, enum sb_type t, int level);

void sb_tree_free(SBtree_node *root);

void sb_tree_print_preorder(SBtree_node *root);
#endif // SBTREE_H
