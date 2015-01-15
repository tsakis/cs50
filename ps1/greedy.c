#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cs50.h>

int euro_coins[] = {2, 1};
size_t euro_coins_size = sizeof(euro_coins) / sizeof(euro_coins[0]);

int cent_coins[] = {50, 20, 10, 5, 2, 1};
size_t cent_coins_size = sizeof(cent_coins) / sizeof(cent_coins[0]);

int dollar_coins[] = {25, 10, 5, 1};
size_t dollar_coins_size = sizeof(dollar_coins) / sizeof(dollar_coins[0]);

float input_euros() {
	float in = 0;
	int c;

	do {
		printf("How much change in euros?\n");
		if (scanf("%f", &in) != 1) {
			while ((c = getchar()) != EOF && c != '\n') {
				printf("%d,", c);
			}
			continue;
		}
	} while (in <= 0);
	return in;
}

float input_dollars() {
    float in = 0;
    do {
        printf("How much change in dollars?\n");
        in = GetFloat();
    } while (in < 0);
    return in;
}

int number_of_coins(int * coins_array, size_t array_size, int amount) {
	int residue = amount;
	int count = 0;
	
	for (int i = 0; (i < array_size) && (residue > 0); i++) {
		count += residue / coins_array[i];
		residue = residue % coins_array[i];
	}
	return count;
}

int calculate_euros(float euro_input) {
	int euros, cents, e, c;
	
	euros = trunc(euro_input);
	cents = (int) (roundf((euro_input - euros) * 100));
	printf("%d and %d\n", euros, cents);
	
    printf("Euro coins:\t%d\n", e = number_of_coins(euro_coins, euro_coins_size, euros));
	printf("Cent coins:\t%d\n", c = number_of_coins(cent_coins, cent_coins_size, cents));
	printf("Total coins:\t%d\n", e + c);
    return 0;
}

int calculate_dollars(float dollar_input) {
    int dollars, cents, total;
    
    dollars = truncf(dollar_input);
    cents = (int) (roundf((dollar_input - dollars) * 100));
    total = dollars * 100 + cents;
    
    //printf("Now in dollars!\n");
    printf("%d\n", number_of_coins(dollar_coins, dollar_coins_size, total));
    return 0;
}

int main(void) {
    float in;
	
    // in = input_euros();
    // calculate_euros(in);
    in = input_dollars();
    calculate_dollars(in);
    return 0;
}

