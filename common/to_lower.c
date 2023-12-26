#include <ctype.h>

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