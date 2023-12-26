#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/common.h"
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int opt;
    char *input_file = NULL;
    char *start_word = NULL;
    int number_of_words;
    char **words = NULL;
    int pipe1[2];
    int pipe2[2];
    //analizza i parametri in ingresso tramite getopt
    while ((opt = getopt(argc, argv, "1:2:::")) != -1) {
        switch (opt) {
            case '1': // caso 1 si legge il file indicato come argomento e genera il csv come output output.csv
                // creazione della prima pipe
                if (pipe(pipe1) == -1) {
                    perror("Error trying to create first pipe");
                    exit(EXIT_FAILURE);
                }
                // creazione del primo processo di lettura del file
                pid_t pid_1 = fork();
                if (pid_1 == -1) {

                } else if (pid_1 == 0) {
                    printf("Processo figlio\n");
                } else {
                    printf("Processo padre\n");
                }
                exit(EXIT_SUCCESS);
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
                break;

            default:
                //nel caso in cui nessun argomento venga specificato in esecuzione del file viene restituito un errore
                perror("Unknown option, please select a valid option -1 or -2\n");
                exit(EXIT_FAILURE);
        }
    }
}