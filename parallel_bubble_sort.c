#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 10
int input_array_size = 1000;
int input_array[1000];
pthread_barrier_t barrier;

void *parallel_bubbleSort(void* void_thread_id_ptr)
{
	int *thread_id = (int *) void_thread_id_ptr;
	int j = 0;
	int temp;
	char early_break_flag = 1;
	int start_point, end_point;

	// INSERT START AND END CALC
	start_point = (*thread_id * input_array_size)/NUM_THREADS;
	printf("\n%d", start_point);
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
	printf("\nThread %d finished\n", *thread_id);
	pthread_barrier_wait(&barrier);

	if (*thread_id == 0){
		for (int i = input_array_size/NUM_THREADS; i < input_array_size; i+= input_array_size/NUM_THREADS)
		{
			int combine_end_point = i + input_array_size/NUM_THREADS; 
			combine(input_array, start_point, i, combine_end_point);
		}
			printf("Finished combining\n");

	}

}

void combine(int array[], int left, int middle, int right){

	int size_left  = middle - left + 1;
	int size_right = right - middle;

	int array_left_half[size_left];
	int array_right_half[size_right];

	for (int i = 0; i < size_left; i++)
	{
        array_left_half[i] = array[left + i]; 
	}
    for (int j = 0; j < size_right; j++)
    {
        array_right_half[j] = array[middle + 1 + j]; 
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < size_left && j < size_right) 
    { 
        if (array_left_half[i] <= array_right_half[j]) 
        { 
            array[k] = array_left_half[i]; 
            i++; 
            k++;
        } 
        else
        { 
            array[k] = array_right_half[j]; 
            j++;
            k++; 
        } 
    } 
 
    while (i < size_left) 
    { 
        array[k] = array_left_half[i]; 
        i++; 
        k++; 
    } 
  
    while (j < size_right) 
    { 
        array[k] = array_right_half[j]; 
        j++; 
        k++; 
    } 

}

int main()
{
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);
	pthread_t thread[NUM_THREADS];
	int tNum[NUM_THREADS];

	for (int i = 0; i < input_array_size; i++) {
		int random_integer = rand() % 10000;
		input_array[i] = random_integer;
	}

	clock_t start = clock(), diff;
	printf("\n Array Size: %d ", input_array_size);
	
	for (int thread_id = 0; thread_id < NUM_THREADS; thread_id++){
		tNum[thread_id] = thread_id;
		pthread_create(&thread[thread_id], NULL, parallel_bubbleSort, &tNum[thread_id]);

	}

	for (int thread_id = 0; thread_id < NUM_THREADS; thread_id++){
		 pthread_join(thread[thread_id], NULL);
	}
	int s;
	diff = clock() - start;
	int milli_sec = diff * 1000 / CLOCKS_PER_SEC;

	// for (int i = 0; i < input_array_size; i++) {
	// 	printf("%d ", input_array[i]);
	// }
	printf("Time taken: %d seconds %d milliseconds\n", milli_sec/1000, milli_sec%1000);

	return 0;
}
