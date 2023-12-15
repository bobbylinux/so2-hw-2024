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
    struct next_word_element *next_element;
};

struct word_element {
    char word[30];
    int count;
    struct next_word_element *next_words;
    struct word_element *next_element;
};

char **read_input(char *input_file_name);

struct next_word_element *create_next_word_element(const char *word);

struct word_element *create_word_element(const char *word);

void add_or_update_element(struct word_element **head, const char *word, const char *next_word);

char *to_lower(char *str);

void free_next_word_element(struct next_word_element *element);

void free_list(struct word_element *head);

int main(int argv, char *args[]) {
    char **words = read_input("input.txt");
    // Testa della lista concatenata
    struct word_element *head = NULL;

    int j = 1;
    for (int i = 0; words[i] != NULL; i++) {
        if (words[j] == NULL) {
            j = 0;
        }
        printf("Word %d: %s\n", i + 1, words[i]);
        add_or_update_element(&head, words[i], words[i + 1]);
        free(words[i]);
        j++;
    }
    free_list(head);
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

// Funzione per inizializzare una nuova struttura next_word_element
struct next_word_element *create_next_word_element(const char *word) {
    struct next_word_element *next_word_element = (struct next_word_element *) malloc(sizeof(struct next_word_element));
    if (next_word_element == NULL) {
        perror("Error trying to allocate new element on list\n");
        exit(EXIT_FAILURE);
    }
    strcpy(next_word_element->word, word);
    next_word_element->count = 1;
    next_word_element->occurrences = 0.0;

    return next_word_element;
}

// Funzione per inizializzare una nuova struttura word_element
struct word_element *create_word_element(const char *word) {
    struct word_element *word_element = (struct word_element *) malloc(sizeof(struct word_element));
    if (word_element == NULL) {
        perror("Error trying to allocate new element on list\n");
        exit(EXIT_FAILURE);
    }
    strcpy(word_element->word, word);
    word_element->count = 1;
    word_element->next_words = NULL;
    word_element->next_element = NULL;
    return word_element;
}

//Aggiunge la parola alla lista concatenata o l'aggiorna con le parole successive.
void add_or_update_element(struct word_element **head, const char *word, const char *next_word) {
    struct word_element *current = *head;
    struct word_element *previous = NULL;

    // Cerca se la parola è già presente nella lista
    while (current != NULL && strcmp(current->word, word) != 0) {
        previous = current;
        current = current->next_element;
    }

    if (current == NULL) {
        // Se la parola non è presente, aggiungi un nuovo elemento alla lista
        struct word_element *new_element = create_word_element(word);
        if (new_element != NULL) {
            // Aggiungi la parola successiva alla nuova struttura next_word_element
            new_element->next_words = create_next_word_element(next_word);

            // Collega il nuovo elemento alla lista
            if (previous == NULL) {
                // Se la lista è vuota, imposta il nuovo elemento come testa
                *head = new_element;
            } else {
                previous->next_element = new_element;
            }
        }
    } else {
        // Se la parola è già presente, cerca la parola successiva nell'array next_word_element
        struct next_word_element *next_words = current->next_words;
        while (next_words != NULL && strcmp(next_words->word, next_word) != 0) {
            next_words = next_words->next_element;
        }
        if (next_words == NULL) {
            // Se la parola successiva non è presente, aggiungi una nuova struttura next_word_element
            next_words = create_next_word_element(next_word);
            if (next_words != NULL) {
                // Aggiungi la nuova struttura next_word_element alla lista
                next_words->next_element = current->next_words;
                current->next_words = next_words;
            }
        } else {
            // Se la parola successiva è già presente, aumenta il campo count
            next_words->count++;
        }
    }
}