#include <cstdlib>
#include <cstdio>
#include <pthread.h>

static long long* fib_array;
const static int NUM_THREADS = 1;
static int num_of_fib;

static long long fib(int n){

	if(n == 0){

		return 0;

	}else if(n == 1){

		return 1;
	// check array to see if it has been
	// calculated before
	// }else if(fib_array[n] > 0){
	// 	return fib_array[n];
	}else{
		return fib(n-1) + fib(n-2);
	}
}

static void* find_fib(void* rank){
	long my_rank = (long) rank;
	//alternating assignment for threads to balance load
	// and help ensure sub answers have been pre calculated
	for(long i = my_rank; i < num_of_fib; i += NUM_THREADS){
		fib_array[i] = fib(i);
	}
	return NULL;
}

int main(int argc, char* argv[]){
	printf("Hw1 Part 1a\n");

	if(argc != 2){fprintf(stderr, "usage: %s num_to_generate\n", argv[0]); exit(-1);}
	num_of_fib = atoi(argv[1]);
	//if(num_of_fib < 1 || num_of_fib > 45){fprintf(stderr, "error: nuber must be 0 < n <= 45\n"); exit(-1);}

	fib_array = new long long[num_of_fib];

	// set value flag for caching
	for(int i = 0; i < num_of_fib; i++){
		fib_array[i] = -1;
	}

	long thread;
	pthread_t* thread_handles;

	thread_handles = new pthread_t[NUM_THREADS];

	for (thread = 0; thread < NUM_THREADS ; thread++){
		pthread_create(&thread_handles[thread], NULL, find_fib, (void*)(thread));
	}

	for(thread = 0; thread < NUM_THREADS; thread++)
		pthread_join(thread_handles[thread], NULL);

	for(int i = 0; i < num_of_fib; i++)
		printf("%lld ", fib_array[i]);

	printf("\n");

	return 0;
}