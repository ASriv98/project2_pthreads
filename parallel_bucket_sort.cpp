#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <vector>
#include <algorithm>

using namespace std;

#define NUM_THREADS 10
int input_array_size = 100000;
int input_array[100000];
vector<int> bucket[NUM_THREADS];
#define MAX_RANDOM_NUMBER 10000
pthread_barrier_t barrier;

void *parallel_bucketSort(void* void_thread_id_ptr)
{
	int *thread_id = (int *) void_thread_id_ptr;
	int start_point, end_point;

	start_point = (*thread_id * MAX_RANDOM_NUMBER)/NUM_THREADS;
	printf("\n%d", start_point);
	end_point = (((*thread_id+1) * MAX_RANDOM_NUMBER)/NUM_THREADS);
	printf(" %d \n", end_point);

	for(int i = 0; i < input_array_size; i++)
	{
		if ((input_array[i] <= end_point) && (input_array[i] >= start_point))
		{
			bucket[*thread_id].push_back(input_array[i]);
		}
	}
	std::sort(bucket[*thread_id].begin(), bucket[*thread_id].end());

	pthread_barrier_wait(&barrier);

	vector<int> sorted_list;
	if (*thread_id == 0){
		for (int i = 0; i < NUM_THREADS; i+= 1)
		{	
			sorted_list.insert(sorted_list.end(), bucket[i].begin(), bucket[i].end());
		}

		for (int i = 0; i < input_array_size; i+= 1)
		{	
			input_array[i] = sorted_list[i];
		}
	}
	
}

int main()
{
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);
	pthread_t thread[NUM_THREADS];
	int tNum[NUM_THREADS];

	for (int i = 0; i < input_array_size; i++) {
		int random_integer = rand() % MAX_RANDOM_NUMBER;
		input_array[i] = random_integer;
	}

	clock_t start = clock(), diff;
	printf("\n Array Size: %d ", input_array_size);
	
	for (int thread_id = 0; thread_id < NUM_THREADS; thread_id++){
		tNum[thread_id] = thread_id;
		pthread_create(&thread[thread_id], NULL, parallel_bucketSort, &tNum[thread_id]);

	}

	for (int thread_id = 0; thread_id < NUM_THREADS; thread_id++){
		 pthread_join(thread[thread_id], NULL);
	}
	int s;
	diff = clock() - start;
	int milli_sec = diff * 1000 / CLOCKS_PER_SEC;

	for (int i = 0; i < input_array_size; i++) {
		printf("%d ", input_array[i]);
	}
	printf("Time taken: %d seconds %d milliseconds\n", milli_sec/1000, milli_sec%1000);

	return 0;
}


