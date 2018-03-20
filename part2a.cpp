#include <pthread.h>
#include <cstdio>

#define NUM_THREADS 10
#define M 3
#define K 2
#define N 3

int A[M][K] = {{1,4},{2,5},{3,6}};
int B[K][N] = {{8,7,6},{5,4,3}};
int C[M][N];

struct V{
	int i; /* row */
	int j; /* column */
};

void* calculate_index(void* data){
	V* index_data;
	index_data = (struct V*)data;
	int total = 0;
	for(int k = 0; k<K; k++){
		total+= A[index_data->i][k]*B[k][index_data->j];
	}
	C[index_data->i][index_data->j] = total;
}

int main(){

	pthread_t workers[NUM_THREADS];
	int i, j;

	for(i=0; i < M; i++){
		for(j=0; j < N; j++){
			V* data;
			data = new V();
			data->i = i;
			data->j = j;
			pthread_create(&workers[j + M*i], NULL, calculate_index, (void*)(data));	
		}		
	}

	for(i=0; i<NUM_THREADS ; i++)
		pthread_join(workers[i], NULL);

	for(i=0; i < M; i++){
		for(j=0; j < N; j++){
			printf("%d ", C[i][j]);
		}
		printf("\n");
	}

	return 0;
}
