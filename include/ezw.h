#ifndef EZW_H
#define EZW_H
#include "smap.h"

Queue *dominant_pass(Smap_tree_node *smap_root, int threshold);
void check_descendants(Smap_tree_node *parent, int threshold);
enum smap_symbol encode_coeff(float coeff, int threshold);

#endif // EZW_H
