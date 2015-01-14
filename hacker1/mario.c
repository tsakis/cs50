#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>

int print_half_pyramid(int height) {
	char * next_char;
	int depth = 2 * height + 2;	
    
    for (int i = 0; i < height; i++) { 
        for (int j = 0; j < depth; j++) {
            if (j < height)
			    // first half-pyramid
				next_char = (j < height - 1 - i) ? " " : "#";
			else if (j > height + 1) {
				// second half-pyramid
				next_char = (j > height + 2 + i) ? " " : "#";
                if (strcmp(next_char," ") == 0)
                    break;
            }
			else
				// middle part
				next_char = " ";
			printf("%s", next_char);
        }
        printf("\n");
	}
	return 0;
}

int fun_pyramids() {
    for (int i = 0; i < 25; i++)
        print_half_pyramid(i);
    return 0;
}

int main() {
	int height;
	do {
		printf("Give a number in [0,23].\n");
		height = GetInt();
	} while (height < 0 || height > 23);

	print_half_pyramid(height);
    //fun_pyramids();
	return 0;
}
