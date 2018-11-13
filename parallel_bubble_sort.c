#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 1000

int input_array_size = 10000;
int input_array[10000];

void *parallel_bubbleSort(void* void_thread_id_ptr)
{
	int *thread_id = (int *) void_thread_id_ptr;
	int j = 0;
	int temp;
	char early_break_flag = 1;
	int start_point, end_point;

	// INSERT START AND END CALC
	start_point = (*thread_id * input_array_size)/NUM_THREADS;
	printf("%d", start_point);
	end_point = ((*thread_id+1) * input_array_size)/NUM_THREADS;
	printf(" %d \n", end_point);

	while (early_break_flag)
	{
		j++;
		early_break_flag = 0;

		for (int i = start_point; i < end_point - j; i++)
		{
			if (input_array[i] > input_array[i + 1])
			{
				temp = input_array[i];
				input_array[i] = input_array[i + 1];
				input_array[i + 1] = temp;
				early_break_flag = 1;
			}
		}
	}
}

int main()
{

	pthread_t thread[NUM_THREADS];
	int tNum[NUM_THREADS];

	for (int i = 0; i < input_array_size; i++) {
		int random_integer = rand() % 10000;
		input_array[i] = random_integer;
	}

	for (int thread_id = 0; thread_id < NUM_THREADS; thread_id++){
		tNum[thread_id] = thread_id;
		pthread_create(&thread[thread_id], NULL, parallel_bubbleSort, &tNum[thread_id]);

	}

	for (int thread_id = 0; thread_id < NUM_THREADS; thread_id++){
		 pthread_join(thread[thread_id], NULL);
	}

	for (int i = 0; i < input_array_size; i++) {
		printf("%d ", input_array[i]);
	}

	return 0;
}
