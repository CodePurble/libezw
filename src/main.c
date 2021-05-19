#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "wavelib.h"

#define ROWS 512
#define COLS 512

int main()
{
    // FILE *fptr = fopen("/mnt/d/libezw/utils/raw_lichtenstein_img_processing_test_bw.bin", "rb");
    FILE *fptr = fopen("/mnt/d/libezw/build/ll.bin", "wb");

    unsigned char *pix_arr = calloc(ROWS*COLS, 1); // array of 8 bit vals
    if(!fptr) {
        fprintf(stderr, "Unable to open file\n");
    }
    else {
        // read_binary_file(fptr, pix_arr, ROWS, COLS);
        // fclose(fptr);
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

        // dispWT2Coeffs(cLL, ir, ic);
        printf("Hello %f\n",cLL[100]);

        write_binary_file(fptr, cLL, ROWS/4, COLS/4);
        wt2_summary(wt);

        // clean up
        wave_free(obj);
        wt2_free(wt);
        free(inp);
        free(wavecoeffs);
        free(oup);
    }
    return 0;
}
