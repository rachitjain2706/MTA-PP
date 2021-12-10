#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int nthreads, tid, idx;

    int n_threads = atoi(argv[1]);
    int N = atoi(argv[2]);
    float a[N], b[N], c[N];

    #pragma omp parallel for
    for (idx = 0; idx < N; ++idx) {
        a[idx] = b[idx] = rand() % N;
    }

    omp_set_num_threads(n_threads);
    #pragma omp parallel for
    for (idx = 0; idx < N; ++idx) {
        c[idx] = a[idx] + b[idx];
    }

    return 0;
}