#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#define MAX_WORDS 10000
#define MAX_WORD_SIZE 30
#define EXCLAMATION_MARK '!'
#define QUESTION_MARK '?'
#define DOT '.'
#define SPACE ' '
#define END_LINE '\n'
#define TAB '\t'
#define EMPTY_CHAR '\0'
#define COLON ':'
#define COMMA ','
#define SEMICOLON ';'
#define APOSTROPHE '\''

//Struttura elemento lista concatenata per le parole
struct next_word_element {
    char word[30];
    int count;
    struct next_word_element *next_element;
};

//Struttura elemento figlio (il successivo) in lista concatenata per le parole
struct word_element {
    char word[30];
    int count;
    struct next_word_element *next_words;
    struct word_element *next_element;
};


char **read_text_file(char *input_file_name);

struct next_word_element *create_next_word_element(const char *word);

struct word_element *create_word_element(const char *word);

void add_or_update_element(struct word_element **head, const char *word, const char *next_word);

char *to_lower(char *str);

void write_output(struct word_element **head, char *output_file_name);

double roundDownToDecimal(double number, int decimalPlaces);

void read_csv_and_build_list(const char *filename, struct word_element **head);

void print_list(const struct word_element *head);

void write_text_file(const struct word_element *head, int number_of_words, char* start_word, char* output_file_name);

int main(int argc, char *argv[]) {
    int opt;
    char *input_file = NULL;
    char *start_word = NULL;
    int number_of_words;
    char **words = NULL;
    while ((opt = getopt(argc, argv, "1:2:::")) != -1) {
        switch (opt) {
            case '1':
                words = read_text_file(optarg);
                // Testa della lista concatenata
                struct word_element *head = NULL;

                int j = 1;
                for (int i = 0; words[i] != NULL; i++) {
                    if (words[j] == NULL) {
                        j = 0;
                    }
                    add_or_update_element(&head, words[i], words[j]);
                    j++;
                }
                write_output(&head, "output.csv");
                free(words);
                free(head);
                break;
            case '2':
                // Se -2 è specificato, leggi il nome del file
                if (argv[optind] != NULL) {
                    input_file = argv[optind];
                    optind++;
                    char *end_pointer;
                    number_of_words =  strtol(argv[optind], &end_pointer, 10);
                    if (argv[optind] == NULL || *end_pointer != '\0') {
                        // Gestisci il caso in cui ci sono caratteri non validi nella stringa
                        perror("Error trying to read number of words, characters not valid");
                        exit(EXIT_FAILURE);
                    }

                    optind++;
                    start_word = argv[optind];

                } else {
                    perror("Option -2 requires at least two argument\n");
                    exit(EXIT_FAILURE);
                }

                struct word_element *list = NULL;

                read_csv_and_build_list(input_file, &list);

//                print_list(list);
                write_text_file(list, number_of_words, start_word, "output.txt");

                break;
            default:
                perror("Unknown option, please select a valid option -1 or -2\n");
                exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    printf("Unknown option, please select a valid option -1 or -2\n");
    exit(EXIT_FAILURE);
}

/**
 * Questa funzione legge il file in input e memorizza le parole in un array di caratteri
 * @param input_file_name
 * @return char**
 */
char **read_text_file(char *input_file_name) {
    int chr, idx_letter = 0, idx_word = 0;
    char *word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
    char **words = (char **) malloc(sizeof(char *) * MAX_WORDS);

    FILE *fp = fopen(input_file_name, "r");
    if (!fp) {
        perror("Error trying to open input file\n");
        exit(EXIT_FAILURE);
    }

    while ((chr = fgetc(fp)) != EOF) {
        if (idx_letter < MAX_WORD_SIZE - 1 && chr == SPACE || chr == QUESTION_MARK || chr == EXCLAMATION_MARK ||
            chr == DOT || chr == END_LINE || chr == TAB) {
            word[idx_letter] = EMPTY_CHAR;
            idx_letter = 0;
            if (idx_word < MAX_WORDS && word[0] != EMPTY_CHAR && word[0] != END_LINE && word[0] != TAB) {
                word = to_lower(word);
                words[idx_word] = strdup(word);
                idx_word++;
                word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
            }
            if (idx_word < MAX_WORDS && chr == QUESTION_MARK || chr == EXCLAMATION_MARK || chr == DOT) {
                word[0] = (char) chr;
                word[1] = EMPTY_CHAR;
                words[idx_word] = strdup(word);
                idx_word++;
                word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
            }
        } else if (chr != EMPTY_CHAR && chr != COMMA && chr != COLON && chr != SEMICOLON && chr != EOF &&
                   chr != APOSTROPHE) {
            word[idx_letter] = (char) chr;
            idx_letter++;
        } else if (chr != EMPTY_CHAR && chr == APOSTROPHE) {
            word[idx_letter] = (char) chr;
            idx_letter++;
            word[idx_letter] = EMPTY_CHAR;
            idx_letter = 0;
            word = to_lower(word);
            words[idx_word] = strdup(word);
            idx_word++;
            word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE);
        }
    }

    word[idx_letter] = EMPTY_CHAR;

    if (idx_word < MAX_WORDS && chr != EMPTY_CHAR && chr != COMMA && chr != COLON && chr != SEMICOLON && chr != EOF) {
        word = to_lower(word);
        words[idx_word] = strdup(word);
        idx_word++;
    }

    words[idx_word] = NULL;

    fclose(fp);

    return words;
}

/**
 * Questa funzione restituisce un lower di una specifica parola passata in ingresso
 * @param str
 * @return char*
 */
char *to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char) str[i]);
    }
    return str;
}

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

    if (current == NULL) {
        // Se la parola non è presente, aggiungi un nuovo elemento alla lista
        struct word_element *new_element = create_word_element(word);

        // Aggiungi la parola successiva alla nuova struttura next_word_element
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
            // Se la parola successiva non è presente, aggiungi una nuova struttura next_word_element
            next_words = create_next_word_element(next_word);
            // Aggiungi la nuova struttura next_word_element alla lista
            next_words->next_element = current->next_words;
            current->next_words = next_words;
        } else {
            // Se la parola successiva è già presente, aumenta il campo count
            next_words->count++;
        }
    }
}

/**
 * Questa funzione si occupa di analizzare la lista cocatenata ed elaborare l'output sotto forma di file csv.
 * @param head
 * @param output_file_name
 */
void write_output(struct word_element **head, char *output_file_name) {
    FILE *fp = fopen(output_file_name, "w");
    if (!fp) {
        perror("Error trying to open output file\n");
        exit(EXIT_FAILURE);
    }

    struct word_element *current = *head;

    // Cerca se la parola è già presente nella lista
    while (current != NULL) {
        fprintf(fp, "%s,", current->word);
        // Itera attraverso la lista next_words
        struct next_word_element *next_words = current->next_words;
        while (next_words != NULL) {
            fprintf(fp, "%s,", next_words->word);

            // Calcola la frazione corretta
            double occurrences = (double) next_words->count / (double) current->count;

            double decimal_part = occurrences - (int) occurrences;

            if (decimal_part != 0.0) {
                fprintf(fp, "%.*lf", 2,roundDownToDecimal(occurrences, 2));

            } else {
                fprintf(fp, "%d", (int) occurrences);
            }

            // Se c'è un elemento successivo, stampa la virgola
            if (next_words->next_element != NULL) {
                fprintf(fp, ",");
            }
            next_words = next_words->next_element;
        }
        fprintf(fp, "\n");
        current = current->next_element;
    }

    // Chiudi il file dopo averlo scritto
    fclose(fp);
}

/**
 * Questa funzione serve per arrotondare i double a due cifre dopo la virgola.
 * @param number
 * @param decimalPlaces
 * @return double
 */
double roundDownToDecimal(double number, int decimalPlaces) {
    double multiplier = 1;
    for (int i = 0; i < decimalPlaces; i++) {
        multiplier *= 10;
    }

    return ((int) (number * multiplier)) / multiplier;
}

/**
 * Questa funzione serve per leggere il csv del punto 2 e per incanalare i dati nella lista concatenata che poi verrà usata
 * per costruire le frasi casuali.
 * @param filename
 * @param head
 */
void read_csv_and_build_list(const char *filename, struct word_element **head) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error trying to open file\n");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char *word = strtok(line, ",");
        char *next_word;
        while ((next_word = strtok(NULL, ",")) != NULL) {
            strtok(NULL, ",");//ignoro le frequenze
            add_or_update_element(&*head, word, next_word);
        }

    }

    fclose(fp);
}

void print_list(const struct word_element *head) {
    const struct word_element *current = head;
    while (current != NULL) {
        printf("%s, count: %d\n", current->word, current->count);

        const struct next_word_element *next_words = current->next_words;
        while (next_words != NULL) {
            printf("    %s, count: %d\n", next_words->word, next_words->count);
            next_words = next_words->next_element;
        }

        printf("\n");
        current = current->next_element;
    }
}

/**
 * Questa funzione serve per scrivere il file di testo che genera le parole casuali usando n caratteri e opzionalmente il carattere di partenza
 * @param head
 * @param number_of_words
 * @param start_word
 */
void write_text_file(const struct word_element *head, int number_of_words, char* start_word, char* output_file_name) {
    const struct word_element *current = head;
    FILE *fp = fopen(output_file_name, "w");
    if (!fp) {
        perror("Error trying to open output file\n");
        exit(EXIT_FAILURE);
    }

    while (current != NULL) {
        printf("%s, count: %d\n", current->word, current->count);

        const struct next_word_element *next_words = current->next_words;
        while (next_words != NULL) {
            printf("    %s, count: %d\n", next_words->word, next_words->count);
            next_words = next_words->next_element;
        }

        printf("\n");
        current = current->next_element;
    }
}