/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Swap the grid for GSL to use.
 *
 *        Version:  1.0
 *        Created:  08/04/2021 10:20:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Skanda Prasad
 *   Organization:  -
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SIZE 16
#define BASE 2

int level_collection[SIZE]; // Collection of decomposition levels of first element of each row
double new_arr[SIZE][SIZE]; // 2D Array form of the original linear array
double final_arr[SIZE][SIZE]; // Final array after swapping

// double get_reflection(double); // No need currently
void swap_decision(int[], double[SIZE][SIZE]); // Decides if the element has to be swapped or not and performs the swap if necessary
void find_levels(double [SIZE * SIZE]);
/*
 * Converts arr[SIZE * SIZE] into 2D array of SIZE x SIZE
 * Assigns the level of decomposition of first element of each row
 * */
int log_base(int); // Calculates variable base logarithm of a value

double arr[256] = {
        3248.88,-154.875,57.625,90.875,75,-1.75,65,-138.5,3,21.5,0,-15.5,41,122.5,-21.5,-66,
        129.125,79.125,162.125,26.125,68.75,-31.25,34.5,5,48.5,-20,-1.5,17.5,-58.5,35,-37.5,-17,
        54.125,-136.375,19.625,-16.875,37.25,7,-3.5,-41,70.5,-19.5,4,29.5,-65,34.5,-8.5,18.5,
        -2.125,-103.875,22.125,-46.625,81.75,-11.25,3.5,58.25,1,30.5,-26.5,-25.5,31.5,-5.5,6.5,8.5,
        91,88.25,-173,-329,-45.5,-17.75,107.5,-108,-17.5,4.5,27.5,6,-31,-8,5,7,
        -22.75,-43.25,25.75,-35,-10.75,84.75,-101.25,-7,0,68.5,-15.5,-12,-39.5,39.5,6.5,-14.5,
        32.75,-12.5,-45,-63.5,-63.25,22.5,114.5,82,0,65.5,-1,23.5,-66,32,-39.5,0,
        -54.75,-39.75,-22.5,110.75,60.75,44.75,-57.5,-107.75,-2,27,-1,-27,2,67,-55.5,117.5,
        -3,30.5,67,47.5,-164,-132.5,-21.5,-189,3,-23.5,1,15.5,41,-81.5,21.5,-13,
        33.5,-25,-15.5,28.5,13.5,-54,-37.5,21,-33.5,28,-3.5,0.5,27.5,37,-51.5,17,
        -16.5,16.5,-10,34.5,-3,35.5,-1.5,-23.5,4.5,19.5,-4,3.5,-32,-28.5,31.5,-15.5,
        39,-49.5,-71.5,-39.5,3.5,3.5,2.5,5.5,9,-4.5,19.5,-25.5,-3.5,11.5,2.5,-10.5,
        -27.5,4.5,71.5,-4,33,28,8,1,-20.5,-4.5,16.5,-3,42,-37,-3,47,
        0,22.5,-8.5,0,405,-41.5,-43.5,-15.5,0,5.5,-20.5,0,-1.5,39.5,-10.5,-18.5,
        0,-1.5,2,27.5,-37,17,-20.5,0,0,-19.5,12,-16.5,-17,-17,-6.5,4,
        -4,-27,-67,-57,3,-5,35.5,137.5,0,27,-12,-18,1,5,34.5,-32.5,
    };

int main()
{
    find_levels(arr);
    swap_decision(level_collection, new_arr);
    return 0;
}

void find_levels(double a[])
{
    for(int i = 0; i < SIZE; i++)
    {
        level_collection[i] = 1 + log_base(SIZE/(i+1));

        for(int j = 0; j < SIZE; j++)
        {
            new_arr[i][j] = a[i*SIZE + j];
            printf("%.2f\t", new_arr[i][j]);
        }
        printf("\n");
    }

    level_collection[0] = log_base(SIZE/(1));
    level_collection[1] = level_collection[0];
    
//    for(int i = 0; i < SIZE; i++)
//        printf("%d\t", level_collection[i]);
}    

int log_base(int x)
{
    int result;
    result = log2(x)/log(2);

    return result;
}

// REFACTOR:
// + Remove globals and pass as parameters
// + Why do we have multiple copies of the ratio
void swap_decision(int c[SIZE], double a[SIZE][SIZE])
{
    int ratio, temp_ratio;

    for(int i = 0; i < SIZE; i++)
    {
        ratio = (SIZE/pow(2, c[i]));
        temp_ratio = ratio;

        for(int j = 0; j < SIZE; j++)
        {
            if(i == 0 || i == 1)
            {
                final_arr[i][j] = new_arr[i + ratio][j - ratio];
            }
            else 
            {
                if(ratio)
                {
                    final_arr[i][j] = new_arr[i + ratio][j - ratio];
                    ratio--;
                }
                else if(temp_ratio)
                {
                    final_arr[i][j] = new_arr[i][j];
                    temp_ratio--;
                }
                else
                {
                    final_arr[i][j] = new_arr[i + ratio][j - ratio];
                }
            }
            printf("%.2f\t", final_arr[i][j]);
        }

        printf("\n");
    }
}




















