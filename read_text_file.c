#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "common.h"
/**
 * Questa funzione legge il file in input e memorizza le parole in un array di caratteri
 * @param input_file_name
 * @return char**
 */
char **read_text_file(char *input_file_name) {
    int chr, idx_letter = 0, idx_word = 0;
    char *word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE); // array che si riferisce alla singola parola
    char **words = (char **) malloc(sizeof(char *) * MAX_WORDS); // array che si riferisce all'insieme di parole

    // apertura del file e ottenimento del puntatore
    FILE *fp = fopen(input_file_name, "r");
    if (!fp) {
        perror("Error trying to open input file\n");
        exit(EXIT_FAILURE);
    }

    // ciclo sul file parola per parola fino a incontrare il carattere di fine file
    while ((chr = fgetc(fp)) != EOF) {
        if (idx_letter < MAX_WORD_SIZE - 1 && chr == SPACE || chr == QUESTION_MARK || chr == EXCLAMATION_MARK ||
            chr == DOT || chr == END_LINE || chr == TAB) {
            // se l'indice della singola lettera è minore del numero massimo consentito (30) e se il carattere analizzato è un carattere "terminale" si stacca la parola
            word[idx_letter] = EMPTY_CHAR; // si stacca la parola
            idx_letter = 0; // si riazzera l'indice
            // se la parola non contiene caratteri sporchi allora si procede alla memorizzazione della parola
            if (idx_word < MAX_WORDS && word[0] != EMPTY_CHAR && word[0] != END_LINE && word[0] != TAB) {
                // si effettua un to lower per rimuovere le maiuscole
                word = to_lower(word);
                // si memorizza la parola dentro l'array dell'insieme di parole (duplicazione della stringa)
                words[idx_word] = strdup(word);
                idx_word++;
                // riallocazione della variabile dedicata ad accogliere la singola parola
                word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
            }
            // se il carattere che si sta analizzando è uno tra ? . e ! allora si deve inserire anche questo nella lista delle parole, viceversa si scarta
            if (idx_word < MAX_WORDS && chr == QUESTION_MARK || chr == EXCLAMATION_MARK || chr == DOT) {
                word[0] = (char) chr;
                word[1] = EMPTY_CHAR;
                words[idx_word] = strdup(word);
                idx_word++;
                // riallocazione della variabile dedicata ad accogliere la singola parola
                word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
            }
            // se il carattere è una normale lettera la aggiunge alla variabile della parola.
        } else if (chr != EMPTY_CHAR && chr != COMMA && chr != COLON && chr != SEMICOLON && chr != EOF &&
                   chr != APOSTROPHE) {
            word[idx_letter] = (char) chr;
            idx_letter++;
            // se il carattere è un apostrofo, c'è bisogno di spezzare la parola ma di includere l'apostrofo nella parola stessa
        } else if (chr != EMPTY_CHAR && chr == APOSTROPHE) {
            word[idx_letter] = (char) chr;
            idx_letter++;
            word[idx_letter] = EMPTY_CHAR;
            idx_letter = 0;
            word = to_lower(word);
            words[idx_word] = strdup(word);
            idx_word++;
            // riallocazione della variabile dedicata ad accogliere la singola parola
            word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
        }
    }

    //si chiude la parola
    word[idx_letter] = EMPTY_CHAR;

    //si aggiunge la parola all'array per l'insieme di parole sempre stando attenti che non si inseriscano caratteri non ammessi
    if (idx_word < MAX_WORDS && chr != EMPTY_CHAR && chr != COMMA && chr != COLON && chr != SEMICOLON && chr != EOF) {
        word = to_lower(word);
        words[idx_word] = strdup(word);
        idx_word++;
    }

    // si aggiunge NULL alla fine dell'array di parole
    words[idx_word] = NULL;

    // si chiude il file
    fclose(fp);

    // viene restituito l'array di parole
    return words;
}