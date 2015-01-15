#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

/*
---AMEX
LEN = 15
STW = 34, 37

---MASTERCARD
LEN = 16
STW = 51..55

---VISA
LEN = 13, 16
STW = 4
*/

#define MAX_POW 16

struct tuple {
    int * array;
    int size;
};

long long read_number();
int max_size_ok(long long number);
struct tuple get_digits(long long number);
int print_array(int * array, int size);
char * check_type(int * array, int size);
char * check_luhn(int * array, int size, char * result);

int main(void) {
    long long ccn;
    int * num_digits = NULL;
    int size;
    struct tuple digits_and_size;
    char * result = "INVALID\n";

    ccn = read_number();
    if (max_size_ok(ccn)) {
        digits_and_size = get_digits(ccn);
        num_digits = digits_and_size.array;
        size = digits_and_size.size;
        //  print_array(num_digits, size);
        if (strcmp(result = check_type(num_digits, size), "INVALID\n") != 0) {
            result = check_luhn(num_digits, size, result);
        }
    }
    printf("%s", result);
    free(num_digits);
    return 0;
}

// Read input credit card number.
long long read_number() {
    long long number;
    
    do {
        printf("Give your credit card number with no spaces or hyphens.\n");
        number = GetLongLong();
    } while (number <= 0);

    return number;
}

// Check size of number to be 16 max.
int max_size_ok(long long number) {
    if (number >= pow(10, MAX_POW + 1))
        return 0;
    else
        return 1;
}

// Get the digits of the number one-by-one and its size.
struct tuple get_digits(long long number) {
    int i = 0;
    int count_digits = 0;
    long long residue = number;
    
    int max_digits[MAX_POW];
    int * digits = NULL;

    for (i = 0; (i < MAX_POW) && (residue > 0); i++) {
        max_digits[i] = residue % 10;
        residue = (residue - max_digits[i]) / 10; 
        // printf("d=%d\tres=%lld\n", max_digits[i], residue);
        count_digits = i + 1;
    }

    digits = malloc(sizeof(*digits) * count_digits);
    for (i = 0; i < count_digits; i++)
        digits[i] = max_digits[i];
    
    struct tuple digits_size = {digits, count_digits};
    return digits_size;
}

// Print an array of specific size
int print_array(int * array, int size) {
    for (int i = 0; i < size; i++)
        printf("%d", array[i]);
    printf("array\n");
    return 0;
}

// Check first digits to identify the type of credit card.
char * check_type(int * array, int size) {
    char * result = NULL;
    
    if ((size == 13 || size == 16) && (array[size - 1] == 4))
        result = "VISA\n";
    else if (size == 16 && array[size - 1] == 5 && (array[size - 2] >= 1 && array[size - 2] <= 5))
        result = "MASTERCARD\n";
    else if (size == 15 && array[size - 1] == 3 && (array[size - 2] == 4 || array[size - 2] == 7))
        result = "AMEX\n";
    else
        result = "INVALID\n";
    return result;
}

// Check Luhn algorithm to verify the checksum.
char * check_luhn(int * array, int size, char * result) {
    int i = 0;
    int product = 0;
    int sum1 = 0;
    int sum2 = 0;
    
    for (i = 0; i < size; i++) {
        if (i % 2 == 1) {
            product = array[i] * 2;
            if (product >= 10)
                 sum1 += (product % 10) + 1;
            else
                 sum1 += product;
        }
        else {
            sum2 += array[i];
        }
    }
    if ((sum1 + sum2) % 10 != 0)
        result = "INVALID\n";
    
    return result;
}
