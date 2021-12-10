// gcc -std=c99 -fopenmp -o merge_sort merge_sort.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"

void generate_list(int *x, int n) {
    int i, j, t;
    for (i = 0; i < n; i++)
        x[i] = rand() % n;
}

void print_list(int *x, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", x[i]);
    }
}

void merge(int *X, int n, int *tmp) {
    int i = 0;
    int j = n / 2;
    int ti = 0;
    while (i < n / 2 && j < n) {
        if (X[i] < X[j]) {
            tmp[ti] = X[i];
            ti++;
            i++;
        } else {
            tmp[ti] = X[j];
            ti++;
            j++;
        }
    }
    while (i < n / 2) {
        tmp[ti] = X[i];
        ti++;
        i++;
    }
    while (j < n) {
        tmp[ti] = X[j];
        ti++;
        j++;
    }
    memcpy(X, tmp, n * sizeof(int));

}

void mergesort(int *X, int n, int *tmp) {
    if (n < 2) return;

    #pragma omp task firstprivate (X, n, tmp)
    mergesort(X, n / 2, tmp);

    #pragma omp task firstprivate (X, n, tmp)
    mergesort(X + (n / 2), n - (n / 2), tmp);

    #pragma omp taskwait

    merge(X, n, tmp);
}


int main(int argc, char *argv[]) {
    int n_threads = atoi(argv[1]);
    int n = atoi(argv[2]);
    double start, stop;

    int data[n], tmp[n];

    generate_list(data, n);
    start = omp_get_wtime();
    #pragma omp parallel num_threads(n_threads)
    {
    #pragma omp single
        mergesort(data, n, tmp);
    }
    stop = omp_get_wtime();
    printf("\nTime: %g\n", stop - start);
}
