
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void swap();

int main(int argc, char *argv[]) {
    int n_threads = atoi(argv[1]);
    int SIZE = atoi(argv[2]);
    int A[SIZE];
    for (int i = 0; i < SIZE; i++) {
        A[i] = rand() % SIZE;
    }
    int N = SIZE;
    int i = 0, j = 0;
    int first;
    int phase;
    double start, end;
    start = omp_get_wtime();
    #pragma omp parallel num_threads(n_threads) default(none) shared(A, N) private(i, phase)
    for (phase = 0; phase < N; phase++) {
        if (phase % 2 == 0) {
            #pragma omp for
            for (int i = 1; i < N; i += 2) {
                if (A[i - 1] > A[i]) {
                    swap(&A[i - 1], &A[i]);
                }
            }
        } else {
            #pragma omp for
            for (int i = 1; i < N - 1; i += 2) {
                if (A[i] > A[i + 1]) {
                    swap(&A[i], &A[i + 1]);
                }
            }
        }
    }
    end = omp_get_wtime();

    printf("Time Parallel= %f", (end - start));
}

void swap(int *num1, int *num2) {

    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}
