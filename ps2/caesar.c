#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

// Prototypes
char * caesar(int k, string in);

int main(int argc, string argv[]) {
    int k = 0;

    if (argc != 2 || ((k = atoi(argv[1])) <= 0)) {
        printf("Proper usage:\n%s k\nwhere k is an integer k > 0\n", argv[0]);
        return 1;
    }
    //printf("Write some plaintext now!\n");
    char * s = caesar(k, GetString());
    printf("%s\n", s);
    
    free(s);
    return 0;
}

// Implements Caesar cipher
// @param int   key
// @param char* input string
// @ret   char* ciphered string
char * caesar(int k, char * in) {
    char char_out, char_in;
    int n = strlen(in);
    char * out = malloc(sizeof(out) * n);
    
    for (int i = 0; i < n; i++) {
        char_in = in[i];
        if (char_in >= 'a' && char_in <= 'z') {
            char_out = ((char_in - 'a') + k) % 26 + 'a';
        }
        else if (char_in >= 'A' && char_in <= 'Z') {
            char_out = ((char_in - 'A') + k) % 26 + 'A';
        }
        else {
            char_out = char_in;
        }
        out[i] = char_out;
    }
    return out;
}

