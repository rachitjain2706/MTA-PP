#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

void mat_mult_thr(double *mat1, double *mat2, double *result, int dim, int nthr) {
    int part_rows, th_id;
    part_rows = dim / nthr;

    omp_set_num_threads(nthr);
    #pragma omp parallel shared(mat1, mat2, result, part_rows) private(th_id)
    {
        int i, j, k;

        #pragma omp for schedule(guided, part_rows)
        for (i = 0; i < dim; i++) {
            for (j = 0; j < dim; j++) {
                *(result + (j + i * dim)) = 0;
                for (k = 0; k < dim; k++)
                    *(result + (j + i * dim)) += *(mat1 + (k + i * dim)) * (*(mat2 + (j + k * dim)));
            }
        }
    }
}

void rand_mat(double *mat, int dim) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            *(mat + (j + i * dim)) = (10.0 * rand()) / RAND_MAX;
}

int main(int argc, char *argv[]) {
    int nthr = atoi(argv[1]);
    int dim = atoi(argv[2]);


    double *mat1 = (double *) malloc(dim * dim * sizeof(double));
    double *mat2 = (double *) malloc(dim * dim * sizeof(double));
    double *mat3 = (double *) malloc(dim * dim * sizeof(double));

    srand((unsigned int) time(NULL));
    rand_mat(mat1, dim);
    rand_mat(mat2, dim);

    mat_mult_thr(mat1, mat2, mat3, dim, nthr);

    free(mat1);
    free(mat2);
    free(mat3);

    return 0;
}
