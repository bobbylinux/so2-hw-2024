#ifndef SO2_HW_2024_COMMON_H
#define SO2_HW_2024_COMMON_H

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

// Dichiarazioni di strutture
// Struttura elemento lista concatenata per le parole
struct next_word_element {
    char word[30];
    int count;
    struct next_word_element *next_element;
};

// Struttura elemento figlio (il successivo) in lista concatenata per le parole
struct word_element {
    char word[30];
    int count;
    struct next_word_element *next_words;
    struct word_element *next_element;
};


// Dichiarazioni delle funzioni comuni
char **read_text_file(char *input_file_name);
char *to_lower(char *str);
struct next_word_element *create_next_word_element(const char *word);
struct word_element *create_word_element(const char *word);
void add_or_update_element(struct word_element **head, const char *word, const char *next_word);
void write_output(struct word_element **head, char *output_file_name);
double round_down_to_decimal(double number, int decimalPlaces);
void read_csv_and_build_list(const char *filename, struct word_element **head);
void print_list(const struct word_element *head);
void write_text_file(const struct word_element *head, int number_of_words, char *start_word, char *output_file_name);


#endif //SO2_HW_2024_COMMON_H
