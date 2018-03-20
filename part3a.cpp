#include <omp.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>

int main(){
	int count = 0, i;
	int const SIZE = 1000000000;
	int* my_array;
	my_array = new int[SIZE];
	double start_time, start_time_p, end_time, end_time_p;

	//intitalize random number generator
	srand((unsigned)time(NULL));

	// Initialized the array using random numbers
	for(i=0; i<SIZE; i++){
		my_array[i] = rand() % 100;
	}

	start_time = omp_get_wtime();
	for(i=0; i<SIZE; i++){
		if(my_array[i] == 99)
			count++;
	}
	end_time = omp_get_wtime();

	printf("The serial code indicates there are %d 99s in the array.\n\n", count);
	printf("The serial code used %f seconds to complete the execution. \n\n", end_time - start_time);

	count = 0;
	omp_set_num_threads(4);
	start_time_p = omp_get_wtime();
	#pragma omp parallel for shared(my_array) private(i) reduction(+:count)
	for(i=0; i<SIZE; i++){
		if(my_array[i] == 99)
			count++;
	}
	end_time_p = omp_get_wtime();

	printf("The parallel code indicates there are %d 99s in the array.\n\n", count);
	printf("The parallel code used %f seconds to complete the execution. \n\n", end_time_p - start_time_p);

	return 0;
}