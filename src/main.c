#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "wavelib.h"

#define ROWS 512
#define COLS 512

int main(int argc, char **argv)
{
    bool return_error = false;

    if(argc == 1) {
        fprintf(stderr, "Error: Requires at least one argument\n");
        return_error = true;
    }
    else if(argc == 2) {
        unsigned char *pix_arr = calloc(ROWS*COLS, 1); // array of 8 bit vals
        FILE *img_bin = fopen(argv[1], "rb");
        if(!img_bin) {
            fprintf(stderr, "Unable to open file: %s\n", argv[1]);
            return_error = true;
        }
        else {
            read_binary_file(img_bin, pix_arr, ROWS, COLS);
            fclose(img_bin);
            free(pix_arr);
        }
    }
    else if(argc == 3) {
        FILE *img_bin = fopen(argv[1], "rb");
        FILE *coeff_bin = fopen(argv[2], "wb");
        unsigned char *pix_arr = calloc(ROWS*COLS, 1); // array of 8 bit vals

        if(!img_bin) {
            fprintf(stderr, "Unable to open file: %s\n", argv[1]);
            return_error = true;
        }
        else {
            read_binary_file(img_bin, pix_arr, ROWS, COLS);
            fclose(img_bin);
        }

        if(!coeff_bin) {
            fprintf(stderr, "Unable to open file: %s\n", argv[2]);
            return_error = true;
        }
        else {
            wave_object obj;
            wt2_object wt;
            double *inp, *wavecoeffs, *oup, *cLL;
            int N = ROWS*COLS;
            int ir, ic;

            char *name = "db2";
            obj = wave_init(name);// Initialize the wavelet
            inp = (double*) calloc(N, sizeof(double));
            oup = (double*) calloc(N, sizeof(double));

            int J = 1; // number of decompositions

            wt = wt2_init(obj, "dwt", ROWS, COLS, J);

            for (int i = 0; i < ROWS; ++i) {
                for (int k = 0; k < COLS; ++k) {
                    inp[i*COLS + k] = (double) pix_arr[i*COLS + k];
                    oup[i*COLS + k] = 0.0;
                }
            }
            wavecoeffs = dwt2(wt, inp);

            cLL = getWT2Coeffs(wt, wavecoeffs, 1, "A", &ir, &ic);

            /* dispWT2Coeffs(cLL, ir, ic); */

            // This is very very bad, just here to test if file is written
            // TODO: Perform IDWT, map those vals to (0..255), and write
            // those to the binary file
            write_binary_file(img_bin, (unsigned char *) cLL, ROWS/4, COLS/4);
            /* wt2_summary(wt); */

            // clean up
            wave_free(obj);
            wt2_free(wt);
            free(inp);
            free(wavecoeffs);
            free(oup);
            free(pix_arr);
            fclose(coeff_bin);
        }
    }

    if(return_error) {
        return 1;
    }
    else {
        return 0;
    }
}
