#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

/**
 * Questa funzione inizializza una nuova struttura next_word_element
 * @param word
 * @return next_word_element
 */
struct next_word_element *create_next_word_element(const char *word) {
    struct next_word_element *next_word_element = (struct next_word_element *) malloc(sizeof(struct next_word_element));
    if (next_word_element == NULL) {
        perror("Error trying to allocate new element on list\n");
        exit(EXIT_FAILURE);
    }
    strcpy(next_word_element->word, word);
    next_word_element->count = 1;
    next_word_element->next_element = NULL;

    return next_word_element;
}

/**
 * Questa funzione inizializza una nuova struttura word_element
 * @param word
 * @return word_element
 */

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

/**
 * Questa funzione prende in ingresso due parole e valuta se la prima esiste o meno nella lista concatenata,
 * se non esiste l'aggiunge, agganciando la seconda parola come parola successiva, viceversa aggiorna la parola
 * nella lista concatenata aggiungendo la parola successiva a quelle già presenti.
 * @param head
 * @param word
 * @param next_word
 */
void add_or_update_element(struct word_element **head, const char *word, const char *next_word) {
    struct word_element *current = *head;
    struct word_element *previous = NULL;
    // Cerca se la parola è già presente nella lista
    while (current != NULL && strcmp(current->word, word) != 0) {
        previous = current;
        current = current->next_element;
    }
    // Se la parola non è presente, aggiunge un nuovo elemento alla lista
    if (current == NULL) {
        struct word_element *new_element = create_word_element(word);

        // Aggiunge la parola successiva alla nuova struttura next_word_element
        new_element->next_words = create_next_word_element(next_word);

        // Collega il nuovo elemento alla lista
        if (previous == NULL) {
            // Se la lista è vuota, imposta il nuovo elemento come testa
            *head = new_element;
        } else {
            previous->next_element = new_element;
        }
    } else {
        current->count++;
        // Se la parola è già presente, cerca la parola successiva nell'array next_word_element
        struct next_word_element *next_words = current->next_words;
        while (next_words != NULL && strcmp(next_words->word, next_word) != 0) {
            next_words = next_words->next_element;
        }
        if (next_words == NULL) {
            // Se la parola successiva non è presente, aggiunge una nuova struttura next_word_element
            next_words = create_next_word_element(next_word);
            // Aggiunge la nuova struttura next_word_element alla lista
            next_words->next_element = current->next_words;
            current->next_words = next_words;
        } else {
            // Se la parola successiva è già presente, aumenta il campo count
            next_words->count++;
        }
    }
}