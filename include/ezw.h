#ifndef EZW_H
#define EZW_H
#include "smap.h"

Queue *dominant_pass(Smap_tree_node *smap_root, int threshold);
Queue *subordinate_pass(Queue *dominant_list, int threshold);
void check_descendants(Smap_tree_node *parent, int threshold);
enum smap_symbol encode_coeff(float coeff, int threshold);
Queue *dominant_pass(Smap_tree_node *smap_root, int threshold);
Queue *subordinate_pass(Queue *dominant_list, int threshold);
void ezw(Smap_tree_node *smap_root, unsigned char iter);

// NOTE: when decoding, just do morton scan, but replace coeff with decoded
// NOTE: approximation only if the current node coeff is zero

#endif // EZW_H
