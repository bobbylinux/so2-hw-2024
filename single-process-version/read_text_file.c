#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../common/common.h"
/**
 * Questa funzione legge il file in input e memorizza le parole in un array di caratteri
 * @param input_file_name
 * @return char**
 */
void read_input_file(char *input_file_name, int pipe_fd[]) {
    int chr, idx_letter = 0;
    char *word = (char *)malloc(sizeof(char) * MAX_WORD_SIZE);

    FILE *fp = fopen(input_file_name, "r");
    if (!fp) {
        perror("Error trying to open input file\n");
        exit(EXIT_FAILURE);
    }

    // Ciclo sul file parola per parola fino a incontrare il carattere di fine file
    while ((chr = fgetc(fp)) != EOF) {
        if (idx_letter < MAX_WORD_SIZE - 1 && (chr == SPACE || chr == QUESTION_MARK || chr == EXCLAMATION_MARK ||
                                               chr == DOT || chr == END_LINE || chr == TAB)) {
            word[idx_letter] = EMPTY_CHAR;
            idx_letter = 0;

            if (word[0] != EMPTY_CHAR && word[0] != END_LINE && word[0] != TAB) {
                word = to_lower(word);
                // Scrivi la parola nella pipe
                write(pipe_fd[1], word, strlen(word) + 1);
            }

            // Se il carattere è uno tra ?, . e !, scrivilo nella pipe
            if (chr == QUESTION_MARK || chr == EXCLAMATION_MARK || chr == DOT) {
                word[0] = (char)chr;
                word[1] = EMPTY_CHAR;
                // Scrivi il carattere nella pipe
                write(pipe_fd[1], word, strlen(word) + 1);
            }
        } else if (chr != EMPTY_CHAR && chr != EOF && chr != APOSTROPHE) {
            word[idx_letter] = (char)chr;
            idx_letter++;
        }
    }

    // Chiudi il file
    fclose(fp);

    // Chiudi la parte di scrittura della pipe
    close(pipe_fd[1]);
}