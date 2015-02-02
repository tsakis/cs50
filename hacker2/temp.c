#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>

#define _XOPEN_SOURCE

void main(void) {

    char * str = (char *)malloc(sizeof(char) * 8);
    char * str2 = (char *)malloc(sizeof(char) * 8);
    char * word = (char *)malloc(sizeof(char) * 8 * 2);
    sprintf(str, "%c", 66);
    printf("%s\n", str);
    
    sprintf(str2, "%c", 99);
    printf("%s\n", str2);
    
    word = strcat(str, str2);
    printf("%s + %s = %s\n", str, str2, word);

    char * pass = "gunshot";
    char * salt = "32";
    char * hash = crypt(pass, salt);
    printf("%s\n", hash);
}
