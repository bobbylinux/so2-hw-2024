#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_WORDS 10000
#define MAX_WORD_SIZE 30
#define EXCLAMATION_MARK '!'
#define QUESTION_MARK '?'
#define DOT '.'
#define SPACE ' '
#define END_LINE '\n'
#define TAB '\t'
#define EMPTY_CHAR '\0'
#define COLON ';'
#define COMMA ','
#define SEMICOLON ';'

struct word_element *pointer;

struct next_word_element {
    char word[30];
    double occurrences;
};

struct word_element {
    char word[30];
    struct next_word_element* next_word;
    struct word_element* next_element;
};

char **read_input(char *input_file_name);

char *to_lower(char *str);

int main(int argv, char *args[]) {
    char **words = read_input("input.txt");
    for (int i = 0; words[i] != NULL; i++) {
        printf("Word %d: %s\n", i + 1, words[i]);
        free(words[i]);
    }
    free(words);
    exit(EXIT_SUCCESS);
}

char **read_input(char *input_file_name) {
    int chr, idx_letter = 0, idx_word = 0;
    char *word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
    char **words = (char **) malloc(sizeof(char *) * MAX_WORDS);

    FILE *fp = fopen(input_file_name, "r");
    if (!fp) {
        perror("Error trying to open input file\n");
        exit(EXIT_FAILURE);
    }

    while ((chr = fgetc(fp)) != EOF) {
        if (chr == SPACE || chr == QUESTION_MARK || chr == EXCLAMATION_MARK || chr == DOT || chr == END_LINE || chr == TAB) {
            word[idx_letter] = EMPTY_CHAR;
            idx_letter = 0;
            if (idx_word < MAX_WORDS && word[0] != EMPTY_CHAR && word[0] != END_LINE && word[0] != TAB) {
                word = to_lower(word);
                words[idx_word] = strdup(word);
                idx_word++;
                word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
                continue;
            }
            if (idx_word < MAX_WORDS && chr == QUESTION_MARK || chr == EXCLAMATION_MARK || chr == DOT) {
                word[0] = (char) chr;
                word[1] = EMPTY_CHAR;
                words[idx_word] = strdup(word);
                idx_word++;
                word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
                continue;
            }
        }

        if (idx_letter < MAX_WORD_SIZE - 1) {
            word[idx_letter] = (char) chr;
            idx_letter++;
        }
    }

    word[idx_letter] = EMPTY_CHAR;

    if (idx_word < MAX_WORDS && chr != EMPTY_CHAR && chr != COMMA && chr != COLON && chr != SEMICOLON) {
        word = to_lower(word);
        words[idx_word] = strdup(word);
        idx_word++;
    }

    words[idx_word] = NULL;

    fclose(fp);

    return words;
}

char *to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char) str[i]);
    }
    return str;
}

struct word_element* add_array_to_list(char* word, struct word_element* head) {

}