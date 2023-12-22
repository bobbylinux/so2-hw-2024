#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <gsl/gsl_rng.h>

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

void write_text_file(const struct word_element *head, int number_of_words, char *start_word, char *output_file_name);

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
    }
    // se qualcosa è andato storto si esce comunque con errore.
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
    char *word = (char *) malloc(sizeof(char) * MAX_WORD_SIZE); // array che si riferisce alla singola parola
    char **words = (char **) malloc(sizeof(char *) * MAX_WORDS); // array che si riferisce all'insieme di parole

    // apertura del file e ottenimento del puntatore
    FILE *fp = fopen(input_file_name, "r");
    if (!fp) {
        perror("Error trying to open input file\n");
        exit(EXIT_FAILURE);
    }

    // ciclo sul file parola per parola fino ad incontrare il carattere di fine file
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
                fprintf(fp, "%.*lf", 2, roundDownToDecimal(occurrences, 2));
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

    // apre il file di output in scrittura
    FILE *fp = fopen(output_file_name, "w");
    if (!fp) {
        perror("Error trying to open output file\n");
        exit(EXIT_FAILURE);
    }

    // se start word è presente cerca la parola da cui partire nella lista concatenata
    if (start_word == NULL) {
        start_word =  (char *)malloc(strlen(head->word) + 1);
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
                shifter = (int) gsl_rng_uniform_int(rng, current->count);
                // altrimenti è necessario andare a pescare il successore che si trova all'indice indicato con shifter
                for (int i = 0; i <= shifter; i++) {
                    next_word = current->next_words->next_element;
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
            fprintf(fp, "%c", SPACE);
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

