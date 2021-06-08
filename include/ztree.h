#ifndef ZTREE_H
#define ZTREE_H

enum smap_val { // significance mapping value
    SN, // Significant Negative
    SP, // Significant Positive
    ZR, // Zerotree Root 
    IZ,// Isolated Zero 
    U // Unitialized
};

typedef struct Ztree_node{
    struct Ztree_node *c1;
    struct Ztree_node *c2;
    struct Ztree_node *c3;
    struct Ztree_node *c4;
    enum smap_val type;
    double coeff;
} Ztree_node;

Ztree_node* ztree_init_root(
        double root, double c1, double c2, double c3); 
Ztree_node* ztree_insert(
        Ztree_node *root, double coeff, int parent);
Ztree_node* ztree_init_node(double coeff);
#endif // ZTREE_H
