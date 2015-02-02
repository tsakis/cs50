#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
#include <unistd.h>
#include <crypt.h>

#define _XOPEN_SOURCE

/* For a general solution
   #define SALT_SIZE 64 * 64
   but since the two first characters of the hash is the salt itself, 
   we don't need that.
*/
#define SALT_SIZE 1
#define SALT_LEN 2 + 1

#define WORD_LEN 8

#define INPUT_FILE "/usr/share/dict/words"
#define my_dict
/*
  Users' passwords, as plaintext, are composed of printable ASCII characters 
  and are no longer than eight characters long.
*/

// Prototypes
int test_password(char * password, char salts[SALT_SIZE][SALT_LEN], char * hash);
char * test_all_passwords(char salts[SALT_SIZE][SALT_LEN], char * hash, char * pass);
char * test_dictionary(char * file_path, char salts[SALT_SIZE][SALT_LEN], 
                       char * hash, char * pass);

void create_salts(char salts[SALT_SIZE][SALT_LEN]);
void print_salt_array(char arr[SALT_SIZE][SALT_LEN], int size);

/* Help for brute force cracking the hashes */
int test_password_all_hashes(char * password);
char * test_all_passwords_all_hashes(char * password);
char * hashes[] = {"50gyRGMzn6mi6", "50q.zrL5e0Sak",
                   "50Bpa7n/23iug", "HAYRs6vZAb4wo"};
#define HASHES_SIZE 4
/* /help */

int main(int argc, string argv[]) {
    
    if (argc != 2) {
        printf("Proper usage:\n%s hash-digest\n", argv[0]);
        return 1;
    }
    
    char * hash = argv[1];

    char salts[SALT_SIZE][SALT_LEN];
    salts[0][0] = hash[0];
    salts[0][1] = hash[1];
    //create_salts(salts);
    //print_salt_array(salts, SALT_SIZE); 
    
    char * pass = "Dumb initialization";
    char * res = test_dictionary(INPUT_FILE, salts, hash, pass);
    
    if (strcmp(pass, res))
        printf("%s\n", res);
    else {
        res = test_all_passwords(salts, hash, pass);
        if (strcmp(pass, res))
            printf("%s\n", res);
    }
    
    //char * res = test_all_passwords_all_hashes(pass);
    return 0;
}

int test_password(char * password, char salts[SALT_SIZE][SALT_LEN], char * hash) {
    char * temp_hash;
    
    printf("Checking... %s.\n", password);
    for (int i = 0; i < SALT_SIZE; i++) {
        temp_hash = crypt(password, salts[i]);
        //printf("%s\n", temp_hash);
        if ( !(strcmp(temp_hash, hash)) )
            return 0;
    }
    return 1;
}

char * test_dictionary(char * file_path, char salts[SALT_SIZE][SALT_LEN], 
                       char * hash, char * pass) {
    int word_max_size = 64;
    int found = 0;
    FILE * fp;
    char * line = malloc(sizeof(char) * word_max_size);
    char * word = NULL;

    fp = fopen(file_path, "r");
    if (fp == NULL)
        return "No free memory";

    while ( fgets(line, word_max_size, fp) ) {
        if (strlen(line) > WORD_LEN || line[0] == '\n')
            continue;
        word = strtok(line, "\n");
        found = !(test_password(word, salts, hash));
        if (found) {
            printf("---FOUND---\n%s\n", line);
            return word;
        }
    }

    free(line);
    fclose(fp);
    return pass;
}

char * test_all_passwords(char salts[SALT_SIZE][SALT_LEN], char * hash, char * pass) {
    // printable ASCII : 32-126
    char * word = NULL;
    char * str_nested[WORD_LEN];
    char * temp = (char *)malloc(sizeof(char) * WORD_LEN);
    int found = 0;

    for (int i = 0; i < WORD_LEN; i++)
        str_nested[i] = (char *)malloc(sizeof(char) * WORD_LEN);
    
    for (int i = 32; i < 127 && found == 0; i++) {
        sprintf(str_nested[0], "%c", i);
        word = str_nested[0];
        found = !(test_password(word, salts, hash));
        for (int j = 32; j < 127 && found == 0; j++) {
            strcpy(str_nested[1], str_nested[0]);
            sprintf(temp, "%c", j);
            word = strcat(str_nested[1], temp);
            found = !(test_password(word, salts, hash));
            for (int k = 32; k < 127 && found == 0; k++) {
                strcpy(str_nested[2], str_nested[1]);
                sprintf(temp, "%c", k);
                word = strcat(str_nested[2], temp);
                found = !(test_password(word, salts, hash));
                for (int l = 32; l < 127 && found == 0; l++) {
                    strcpy(str_nested[3], str_nested[2]);
                    sprintf(temp, "%c", l);
                    word = strcat(str_nested[3], temp);
                    found = !(test_password(word, salts, hash));
                    for (int m = 32; m < 127 && found == 0; m++) {
                        strcpy(str_nested[4], str_nested[3]);
                        sprintf(temp, "%c", m);
                        word = strcat(str_nested[4], temp);
                        found = !(test_password(word, salts, hash));
                        for (int n = 32; n < 127 && found == 0; n++) {
                            strcpy(str_nested[5], str_nested[4]);
                            sprintf(temp, "%c", n);
                            word = strcat(str_nested[5], temp);
                            found = !(test_password(word, salts, hash));
                            for (int o = 32; o < 127 && found == 0; o++) {
                                strcpy(str_nested[6], str_nested[5]);
                                sprintf(temp, "%c", o);
                                word = strcat(str_nested[6], temp);
                                found = !(test_password(word, salts, hash));
                                for (int p = 32; p < 127 && found == 0; p++) {
                                    strcpy(str_nested[7], str_nested[6]);
                                    sprintf(temp, "%c", p);
                                    word = strcat(str_nested[7], temp);
                                    found = !(test_password(word, salts, hash));
                               }
                            }
                        }
                    }
                }
            }
        }
    }
    if (found) {
        printf("---FOUND---\n%s\n", word);
        return word;
    }
    return pass;
}

void create_salts(char salts[SALT_SIZE][SALT_LEN]) {
    // a-z   : 97-122
    // A-Z   : 65-90
    // ./0-9 : 46-57
    int k = 0;
    char temp_str[SALT_LEN] = "\0\0\0";
    for (int i = 46; i <= 122; i++) {
        if ((i > 57 && i < 65) || (i > 90 && i < 97))
            continue;
        temp_str[0] = i;
        for (int j = 46; j <= 122; j++) {
            if ((j > 57 && j < 65) || (j > 90 && j < 97))
                    continue;
            temp_str[1] = j;
            strcpy(salts[k], temp_str);
            k++;
        }
    }
    return;
}

void print_salt_array(char arr[SALT_SIZE][SALT_LEN], int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\t", arr[i]);
    }
    printf("\n");
    return;
}

int test_password_all_hashes(char * password) {
    char * temp_hash;
    char salt[3] = "\0\0\0";
    
    
    //printf("Checking... %s.\n", password);
    for (int i = 0; i < HASHES_SIZE; i++) {
        salt[0] = hashes[i][0];
        salt[1] = hashes[i][1];
        temp_hash = crypt(password, salt);
        //printf("%s\n", temp_hash);
        if ( !(strcmp(temp_hash, hashes[i])) ) {
            FILE * fp = fopen("results.txt", "a");
            if (fp == NULL)
                return -123;
            fprintf(fp, "%s:%s\n", temp_hash, password);
            fclose(fp);
        }
    }
    return 1;
}

char * test_all_passwords_all_hashes(char * pass) {
    // printable ASCII : 32-126
    char * word = NULL;
    char * str_nested[WORD_LEN];
    char * temp = (char *)malloc(sizeof(char) * WORD_LEN);
    int found = 0;

    for (int i = 0; i < WORD_LEN; i++)
        str_nested[i] = (char *)malloc(sizeof(char) * WORD_LEN);
    
    for (int i = 32; i < 127 && found == 0; i++) {
        sprintf(str_nested[0], "%c", i);
        word = str_nested[0];
        found = !(test_password_all_hashes(word));
        printf("%s\n", word);
        for (int j = 32; j < 127 && found == 0; j++) {
            strcpy(str_nested[1], str_nested[0]);
            sprintf(temp, "%c", j);
            word = strcat(str_nested[1], temp);
            found = !(test_password_all_hashes(word));
            for (int k = 32; k < 127 && found == 0; k++) {
                strcpy(str_nested[2], str_nested[1]);
                sprintf(temp, "%c", k);
                word = strcat(str_nested[2], temp);
                found = !(test_password_all_hashes(word));
                for (int l = 32; l < 127 && found == 0; l++) {
                    strcpy(str_nested[3], str_nested[2]);
                    sprintf(temp, "%c", l);
                    word = strcat(str_nested[3], temp);
                    found = !(test_password_all_hashes(word));
                    for (int m = 32; m < 127 && found == 0; m++) {
                        break; //check until here
                        strcpy(str_nested[4], str_nested[3]);
                        sprintf(temp, "%c", m);
                        word = strcat(str_nested[4], temp);
                        found = !(test_password_all_hashes(word));
                        for (int n = 32; n < 127 && found == 0; n++) {
                            strcpy(str_nested[5], str_nested[4]);
                            sprintf(temp, "%c", n);
                            word = strcat(str_nested[5], temp);
                            found = !(test_password_all_hashes(word));
                            for (int o = 32; o < 127 && found == 0; o++) {
                                strcpy(str_nested[6], str_nested[5]);
                                sprintf(temp, "%c", o);
                                word = strcat(str_nested[6], temp);
                                found = !(test_password_all_hashes(word));
                                for (int p = 32; p < 127 && found == 0; p++) {
                                    strcpy(str_nested[7], str_nested[6]);
                                    sprintf(temp, "%c", p);
                                    word = strcat(str_nested[7], temp);
                                    found = !(test_password_all_hashes(word));
                               }
                            }
                        }
                    }
                }
            }
        }
    }
    if (found) {
        printf("---FOUND---\n%s\n", word);
        return word;
    }
    return pass;
}
