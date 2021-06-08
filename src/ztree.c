#include <stdio.h>
#include <stdlib.h>
#include "ztree.h"

Ztree_node* ztree_init_root(double root, double c1, double c2, double c3)
{
    Ztree_tree* root = ztree_init_node(root);
    root->c1 = ztree_init_node(c1);
    root->c2 = ztree_init_node(c2);
    root->c3 = ztree_init_node(c3);
    return root;
}

Ztree_node* ztree_init_node(double coeff)
{
    Ztree_node *new_node= (Ztree_node *) malloc(sizeof(Ztree_node));
    new_node->c1 = NULL;
    new_node->c2 = NULL;
    new_node->c3 = NULL;
    new_node->c4 = NULL;
    new_node->coeff = coeff;
    new_node->type = U;
    return new_node;
}

// TODO: 1) Add 4 nodes at a time to tree
//       2) 

Ztree_node* ztree_insert(Ztree_node *root, double coeff, int parent)
{
    if(root) {
        Ztree_node *new_node = ztree_init_node(coeff);
        Ztree_node *curr_node;
        switch(parent) {
            case 1: 
                curr_node = root->c1;
                break;
            case 2: 
                curr_node = root->c2;
                break;
            case 3: 
                curr_node = root->c3;
                break;
            default: 
                return NULL; // Invalid Parent ID 
                break;
        }
        
    while(curr)


    } 
    else {
        return NULL;
    }        
    
    

}
