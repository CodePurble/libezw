#ifndef SMAP_H
#define SMAP_H
#include "sbtree.h"
#include "queue.h"

enum print_conf {
    COEFF,
    TYPE,
    ALL
};

typedef struct Approx_map_node {
    unsigned char coeff;
    struct Approx_map_node *parent;
    struct Approx_map_node *children[4];
    char isroot : 1;
} Approx_map_node;

// OPTIMIZE: Maybe pass by reference? Soo many copies!!

void amap_tree_print_levelorder(Approx_map_node *root, enum print_conf p);
void amap_tree_print_preorder(Approx_map_node *root, enum print_conf p);
Approx_map_node *amap_tree_init_node(unsigned char symbol);
Approx_map_node *amap_tree_init_root(unsigned char val,
                                    unsigned char c1val,
                                    unsigned char c2val,
                                    unsigned char c3val);

Queue *amap_tree_insert_quad(Queue *q, unsigned char *cvals);
Approx_map_node *amap_treeify(SBtree_node *sb_root, int levels);
unsigned char *amap2levelorder(Approx_map_node *root, int rows, int cols);
#endif // SMAP_H
