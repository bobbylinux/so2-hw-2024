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

char **read_text_file(char *input_file_name, int pipe[]);
char *to_lower(char *str);
#endif //SO2_HW_2024_COMMON_H
