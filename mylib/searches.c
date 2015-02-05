// Linear search is trivial...

// Binary search recursive
// call bin_search(num, array, 0, n - 1);

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
