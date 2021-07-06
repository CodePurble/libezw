#ifndef SMAP_H
#define SMAP_H
#include "sbtree.h"
#include "queue.h"

enum smap_val { // significance map value
    SN, // Significant Negative
    SP, // Significant Positive
    ZR, // Zerotree Root
    IZ,// Isolated Zero
    U, // Unitialized
};

typedef struct Smap_tree_node {
    double coeff;
    struct Smap_tree_node *children[4];
    char isroot : 1;
    enum smap_val type;
} Smap_tree_node;

void smap_tree_print_preorder(Smap_tree_node *root);
Smap_tree_node *smap_tree_init_node(double coeff);

Smap_tree_node *smap_tree_init_root(double val,
                                    double c1val,
                                    double c2val,
                                    double c3val);

Queue *smap_tree_insert_quad(Queue *q, double *cvals);
Smap_tree_node *smap_treeify(SBtree_node *sb_root, int levels);



#endif // SMAP_H
