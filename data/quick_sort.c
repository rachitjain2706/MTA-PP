#include<stdio.h>
#include<omp.h>
#include<math.h>
#include <stdlib.h>

void swap(int *num1, int *num2) {
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        #pragma omp task firstprivate(arr, low, pi)
        {
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
}

int main(int argc, char *argv[]) {

    int n_threads = atoi(argv[1]);
    int ARRAY_MAX_SIZE = atoi(argv[2]);

    int arr[ARRAY_MAX_SIZE];

    double start_time, run_time;
    for (int i = 0; i < ARRAY_MAX_SIZE - 1; i++) {
        arr[i] = rand() % ARRAY_MAX_SIZE + 1;

    }
    int n = sizeof(arr) / sizeof(arr[0]);
    omp_set_num_threads(n_threads);
    start_time = omp_get_wtime();
    {
        int id = omp_get_thread_num();
        int nthrds = omp_get_num_threads();
        #pragma omp single nowait
        quickSort(arr, 0, n - 1);
    }
    run_time = omp_get_wtime() - start_time;
    printf("\n Execution time was %lf seconds\n ", run_time);
    return 0;
}
