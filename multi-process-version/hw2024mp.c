#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/common.h"
#include <sys/wait.h>
#include <string.h>

/**
 * Questa funzione è il punto di ingresso del programma multi process.
 * Utilizza pipe per passare dati tra processi.
 * L'opzione -1 utilizza tre processi a cascata.
 * L'opzione -2 utilizza due processi a cascata.
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
                    perror("Error trying to fork the first process");
                    exit(EXIT_FAILURE);
                } else if (pid_1 == 0) {
                    // processo figlio 1
                    close(pipe1[1]); // Chiude la parte di scrittura della pipe nel figlio
                    char received_word[MAX_WORD_SIZE];
                    char **received_words = NULL;
                    int num_words = 0;
                    while (read(pipe1[0], received_word, MAX_WORD_SIZE) > 0) {
                        if (strlen(received_word) > 0) {
                            num_words++;
                            received_words = (char **) realloc(received_words, num_words * sizeof(char *));
                            if (received_words == NULL) {
                                perror("Memory allocation error");
                                exit(EXIT_FAILURE);
                            }
                            received_words[num_words - 1] = strdup(received_word);
                        }
                    }
                    close(pipe1[0]); // Chiude la parte di lettura della pipe nel figlio
                    // Testa della lista concatenata
                    struct word_element *list = NULL;
                    // valorizzo il punto con successore la prima parola della frase
                    if (received_words != NULL) {
                        add_or_update_element(&list, ".", received_words[0]);
                        int j = 1;
                        // valorizza la lista concatenata con le parole dell'array
                        for (int i = 0; j < num_words; i++) {
                            add_or_update_element(&list, received_words[i], received_words[j]);
                            j++;
                        }
                    }
                    // creazione della seconda pipe
                    if (pipe(pipe2) == -1) {
                        perror("Error trying to create second pipe");
                        exit(EXIT_FAILURE);
                    }
                    pid_t pid_2 = fork();  // Processo figlio 2
                    if (pid_2 == -1) {
                        perror("Error trying to fork the first process");
                        exit(EXIT_FAILURE);
                    } else if (pid_2 == 0) {
                        //processo figlio 2
                        char serialized_list[MAX_SERIALIZED_SIZE];
                        close(pipe2[1]); // chiudere la pipe in scrittura nel processo figlio 2
                        ssize_t bytes_read = read(pipe2[0], serialized_list, MAX_SERIALIZED_SIZE);
                        if (bytes_read <= 0) {
                            // Gestire l'errore o l'assenza di dati
                            perror("Error reading from pipe2 or empty serialized list\n");
                            exit(EXIT_FAILURE);
                        }
                        serialized_list[bytes_read] = '\0';  // Assicurati di terminare la stringa con null character
                        // Deserializza la lista concatenata
                        struct word_element *received_list = deserialize_list(serialized_list);
                        // Ora è possibile scrivere il file output.csv utilizzando la lista concatenata
                        write_output(&received_list, "output.csv");
                        // Liberare la memoria utilizzata per la deserializzazione
                        free(received_list);
                        exit(EXIT_SUCCESS);
                    } else {
                        // processo figlio 1
                        // Serializza la lista concatenata prima di inviarla attraverso la pipe
                        char *serialized_list = serialize_list(list);
                        close(pipe2[0]); // Chiudo la pipe in lettura
                        // Invia la lista serializzata attraverso la pipe2
                        write(pipe2[1], serialized_list, strlen(serialized_list) + 1);
                        waitpid(pid_2, NULL, 0);
                        free(serialized_list);
                        exit(EXIT_SUCCESS);
                    }
                } else {
                    //processo padre deve leggere le parole dal file
                    close(pipe1[0]); // Chiude la parte di lettura della pipe
                    // legge il file di input e sposta le parole nell'array di char words ripulendo il testo
                    words = read_text_file(optarg);
                    for (int i = 0; words[i] != NULL; i++) {
                        write(pipe1[1], words[i], strlen(words[i]) + 1); // Invia ogni parola separatamente
                        usleep(10000);
                    }
                    // Invia una stringa vuota come marca di fine
                    write(pipe1[1], "", 1);
                    close(pipe1[1]); // Chiude la parte di lettura della pipe
                    // libera la memoria
                    waitpid(pid_1, NULL, 0);  // Attendi il figlio 1
                    exit(EXIT_SUCCESS);
                }
            case '2': // caso 2 viene letto il file in input e aggiunte le parole ad una lista concatenata
                if (optind < argc && argv[optind] != NULL) {
                    input_file = argv[optind];
                    optind++;
                    if (optind < argc) {
                        // viene convertito la stringa che indica il numero di parole in un intero
                        char *end_pointer;
                        number_of_words = (int) strtol(argv[optind], &end_pointer, 10);
                        if (argv[optind] == NULL || *end_pointer != '\0') {
                            // gestisce il caso in cui ci sono caratteri non validi nella stringa
                            perror("Error trying to read number of words, characters not valid");
                            exit(EXIT_FAILURE);
                        }
                        optind++;
                        // nel caso in cui è presente prende anche la parola dalla quale iniziale le frasi random
                        start_word = argv[optind];
                    } else {
                        perror("Option -2 requires at least two argument\n");
                        exit(EXIT_FAILURE);
                    }
                } else { //se gli argomenti non sono presenti va in errore.
                    perror("Option -2 requires at least two argument\n");
                    exit(EXIT_FAILURE);
                }
                // creazione della pipe
                if (pipe(pipe1) == -1) {
                    perror("Error trying to create first pipe");
                    exit(EXIT_FAILURE);
                }
                // creazione del primo processo di lettura del file
                pid_t pid_3 = fork();
                if (pid_3 == -1) {
                    perror("Error trying to fork the third process");
                    exit(EXIT_FAILURE);
                } else if (pid_3 == 0) {
                    // processo figlio
                    char serialized_list[MAX_SERIALIZED_SIZE];
                    close(pipe1[1]); // chiudere la pipe in scrittura nel processo figlio 2
                    ssize_t bytes_read = read(pipe1[0], serialized_list, MAX_SERIALIZED_SIZE);
                    if (bytes_read <= 0) {
                        // Gestire l'errore o l'assenza di dati
                        perror("Error reading from pipe2 or empty serialized list\n");
                        exit(EXIT_FAILURE);
                    }
                    serialized_list[bytes_read] = '\0';  // Assicurati di terminare la stringa con null character
                    // Deserializza la lista concatenata
                    struct word_element *received_list = deserialize_list(serialized_list);
                    // Ora è possibile scrivere il file output.csv utilizzando la lista concatenata
                    write_text_file(received_list, number_of_words, start_word, "output.txt");
                    // Liberare la memoria utilizzata per la deserializzazione
                    free(received_list);
                    exit(EXIT_SUCCESS);
                } else {
                    // processo padre
                    // inizializza la lista concatenata vuota
                    struct word_element *list = NULL;
                    // viene letto il file di input e viene costruita la lista concatenata delle parole con i rispettivi successori
                    read_csv_and_build_list(input_file, &list);
                    char *serialized_list = serialize_list(list);
                    close(pipe1[0]); // Chiudo la pipe in lettura
                    // Invia la lista serializzata attraverso la pipe2
                    write(pipe1[1], serialized_list, strlen(serialized_list) + 1);
                    waitpid(pid_3, NULL, 0); // Attendi il figlio
                    exit(EXIT_SUCCESS);
                }
            default:
                //nel caso in cui nessun argomento venga specificato in esecuzione del file viene restituito un errore
                exit(EXIT_FAILURE);
        }
    }
}