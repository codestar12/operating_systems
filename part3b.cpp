#include <pthread.h>
#include <cstdio>
#include <sys/time.h>
#include <cstdlib>

#define NUM_THREADS 8

int private_count[NUM_THREADS];

int* my_array;

int const SIZE = 10000000;

void* count_99(void* rank){
	long my_rank = (long) rank;
	for(long i = my_rank; i < SIZE; i+=NUM_THREADS){
		if(my_array[i] == 99)
			private_count[my_rank] += 1;
	}
}

int main(){

	my_array = new int[SIZE];
	int count = 0, i;
	timeval start_time, start_time_p, end_time, end_time_p;
	pthread_t workers[NUM_THREADS];

	//intitalize random number generator
	srand((unsigned)time(NULL));

	// Initialized the array using random numbers
	for(i=0; i<SIZE; i++){
		my_array[i] = rand() % 100;
	}

	//start_time = omp_get_wtime();
	gettimeofday(&start_time, NULL);
	for(i=0; i<SIZE; i++){
		if(my_array[i] == 99)
			count++;
	}
	//end_time = omp_get_wtime();
	gettimeofday(&end_time, NULL);

	double runtime = end_time.tv_sec + end_time.tv_usec / 1000000.0 - start_time.tv_sec - start_time.tv_usec / 1000000.0;
	printf("The serial code indicates there are %d 99s in the array.\n\n", count);
	printf("The serial code used %f seconds to complete the execution. \n\n", runtime);

	// setup for pthreads
	count = 0;

	for(i=0; i<NUM_THREADS; i++){
		private_count[i] = 0;
	}

	gettimeofday(&start_time_p, NULL);
	for(i=0; i<NUM_THREADS; i++){
		pthread_create(&workers[i], NULL, count_99, (void*)(i));
	}

	for(i=0; i<NUM_THREADS; i++)
		pthread_join(workers[i], NULL);

	for(i=0; i<NUM_THREADS; i++)
		count += private_count[i];
	gettimeofday(&end_time_p, NULL);



	// omp_set_num_threads(4);
	// start_time_p = omp_get_wtime();
	// #pragma omp parallel for shared(my_array) private(i) reduction(+:count)
	// for(i=0; i<SIZE; i++){
	// 	if(my_array[i] == 99)
	// 		count++;
	// }
	// end_time_p = omp_get_wtime();

	runtime = end_time_p.tv_sec + end_time_p.tv_usec / 1000000.0 - start_time_p.tv_sec - start_time_p.tv_usec / 1000000.0;
	printf("The parallel code indicates there are %d 99s in the array.\n\n", count);
	printf("The parallel code used %f seconds to complete the execution. \n\n", runtime);

	return 0;
}