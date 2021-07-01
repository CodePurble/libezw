#ifndef SMAP_H
#define SMAP_H
#include "sbtree.h"

enum smap_val { // significance map value
    SN, // Significant Negative
    SP, // Significant Positive
    ZR, // Zerotree Root
    IZ,// Isolated Zero
    U, // Unitialized
};

typedef struct Smap_tree_node {
    struct Smap_tree_node *c1;
    struct Smap_tree_node *c2;
    struct Smap_tree_node *c3;
    struct Smap_tree_node *c4;
    int isroot : 1;
    enum smap_val type;
    double coeff;
} Smap_tree_node;

Smap_tree_node* smap_tree_init_node(double coeff);
Smap_tree_node* smap_tree_init_root(double val,
        double c2val,
        double c3val,
        double c4val);
Smap_tree_node* smap_tree_insert_quad(
        Smap_tree_node *root,
        double *cvals,
        int child_level,
        int parent_t
        );
Smap_tree_node *smap_treeify(
        SBtree_node *sb_root,
        int levels,
        int rows,
        int cols
        );
void smap_tree_print_preorder(Smap_tree_node *root);

#endif // SMAP_H
