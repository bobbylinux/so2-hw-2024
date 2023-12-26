#include <stdio.h>
#include <stdlib.h>
#include "common.h"

/**
 * Questa funzione si occupa di analizzare la lista cocatenata ed elaborare l'output sotto forma di file csv.
 * @param head
 * @param output_file_name
 */
void write_output(struct word_element **head, char *output_file_name) {
    // apre il file di output in scrittura
    FILE *fp = fopen(output_file_name, "w");
    if (!fp) {
        perror("Error trying to open output file\n");
        exit(EXIT_FAILURE);
    }

    // assegna la testa della lista all'elemento corrente
    struct word_element *current = *head;

    // Cerca se la parola è già presente nella lista
    while (current != NULL) {
        fprintf(fp, "%s,", current->word);
        // Itera attraverso la lista next_words
        struct next_word_element *next_words = current->next_words;
        while (next_words != NULL) {
            fprintf(fp, "%s,", next_words->word);

            // Calcola la frequenza corretta sulla base delle occorrenze totali della parola successiva e di quella precedente
            double occurrences = (double) next_words->count / (double) current->count;

            // controlla se la frequenza è intera o decimale
            double decimal_part = occurrences - (int) occurrences;

            // se decimale formatta a 2 cifre dopo la virgola, se intero inserisce solo il valore intero
            if (decimal_part != 0.0) {
                fprintf(fp, "%.*lf", 2, round_down_to_decimal(occurrences, 2));
            } else {
                fprintf(fp, "%d", (int) occurrences);
            }

            // Se c'è un elemento successivo, stampa la virgola
            if (next_words->next_element != NULL) {
                fprintf(fp, ",");
            }
            //scorre la lista delle parole successive
            next_words = next_words->next_element;
        }
        // va a capo nel file
        fprintf(fp, "\n");
        // scorre la lista delle parole precedenti
        current = current->next_element;
    }

    // Chiude il file dopo averlo scritto
    fclose(fp);
}
