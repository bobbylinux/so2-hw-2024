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

struct next_word_element {
    char word[30];
    int count;
    double occurrences;
};

struct word_element {
    char word[30];
    int count;
    struct next_word_element* next_words;
    struct word_element *next_element;
};

char **read_input(char *input_file_name);

struct word_element *add_element_to_list(struct word_element* head, char *word, char *next_word);

struct word_element *search_word(struct word_element* head, const char *key);

char *to_lower(char *str);

int main(int argv, char *args[]) {
    char **words = read_input("input.txt");
    struct word_element* words_list = NULL;
    int j = 1;
    for (int i = 0; words[i] != NULL; i++) {
        if (words[j] == NULL) {
            j = 0;
        }
        printf("Word %d: %s\n", i + 1, words[i]);
        add_element_to_list(&words_list, words[i], words[j]);
        free(words[i]);
        j++;
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
        if (chr == SPACE || chr == QUESTION_MARK || chr == EXCLAMATION_MARK || chr == DOT || chr == END_LINE ||
            chr == TAB) {
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

struct word_element *add_element_to_list(struct word_element* head, char *word, char *next_word) {
    struct word_element* result = search_word(head, word);
    if (result == NULL) { //aggiunto la parola se non la trovo
        struct next_word_element* next_word_element = (struct next_word_element*)malloc(sizeof(struct next_word_element));
        if (next_word_element == NULL) {
            fprintf(stderr, "Error trying to allocate new list element\n");
            exit(EXIT_FAILURE);
        }

        strncpy(next_word_element->word, next_word, sizeof(next_word_element->word));
        next_word_element->count = 1;
        next_word_element->occurrences = 0;
        struct word_element* word_element = (struct word_element*)malloc(sizeof(struct word_element));
        if (word_element == NULL) {
            fprintf(stderr, "Error trying to allocate new list element\n");
            exit(EXIT_FAILURE);
        }
        strncpy(word_element->word, word, sizeof(word_element->word));

        word_element->count = 1;
        word_element->next_words = next_word_element;

        word_element->next_element = head;
        head = word_element;
    }
    return head;
}

struct word_element *search_word(struct word_element* head, const char *key) {
    while (head != NULL) {
        if (strcmp(head->word, key) == 0) {
            return head;
        }
        head = head->next_element;
    }
    return NULL;
}