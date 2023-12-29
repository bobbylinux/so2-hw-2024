#include <stdlib.h>
#include <string.h>
#include "../common/common.h"

void free_list(struct word_element *head) {
    struct word_element *current = head;
    while (current != NULL) {
        struct word_element *next = current->next_element;
        free(current->word);
        free(current);
        current = next;
    }
}