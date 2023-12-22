#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
/**
 * Questa funzione serve per leggere il csv del punto 2 e per incanalare i dati nella lista concatenata che poi verrà usata
 * per costruire le frasi casuali.
 * @param filename
 * @param head
 */
void read_csv_and_build_list(const char *filename, struct word_element **head) {
    // apre il file in lettura
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error trying to open file\n");
        exit(EXIT_FAILURE);
    }
    // impone un limite massimo a 1024 della linea
    char line[1024];
    // scorre il file csv linea per linea
    while (fgets(line, sizeof(line), fp)) {
        char *word = strtok(line, ","); // preleva la parola prima del carattere separatore ,
        char *next_word; // aggiunge la parola successiva alla lista concatenata
        while ((next_word = strtok(NULL, ",")) != NULL) {
            // ignora le frequenze
            strtok(NULL, ",");
            // aggiunge l'elemento alla lista concatenata
            add_or_update_element(&*head, word, next_word);
        }
    }

    // chiude il file dopo averlo letto
    fclose(fp);
}
