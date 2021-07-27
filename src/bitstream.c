#include "bitstream.h"
#include "smap.h"

void symbol_set_init(struct symbol_set *s_set, unsigned short threshold, unsigned int size)
{
    s_set->threshold = threshold;
    s_set->size = size;
}

void symbol_group_insert(struct symbols_group *s_grp, int count, unsigned char symbol)
{
    switch(count) {
        case 0:
            s_grp->s0 = symbol;
            break;
        case 1:
            s_grp->s1 = symbol;
            break;
        case 2:
            s_grp->s2 = symbol;
            break;
        case 3:
            s_grp->s3 = symbol;
            break;
        case 4:
            s_grp->s4 = symbol;
            break;
        case 5:
            s_grp->s5 = symbol;
            break;
        case 6:
            s_grp->s6 = symbol;
            break;
        case 7:
            s_grp->s7 = symbol;
            break;
        case 8:
            s_grp->s8 = symbol;
            break;
        case 9:
            s_grp->s9 = symbol;
            break;
        default: return;
    }
}
