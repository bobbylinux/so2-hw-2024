#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gsl/rng/gsl_rng.h"
#include <time.h>
#include "common.h"

/**
 * Questa funzione serve per scrivere il file di testo che genera le parole casuali usando n caratteri e opzionalmente il carattere di partenza
 * @param head
 * @param number_of_words
 * @param start_word
 */
void write_text_file(const struct word_element *head, int number_of_words, char *start_word, char *output_file_name) {
    const struct word_element *current = head;
    struct next_word_element *next_word = NULL;
    int index = 0;
    int shifter = 0;
    int first_word_found = 0;
    int list_check = 0;

    // gestione dei numeri random
    const gsl_rng_type *rngType;// Inizializza il generatore di numeri casuali della GSL
    gsl_rng *rng;
    gsl_rng_env_setup();
    rngType = gsl_rng_default;
    rng = gsl_rng_alloc(rngType);
    // Imposta il seme basato sull'orologio di sistema
    gsl_rng_set(rng, time(NULL));

    // apre il file di output in scrittura
    FILE *fp = fopen(output_file_name, "w");
    if (!fp) {
        perror("Error trying to open output file\n");
        exit(EXIT_FAILURE);
    }

    // se start word è presente cerca la parola da cui partire nella lista concatenata
    if (start_word == NULL) {
        start_word = (char *) malloc(strlen(head->word) + 1);
        if (start_word == NULL) {
            perror("Error trying to allocate temp string");
            exit(EXIT_FAILURE);
        }
        strcpy(start_word, head->word);
    }
    // fin quando l'indice di parole inserite nel testo è minore del numero di parole passate come argomento cicla
    while (index < number_of_words) {
        // se la lista concatenata è stata scorsa tutta almeno due volte e il primo carattere non è stato ancora trovato si esce con errore, che vuol dire che l'elemento fornito in input non è stato trovato nella lista.
        if (list_check == 2 && !first_word_found) {
            perror("Please provide a valid word to start random phrases generation");
            exit(EXIT_FAILURE);
        }
        // se non lo trova scorre la lista concatenata
        while (current != NULL && strcmp(current->word, start_word) != 0) {
            current = current->next_element;
        }
        // se è stato trovato l'elemento
        if (current != NULL) {
            first_word_found = 1; // elemento trovato
            // se il count (che indica il numero di successori nel file csv fornito in input) è 1 non serve usare la randomizzazione della parola successiva da usare nel testo
            if (current->count == 1) {
                fprintf(fp, "%s", current->next_words->word);
                printf("%s ", current->next_words->word);
                strcpy(start_word, current->next_words->word);
            } else {
                next_word = current->next_words;
                shifter = (int) gsl_rng_uniform_int(rng, current->count);
                // altrimenti è necessario andare a pescare il successore che si trova all'indice indicato con shifter
                int k = 0;
                while (k < shifter) {
                    next_word = current->next_words->next_element;
                    k++;
                }
                // se non ci sono stati errori si procede
                if (next_word != NULL) {
                    fprintf(fp, "%s", next_word->word);
                    printf("%s ", next_word->word);
                    strcpy(start_word, next_word->word);
                }
            }
            index++;
            // aggiunge uno spazio tra parola e parola nel file
            if (number_of_words - index > 0) {
                fprintf(fp, "%c", SPACE);
            }
        } else {
            // non ha trovato l'elemento a fine lista, bisogna ripartire dalla testa almeno una volta
            current = head;
            // aggiungo 1 al numero di volte che è stata visitata la lista fino alla fine
            list_check++;
        }
    }
    // a fine frase stampa carattere a capo
    fprintf(fp, "%c", '\n');
    // chiude il file scritto
    fclose(fp);
    // Libera la memoria allocata per il generatore di numeri casuali
    gsl_rng_free(rng);
}
