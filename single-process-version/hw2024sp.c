#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/common.h"

/**
 * Questa funzione è il punto di ingresso del programma single process
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
    int opt;
    char *input_file = NULL;
    char *start_word = NULL;
    int number_of_words;
    char **words = NULL;
    //analizza i parametri in ingresso tramite getopt
    while ((opt = getopt(argc, argv, "1:2:::")) != -1) {
        switch (opt) {
            case '1': // caso 1 si legge il file indicato come argomento e genera il csv come output output.csv
                // legge il file di input e sposta le parole nell'array di char words ripulendo il testo
                words = read_text_file(optarg);
                // testo vuoto
                if (words[0] == NULL) {
                    perror("The input file is empty");
                    exit(EXIT_FAILURE);
                }
                // Testa della lista concatenata
                struct word_element *head = NULL;
                // valorizzo il punto con successore la prima parola della frase
                add_or_update_element(&head, ".", words[0]);
                int j = 1;
                // valorizza la lista concatenata con le parole dell'array
                for (int i = 0; words[j] != NULL; i++) {
                    add_or_update_element(&head, words[i], words[j]);
                    j++;
                }
                // viene scritto l'output nel file output.csv
                write_output(&head, "output.csv");
                // libera la memoria
                free(words);
                free(head);
                break;
            case '2': // caso 2 viene letto il file in input e aggiunte le parole ad una lista concatenata
                if (argv[optind] != NULL) {
                    input_file = argv[optind];
                    optind++;
                    // viene convertito la stringa che indica il numero di parole in un intero
                    char *end_pointer;
                    number_of_words = strtol(argv[optind], &end_pointer, 10);
                    if (argv[optind] == NULL || *end_pointer != '\0') {
                        // gestisce il caso in cui ci sono caratteri non validi nella stringa
                        perror("Error trying to read number of words, characters not valid");
                        exit(EXIT_FAILURE);
                    }

                    optind++;
                    // nel caso in cui è presente prende anche la parola dalla quale iniziale le frasi random
                    start_word = argv[optind];
                } else { //se gli argomenti non sono presenti va in errore.
                    perror("Option -2 requires at least two argument\n");
                    exit(EXIT_FAILURE);
                }
                // inizializza la lista concatenata vuota
                struct word_element *list = NULL;
                // viene letto il file di input e viene costruita la lista concatenata delle parole con i rispettivi successori
                read_csv_and_build_list(input_file, &list);
                // viene scritta la frase di parole casuali sul file output.txt
                write_text_file(list, number_of_words, start_word, "output.txt");
                // liberata la memoria
                free(list);
                break;
            default:
                //nel caso in cui nessun argomento venga specificato in esecuzione del file viene restituito un errore
                perror("Unknown option, please select a valid option -1 or -2\n");
                exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    // se qualcosa è andato storto si esce comunque con errore.
    printf("Unknown option, please select a valid option -1 or -2\n");
    exit(EXIT_FAILURE);
}





