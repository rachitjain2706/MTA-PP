#include <limits.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

// Parallel Histogram: Function to build histogram using parallel running threads
void parallel_histogram(float* numbers, int* bins, int num_count, int bin_count, int thread_count){

	// Initializing histogram bins
	for(int i = 0; i < bin_count; i++){
		bins[i] = 0;
	}

	// Setting up threads for parallelization - thread count provided to us in command line argument
	#pragma omp parallel num_threads(thread_count)
	{
		int bin_num; // stores the bin number that the floating number belongs to
		#pragma omp for nowait // distributing for loop iterations across threads..nowait is not necessary here 
		for(int i = 0; i < bin_count; i++){ // outer loop iterating over the bins - outer loop will be divided accross threads
			for(int j = 0; j < num_count; j++){ // inner loop iterating over all elements
				if(numbers[j] == 20){ // if number is 20 then put it in last bin
					bin_num = bin_count-1;
				}
				else{ // range covering each bin is 20/bin_count
					bin_num = (int)(numbers[j]*bin_count/20); 
				}
				 
				if(bin_num == i){ // update the bin which is assigned to the particular thread
					bins[i]++;
				}
			}
		}
	}

	for(int i = 0; i < bin_count; i++) { // printing the bins
		printf("bin[%d] = %d\n", i, bins[i]);
	}
}

int main(int argc, char *argv[]){
	int count;
	int i = 0;

	float* numbers = (float*) malloc(sizeof(float) * atoi(argv[2]));
	for(int i = 0; i < atoi(argv[2]); i++) {
		// numbers[i] = rand() % 20;
		numbers[i] = ((float) rand() / (float) RAND_MAX) * 20.0;
	}

	int* bins = (int*) malloc(sizeof(int) * 20); // initializing bins array

	parallel_histogram(numbers, bins, atoi(argv[2]), 20, atoi(argv[1])); // function call

	// free all the dynamically allocated arrays
	free(bins);
	free(numbers);
}