#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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
		int random_integer = rand() % 50;
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

void sequential_bubbleSort(int *array, int array_size)
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

int main()
{
	if (sorted_test(sequential_bubbleSort)) {
		printf("Sequential bubble sort passes testing\n");
		complexity_test(sequential_bubbleSort);
	}
	else
		printf("Sequential bubble sort fails testing\n");

	return 0;
}
