#include <stdlib.h>
#include "../common/common.h"

struct word_element *reverse_list(struct word_element *head) {
    struct word_element *prev = NULL;
    struct word_element *current = head;
    struct word_element *next = NULL;

    while (current != NULL) {
        next = current->next_element;
        current->next_element = prev;
        prev = current;
        current = next;
    }

    return prev;
}