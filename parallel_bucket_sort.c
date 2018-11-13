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

