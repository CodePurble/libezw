#ifndef SMAP_H
#define SMAP_H
#include "sbtree.h"
#include "queue.h"

enum smap_symbol { // significance map value
    SP = 0, // Significant Positive
    SN = 1, // Significant Negative
    IZ = 2,// Isolated Zero
    ZR = 3, // Zerotree Root
    U = -1, // Unitialized
};

enum print_conf {
    COEFF,
    TYPE,
    ALL
};

typedef struct Smap_tree_node {
    double coeff;
    struct Smap_tree_node *parent;
    struct Smap_tree_node *children[4];
    char isroot : 1;
    enum smap_symbol type;
    unsigned char not_available;
} Smap_tree_node;

// OPTIMIZE: Maybe pass by reference? Soo many copies!!

char* smap_symbol_to_str(enum smap_symbol s);

void smap_tree_print_preorder(Smap_tree_node *root, enum print_conf p);
Smap_tree_node *smap_tree_init_node(double coeff);
Smap_tree_node *smap_tree_init_root(double val,
                                    double c1val,
                                    double c2val,
                                    double c3val);

Queue *smap_tree_insert_quad(Queue *q, double *cvals);
Smap_tree_node *smap_treeify(SBtree_node *sb_root, int levels);
Smap_tree_node* smap_tree_reset(Smap_tree_node *root);
#endif // SMAP_H
