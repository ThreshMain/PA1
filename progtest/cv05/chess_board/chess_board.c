#include <stdio.h>
#include <stdlib.h>

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

int main(void) {
    int fields_per_row;
    int size_per_field;
    if (load_number("Zadejte pocet poli:\n", &fields_per_row) == EXIT_SUCCESS &&
        load_number("Zadejte velikost pole:\n", &size_per_field) == EXIT_SUCCESS) {
        int row_count = fields_per_row * size_per_field + 2;
        for (int row_number = 0; row_number < row_count; ++row_number) {
            int border = row_number == 0 || row_number == row_count - 1;
            putchar(border ? '+' : '|');
            if (border) {
                for (int i = 0; i < row_count - 2; ++i) {
                    putchar('-');
                }
            } else {
                for (int i = 0; i < row_count - 2; ++i) {
                    if ((i / size_per_field) % 2 != ((row_number - 1) / size_per_field) % 2) {
                        putchar('X');
                    } else {
                        putchar(' ');
                    }
                }
            }
            printf("%c\n", border ? '+' : '|');
        }
        return EXIT_SUCCESS;
    }
    printf("Nespravny vstup.\n");
    return EXIT_FAILURE;
}

