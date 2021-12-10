#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int NUM_POINTS = 0;
static int n_threads = 0;

void count_sort(int a[], int n) {
    int i, j, count;
    int *temp = malloc(n * sizeof(int));

    #pragma omp parallel for shared(a, n, temp) private(i, j, count)
    for (i = 0; i < n; i++) {
        count = 0;
        for (j = 0; j < n; j++)
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        temp[count] = a[i];
    }

    omp_set_num_threads(n_threads);
    #pragma omp parallel for shared(a, n, temp) private(i)
    for (i = 0; i < n; i++)
        a[i] = temp[i];

    free(temp);
}

void get_options(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                NUM_POINTS = atoi(optarg);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    int n, *a;

    n_threads = atoi(argv[1]);
    int NUM_POINTS = atoi(argv[2]);

    n = NUM_POINTS;
    a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        a[i] = rand() % n;

    count_sort(a, n);

    return 0;
}
