#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"

// Funzione di serializzazione
char *serialize_list(struct word_element *head) {
    if (head == NULL) {
        return NULL;
    }

    // Calcola la lunghezza totale necessaria per la serializzazione
    int total_length = 1;  // Inizia con 1 per il separatore ',' iniziale
    struct word_element *current = head;

    while (current != NULL) {
        // Lunghezza della parola e del conteggio
        total_length += snprintf(NULL, 0, "%s,%d", current->word, current->count);

        struct next_word_element *next_words = current->next_words;
        while (next_words != NULL) {
            // Lunghezza della parola successiva e del suo conteggio
            total_length += snprintf(NULL, 0, ",%s,%d", next_words->word, next_words->count);
            next_words = next_words->next_element;
        }

        if (current->next_element != NULL) {
            total_length += 1;  // +1 per il separatore '|'
        }
        current = current->next_element;
    }

    // Alloca memoria per la stringa serializzata
    char *serialized_str = (char *)malloc(total_length + 1);  // +1 per il terminatore di stringa '\0'
    if (serialized_str == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Costruisci la stringa serializzata
    current = head;
    int offset = 0;

    while (current != NULL) {
        // Scrivi la parola e il conteggio
        offset += sprintf(serialized_str + offset, "%s,%d", current->word, current->count);
        struct next_word_element *next_words = current->next_words;
        while (next_words != NULL) {
            // Scrivi la parola successiva e il suo conteggio
            offset += sprintf(serialized_str + offset, ",%s,%d", next_words->word, next_words->count);
            next_words = next_words->next_element;
        }
        if (current->next_element != NULL) {
            offset += sprintf(serialized_str + offset, "|");
        }
        current = current->next_element;
    }

    return serialized_str;
}


