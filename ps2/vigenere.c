#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

// Prototypes
int not_word(char * word, int len);
int * alpha_to_key(char * word, int * kw, int len);
char * vigenere(int * k, int kw_len, char * in);

int main(int argc, string argv[]) {
    
    if (argc != 2) {
        printf("Proper usage:\n%s keyword\n", argv[0]);
        return 1;
    }
    
    int kw_len = strlen(argv[1]);
    if (not_word(argv[1], kw_len)) {
        printf("Proper usage:\n%s keyword\nwhere keyword "
                "only contains letters\n", argv[0]);
        return 1;
    }

    int * kw = malloc(sizeof(*kw) * kw_len);
    kw = alpha_to_key(argv[1], kw, kw_len);

    //printf("Write some plaintext now!\n");
    char * s = vigenere(kw, kw_len, GetString());
    printf("%s\n", s);
    
    free(kw);
    free(s);
    return 0;
}

// Checks if word contains non-letter characters
int not_word(char * word, int len) {
    for (int i = 0; i < len; i++)
        if (!(isalpha(word[i])))
            return 1;
    return 0;
}

// Converts a char word to the equivalent integer keyword
int * alpha_to_key(char * word, int * kw, int len) {
    for (int i = 0; i < len; i++) {
        if (isupper(word[i]))
            word[i] = tolower(word[i]);
        kw[i] = word[i] - 'a';
    }
    return kw;
}

// Implements Vigenere cipher
// @param int*  keyword integers
// @param int   keyword length
// @param char* input string
// @ret   char* ciphered string
char * vigenere(int * k, int kw_len, char * in) {
    char char_out, char_in;
    int n = strlen(in);
    char * out = malloc(sizeof(out) * n);
    int kw_i = 0;

    for (int i = 0; i < n; i++) {
        char_in = in[i];
        if (char_in >= 'a' && char_in <= 'z') {
            char_out = ((char_in - 'a') + k[kw_i]) % 26 + 'a';
            kw_i = (kw_i + 1) % kw_len;
        }
        else if (char_in >= 'A' && char_in <= 'Z') {
            char_out = ((char_in - 'A') + k[kw_i]) % 26 + 'A';
            kw_i = (kw_i + 1) % kw_len;
        }
        else {
            char_out = char_in;
        }
        out[i] = char_out;
    }
    return out;
}
