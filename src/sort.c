//making a radix sort in C

//mini function to get the max real quick
int getMax(int array[], int n){
    int max = array[0];
    for (int i = 1; i < n;i++)
        if(array[i] > max)
        max = array[i];
    return max;
}

//function to do counting sort of array
void countSort