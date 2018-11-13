#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>



// All sorts should sort in ascending order and take an int[] array and int array_size as input
int compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}
// sort_test function takes in a sorting function as input and returns true if it correctly sorts
char sorted_test(void(*function)(int[], int)) {
	int test_array[10], sorted_array[10];
	int array_size = 10;

	for (int i = 0; i < array_size; i++) {
		int random_integer = rand() % 100;
		test_array[i] = random_integer;
		sorted_array[i] = random_integer;
	}

	qsort(sorted_array, array_size, sizeof(int), compare);
	(*function)(test_array, array_size);

	for (int i = 0; i < array_size; ++i) {
		if (test_array[i] != sorted_array[i])
			return 0;
	}
	return 1;
}

// Outputs run time for different array input sizes
void complexity_test(void(*function)(int[], int)) {

	for (int array_size = 10; array_size < 10001; array_size = array_size * 10) {
		int *test_array = malloc(array_size*sizeof(*test_array));
		for (int i = 0; i < array_size; i++) {
			int random_integer = rand() % 50;
			test_array[i] = random_integer;
		}
		//reset_and_start_timer();
		clock_t start = clock(), diff;
		(*function)(test_array, array_size);
		//double dt = get_elapsed_mcycles();
		diff = clock() - start;
		int milli_sec = diff * 1000 / CLOCKS_PER_SEC;
		printf("Array Size: %d", array_size);
		printf("Time taken: %d seconds %d milliseconds\n", milli_sec/1000, milli_sec%1000);
	}
}

void sequential_bubbleSort(int array[], int array_size)
{
	int j = 0;
	int temp;
	char early_break_flag = 1;

	while (early_break_flag)
	{
		j++;
		early_break_flag = 0;

		for (int i = 0; i < array_size - j; i++)
		{
			if (array[i] > array[i + 1])
			{
				temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;
				early_break_flag = 1;
			}
		}
	}
}

void sequential_merge(int array[], int left, int middle, int right){

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

void sequential_merge_sort(int array[], int left, int right)
{
	int middle = (left + right - 1) / 2;

	if (left < right)
	{
	sequential_merge_sort(array, left, middle);
	sequential_merge_sort(array, middle+1, right);
	sequential_merge(array, left, middle, right);
	}
	
}

void sequential_merge_sort_wrapper(int array[], int array_size){
	sequential_merge_sort(array, 0, array_size-1);
}

void sequential_bucketSort(int array[], int array_size){

	int max = array[0];

	for (int c = 1; c < array_size; c++)
	{
		if(array[c] > max)
			max = array[c];
		c++;
	}
	max = max + 1;
	int counter[max];
	for(int i = 0; i < max; i++)
		counter[i] = 0;
	for(int i = 0; i < array_size; i++)
		(counter[array[i]])++;
	for(int i=0, j=0; i < max; i++)
		for(; counter[i] > 0; (counter[i])--){
			array[j++] = i;}
}



void parallel_bubble_sort(){
	
}






int main()
{
	if (sorted_test(sequential_bubbleSort)) {
		printf("Sequential bubble sort passes testing\n");
		complexity_test(sequential_bubbleSort);
	}
	else
		printf("Sequential bubble sort fails testing\n");

	if (sorted_test(sequential_merge_sort_wrapper)) {
		printf("Sequential merge sort passes testing\n");
		complexity_test(sequential_merge_sort_wrapper);
	}
	else
		printf("Sequential merge sort fails testing\n");

	if (sorted_test(sequential_bucketSort)) {
		printf("Sequential bucket sort passes testing\n");
		complexity_test(sequential_bucketSort);
	}
	else
		printf("Sequential bucket sort fails testing\n");

	return 0;
}
