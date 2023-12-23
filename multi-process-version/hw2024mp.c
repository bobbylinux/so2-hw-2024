#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/common.h"

int main(int argc, char *argv[]) {
    printf("hello from multiprocess");
    // dichiarazione delle pipe
    int pipe1[2];
    int pipe2[2];

    // creazione della prima pipe
    if (pipe(pipe1) == -1) {
        perror("Error trying to create first pipe");
        exit(EXIT_FAILURE);
    }

    // creazione del primo processo di lettura del file
    pid_t pid_read_file = fork();
    if (pid_read_file == -1) {
        perror("Error trying to fork the first process");
        exit(EXIT_FAILURE);
    } else if (pid_read_file == 0) {
        // siamo nel processo figlio

    }


}