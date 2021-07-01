#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wavelib.h"
#include "utils.h"
#include "ezw.h"

void dominant_pass(SBtree_node *root, int levels, int rows, int cols)
{
    int ll_len = (rows/pow(2, levels))*(rows/pow(2, levels));
    double max = max_double(root->ll->coeffs, ll_len);
    int threshold = pow(2, (int) floor((int) log2(max)));
    SBtree_node *curr_node = root->ll;
    double curr_val = 0;
    while(1) {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++);
        }
    }
}
