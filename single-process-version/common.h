#ifndef SO2_HW_2024_COMMON_H
#define SO2_HW_2024_COMMON_H

// Dichiarazioni delle funzioni comuni
struct next_word_element *create_next_word_element(const char *word);
struct word_element *create_word_element(const char *word);
void add_or_update_element(struct word_element **head, const char *word, const char *next_word);
void write_output(struct word_element **head, char *output_file_name);
double round_down_to_decimal(double number, int decimalPlaces);
void read_csv_and_build_list(const char *filename, struct word_element **head);
void write_text_file(const struct word_element *head, int number_of_words, char *start_word, char *output_file_name);


#endif //SO2_HW_2024_COMMON_H
