#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void) {

    string name;
    string token = NULL;
    string delimiter = " ";
    
    //printf("Give your name\n");
    name = GetString();

    while ((token = strtok(name, delimiter)) != NULL) {
        name = NULL;
        printf("%c", toupper(token[0]));
    }
    printf("\n");
    return 0;
}
