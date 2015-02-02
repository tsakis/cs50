#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

int random_generator(int a[SIZE]);
void print_array(int a[SIZE]);
void swap(int * x, int * y);
void selection_sort(int a[SIZE]);
void bubble_sort(int a[SIZE]);
void insertion_sort(int a[SIZE]);
void quicksort(int a[SIZE], int lo, int hi);
int qsort_partition(int a[], int lo, int hi);

int main(int argc, char * argv[]) {

    int a[SIZE];
    a[0] = random_generator(a);
    print_array(a); 
    //selection_sort(a);
    //bubble_sort(a);
    //insertion_sort(a);
    quicksort(a, 0, SIZE);
    printf("-------------\n");
    print_array(a);
    return 0;
}

void selection_sort(int a[SIZE]) {
    /* 
       Traverse the whole list n times, doing
       linear search for the minimum of the unsorted list each time
       Put the minimum to its correct position in the sorted list (swap)
       O(n^2)
       Omega(n^2)
       method -selection
    */
    printf("SELECTION\n");
    int min, min_pos;
    for (int i = 0; i < SIZE - 1; i++) {
        min = a[i];
        min_pos = i;
        for (int j = i + 1; j < SIZE; j++) {
            if (a[j] < min) {
                min = a[j];
                min_pos = j;
            }
        }
        if (i != min_pos)
            swap(&a[i], &a[min_pos]);
        print_array(a);
    }
    return;
}

void bubble_sort(int a[SIZE]) {
    /* 
       Traverse the whole list n times, doing
       n comparisons between neighbouring element and swapping them
       The largest element bubbles up to the end of the list
       O(n^2)
       Omega(n)
       method - exchange
    */
    printf("BUBBLE\n");
    int swapped;
    for (int i = 0; i < SIZE - 1 && swapped; i++) {
        swapped = 0;
        for (int j = 0; j < SIZE - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                swap(&a[j], &a[j + 1]);
                swapped = 1;
            }
        }
        print_array(a);
    }
    return;
}

void insertion_sort(int a[SIZE]) {
    /* 
       Divide the list to the sorted (left) and unsorted (right) portion.
       Traverse the unsorted portion n times, doing comparisons with the 
       elements of the sorted portion and shift them to the right when
       we find the correct position for the element in the sorted portion.
       We traverse the sorted portion from right to left, and the
       unsorted portion normally from left to right.
       O(n^2)
       Omega(n)
       method - insertion
    */
    printf("INSERTION\n");
    int element, j;
    for (int i = 0; i < SIZE; i++) {
        element = a[i];
        j = i;
        while (j > 0 && element < a[j-1]) {
            a[j] = a[j-1];
            j--;
        }
        a[j] = element;
        print_array(a);
    }
    return;
}

void quicksort(int a[SIZE], int lo, int hi) {
    /* 
       Divide and conquer algorithm.
       
       qsort_partition:
       Get a pivot (here last element of array).
       Compare all elements right of the wall with the pivot.
       If lesser than pivot, move element to the left of the wall
       and move the wall one place to the right.
       This way, after each qsort_partition, all the elements < pivot
       are to the left of it, all > pivot are to the right of it and
       pivot is in the right position.

       Apply same algorithm recursively for each left/right sub-array
       until one-element arrays.

       ** Watch out for the indices (inclusive or not..)
       O(n^2)
       Omega(n logn)
       method - partitioning
    */
    int part = 0;
    
    if (lo < hi) {
        part = qsort_partition(a, lo, hi);
        print_array(a);
        quicksort(a, lo, part);
        quicksort(a, part + 1, hi);
    }
    return;

}

int qsort_partition(int a[], int lo, int hi) {
    int pivot = a[hi - 1]; // last as pivot - simple bad choice, try median
    int wall = lo;

    for (int i = lo; i < hi; i++) {
        if (a[i] < pivot) {
            swap(&a[i], &a[wall]);
            wall++;
        }
    }
    swap(&a[hi - 1], &a[wall]);
    return wall;
}


void swap(int * x, int * y) {
    int temp = *x;
    *x = *y;
    *y = temp;
    return;
}

int random_generator(int a[SIZE]) {
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        a[i] = rand() % 50;
    }
    return a[0];
}

void print_array(int a[SIZE]) {
    for (int i = 0; i < SIZE; i++)
        printf("%d\t", a[i]);
    printf("\n");
}
