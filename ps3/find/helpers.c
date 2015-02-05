/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

// binary search recursive
bool bin_search(int value, int values[], int lo, int hi);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // naive linear search algorithm
    if (n <= 0)
        return false;

    return bin_search(value, values, 0, n - 1);
}

bool bin_search(int value, int values[], int lo, int hi)
{
    int mid = lo + (hi - lo) / 2;

    if (lo > hi)
        return false;
    else if (value == values[mid])
        return true;
    else if (value < values[mid])
        return bin_search(value, values, lo, mid - 1);
    else if (value > values[mid])
        return bin_search(value, values, mid + 1, hi);
    else {
        printf("WOW how did we come here?\n");
        return false;
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Insertion sort: O(n^2) sorting algorithm
    int element, j;
    for (int i = 0; i < n; i++) {
        // traverse unsorted portion l-r
        j = i;
        element = values[j];
        while (j > 0 && element < values[j-1]) {
            // traverse already sorted portion r-l
            // until right element position is found
            values[j] = values[j-1];
            j--;
        }
        values[j] = element;
    }
    return;
}

/**
 * Prints array.
 */
void print(int values[], int n)
{
    if (n < 0)
        return;

    for (int i = 0; i < n; i++)
        printf("%d\t", values[i]);
    return;
}

