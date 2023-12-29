#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#define MAX_TOKENS 10000

// Funzione di deserializzazione
struct word_element *deserialize_list(const char *serialized_list) {
    if (serialized_list == NULL || strlen(serialized_list) == 0) {
        return NULL;
    }
    // Allocazione dinamica dell'array di stringhe
    char **token_array = (char **)malloc(MAX_TOKENS * sizeof(char *));
    if (token_array == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    struct word_element *head = NULL;
    struct word_element *current = NULL;
    int i = 1;
    char* word;
    char* count;
    // Inizializzazione della tokenizzazione
    token_array[0] = strtok((char *)serialized_list, "|");

    // Tokenizzazione degli altri elementi
    while (i < MAX_TOKENS - 1) {
        token_array[i] = strtok(NULL, "|");
        if (token_array[i] == NULL) {
            break;  // Se non ci sono più token, esci dal ciclo
        }
        i++;
    }
    for (int j=0; j < i; j++) {
        // Alloca memoria per la nuova parola
        struct word_element *new_element = (struct word_element *)malloc(sizeof(struct word_element));
        if (new_element == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        word = strtok(token_array[j], ",");
        count = strtok(NULL, ",");
        strcpy(new_element->word, word);
        new_element->count = atoi(count);
        new_element->next_words = NULL;
        new_element->next_element = NULL;
        char *sub_token = strtok(NULL, ",");
        while (sub_token!= NULL) {
            // Alloca memoria per la nuova sotto-parola
            struct next_word_element *next_word = (struct next_word_element *)malloc(sizeof(struct next_word_element));
            if (next_word == NULL) {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
            }
            // Tokenizza la sotto-parola e il conteggio
            char *next_word_str = sub_token;
            char *next_word_count_str = strtok(NULL, ",");
            strcpy(next_word->word, next_word_str);
            next_word->count = atoi(next_word_count_str);
            // Collega la sotto-parola alla lista
            next_word->next_element = new_element->next_words;
            new_element->next_words = next_word;
            sub_token = strtok(NULL, ",");
        }
        // Collega la nuova parola alla lista
        if (head == NULL) {
            head = new_element;
            current = new_element;
        } else {
            current->next_element = new_element;
            current = new_element;
        }
    }

    return head;
}