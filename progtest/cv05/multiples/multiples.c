#include <stdio.h>
#include <stdlib.h>
#include <cstring>

int verify_number(int number) {
    return number > 0;
}

int load_number(const char *message, int *result) {
    if (message != NULL) {
        printf("%s", message);
    }
    if (scanf("%d", result) == 1 &&
        verify_number(*result)) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int number_of_digits(int number) {
    int digits = 0;
    while (number > 0) {
        number /= 10;
        digits++;
    }
    return digits;
}

void print_number(int number, int digits) {
    int number_digits = number_of_digits(number);
    for (int i = digits; i > 0; --i) {
        if (i > number_digits) {
            putchar(' ');
        } else {
            printf("%d", number);
            return;
        }
    }
}

void print_message(const char *message, int length, char fill) {
    int message_length = (int) strlen(message);
    for (int i = length; i >= 0; --i) {
        if (i >= message_length) {
            putchar(fill);
        } else {
            printf("%s", message);
            return;
        }
    }
}

int main(void) {
    int size;
    if (load_number("Rozsah:\n", &size) == EXIT_SUCCESS) {
        int max_length = number_of_digits(size * size) + 1;
        int max_length_info = max_length - 1;
        // HEADER
        for (int i = -1; i < size; ++i) {
            if (i == -1) {
                print_message("|", max_length_info, ' ');
            } else {
                print_number(size - i, max_length);
            }
        }
        putchar('\n');
        print_message("+", max_length_info, '-');
        print_message("", max_length * size - 1, '-');
        putchar('\n');
        for (int row = 1; row <= size; ++row) {
            for (int i = 0; i <= size - row + 1; ++i) {
                if (i == 0) {
                    print_number(row, max_length_info);
                    putchar('|');
                } else {
                    print_number(row * (size - i + 1), max_length);
                }
            }
            putchar('\n');
        }
        return EXIT_SUCCESS;
    }
    printf("Nespravny vstup.\n");
    return EXIT_FAILURE;
}

