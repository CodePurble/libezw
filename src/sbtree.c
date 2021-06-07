#include <stdio.h>
#include <stdlib.h>
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

// Returns NULL if required target not found or if args are not applicable,
// e.g. sb_type is "ROOT", level is out of bounds
// Perform NULL check before using
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
        printf("%f | ", root->coeffs[0]);
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
