#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARATOR '"'

typedef struct {
    char *chars;
    int length;
    int actual_length;
    int capacity;
} string_t;

string_t *create_string() {
    string_t *s = (string_t *) malloc(sizeof(string_t));
    s->chars = (char *) malloc(sizeof(char) * 1);
    s->chars[0] = '\0';
    s->length = 0;
    s->capacity = 1;
    s->actual_length = 0;
    return s;
}

void add_char_to_string(string_t *s, char c) {
    if (s->length >= s->capacity - 1) {
        s->capacity *= 2;
        s->chars = (char *) realloc(s->chars, sizeof(char) * s->capacity);
    }
    s->chars[s->length] = c;
    s->chars[s->length + 1] = 0;
    s->length++;
    s->actual_length++;
}

void free_string(string_t *s) {
    if (s == NULL) return;
    free(s->chars);
    free(s);
}

int load_acronym(string_t *out) {
    int input;
    while ((input = getchar()) != EOF && ((input >= 'A' && input <= 'Z') || input == '\n')) {
        if (input == '\n') {
            return out->length>0;
        } else {
            add_char_to_string(out, (char) (input - ('A' - 'a')));
        }
    }
    return input == EOF ? EOF : 0;
}

int read_custom_line(string_t *buffer) {
    int input;
    int separator_count = 0;
    while ((input = getchar()) != EOF) {
        if (input == '\n') {
            return separator_count == 2;
        } else if (input == SEPARATOR) {
            if (separator_count == 0 || separator_count == 1) {
                separator_count++;
            }
        } else {
            if (separator_count == 0 && input >= 'A' && input <= 'Z') {
                return 0;
            } else if (separator_count == 1) {
                input += (input >= 'A' && input <= 'Z') ? 'a' - 'A' : 0;
                add_char_to_string(buffer, (char) input);
            }
        }
    }
    return input == EOF ? EOF : 0;
}

void shift_string(string_t *string, int positions) {
    string->length -= positions;
    string->chars = string->chars + positions;
}


int solve(string_t *acronym, string_t *input, int max_count, int current_count, char operation) {
    if (acronym->length == 0) {
        if (operation == '?') {
            printf("\"%s\"\n", input->chars - (input->actual_length - input->length));
        }
        return 1;
    } else if (input->length == 0) {
        return 0;
    }
    int solution = 0;
    if (input->chars[0] == acronym->chars[0] && current_count > 0) {
        input->chars[0] -= 'a' - 'A';
        shift_string(input, 1);
        shift_string(acronym, 1);
        solution += solve(acronym, input, max_count, current_count - 1, operation);
        shift_string(input, -1);
        shift_string(acronym, -1);
        input->chars[0] += 'a' - 'A';
    }
    if (input->chars[0] == ' ') {
        shift_string(input, 1);
        solution += solve(acronym, input, max_count, max_count, operation);
        shift_string(input, -1);
        return solution;
    }
    shift_string(input, 1);
    solution += solve(acronym, input, max_count, current_count, operation);
    shift_string(input, -1);
    return solution;
}


int acronym_finder() {
    printf("Zkratka:\n");
    string_t *acronym = create_string();
    int status = load_acronym(acronym);
    if (status == EOF || status != 1) {
        free_string(acronym);
        return 0;
    }
    printf("Problemy:\n");
    string_t *input = NULL;
    int in_one_word;
    char action;
    while ((status = scanf("%c %d", &action, &in_one_word)) == 2 && in_one_word > 0 &&
           (action == '?' || action == '#')) {
        input = create_string();
        status = read_custom_line(input);
        if (status != 1) break;
        if (action == '#') {
            for (int i = 1; i <= in_one_word; ++i) {
                int count = solve(acronym, input, i, i, action);
                printf("> limit %d: %d\n", i, count);
            }
        } else {
            int count = solve(acronym, input, in_one_word, in_one_word, action);
            printf("> %d\n", count);
        }
        free_string(input);
        input = NULL;
    }
    free_string(acronym);
    free_string(input);
    if (status == EOF) {
        return 1;
    }
    return 0;
}

int main() {
    if (!acronym_finder()) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    return 0;
}