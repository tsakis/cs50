#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int print_half_pyramid(int depth) {
	char * next_char;	
	// for depth 8 we have 2-9 hashes
        for (int i = 0; i < depth; i++) { 
                for (int j = 0; j < depth + 1; j++) {
                        next_char = (j > depth - 2 -i) ? "#" : " ";
                        printf("%s", next_char);
                }
                printf("\n");
	}
	return 0;
}

int main() {
/*
Python:
	for i in range(8):
    	    print (" " * (9-2-i)) + ("#" * (i+2))
*/
	int depth;
	do {
		printf("Give a number in [0,23].\n");
		depth = GetInt();
		//if(scanf("%d%c", &depth, &term) == 2 && term == '\n')
		//	read_int = 1;
	} while (depth < 0 || depth > 23);

	print_half_pyramid(depth);
	return 0;
}
