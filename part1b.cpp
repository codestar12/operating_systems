#include <cstdlib>
#include <cstdio>
#include <Windows.h>

DWORD* fib_array;
const static int NUM_THREADS = 4;
int num_of_fib;

 DWORD fib(int n){

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

DWORD WINAPI find_fib(LPVOID rank){
	DWORD my_rank = *(DWORD*) rank;
	//alternating assignment for threads to balance load
	// and help ensure sub answers have been pre calculated
	for(DWORD i = my_rank; i < num_of_fib; i += NUM_THREADS){
		fib_array[i] = fib(i);
	}
	return NULL;
}

int main(int argc, char* argv[]){
	printf("Hw1 Part 1a\n");

	if(argc != 2){fprintf(stderr, "usage: %s num_to_generate\n", argv[0]); exit(-1);}
	num_of_fib = atoi(argv[1]);
	//if(num_of_fib < 1 || num_of_fib > 45){fprintf(stderr, "error: nuber must be 0 < n <= 45\n"); exit(-1);}

	fib_array = new DWORD [num_of_fib];

	// set value flag for caching
	/*
	for(int i = 0; i < num_of_fib; i++){
		fib_array[i] = -1;
	}
	*/
	DWORD threads[NUM_THREADS];
	DWORD thread_id[NUM_THREADS];
	HANDLE* thread_handles;

	thread_handles = new HANDLE[NUM_THREADS];

	for (DWORD thread = 0; thread < NUM_THREADS ; thread++){
		threads[thread] = thread;
		//pthread_create(&thread_handles[thread], NULL, find_fib, (void*)(thread));
		thread_handles[thread] = CreateThread(NULL, 0, find_fib, &threads[thread], 0, &thread_id[thread]);
	}

	for(DWORD thread = 0; thread < NUM_THREADS; thread++){
		if (thread_handles[thread] != NULL) {
			WaitForSingleObject(thread_handles[thread], INFINITE);
			CloseHandle(thread_handles[thread]);
		}
	}

	for(int i = 0; i < num_of_fib; i++)
		printf("%ld ", fib_array[i]);

	printf("\n");

	return 0;
}