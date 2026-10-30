//making a radix sort in C
#include <stdio.h> 

//mini function to get the max real quick
int getMax(int array[], int n){
    int max = array[0];
    for (int i = 1; i < n;i++)
        if(array[i] > max)
        max = array[i];
    return max;
}

//function to do counting sort of array
void countSort(int arr[], int n, int exp) {
    int output[n]; // Output array
    int count[10] = {0}; // Initialize count array as 0

    // Store count of occurrences in count[]
    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    // Change count[i] so that count[i] now 
    // contains actual position of this digit
    // in output[]
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Copy the output array to arr[], 
    // so that arr[] now contains sorted 
    // numbers according to current digit
    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}


void radixSort(int arr[], int n) {
  
    // Find the maximum number to know 
    // the number of digits
    int m = getMax(arr, n); 

    // Do counting sort for every digit
    // exp is 10^i where i is the current 
    // digit number
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

// A utility function to print an array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}