#include <stdio.h>
#include <stdlib.h>

void number_of_digits(long number, int base, int *digits, long *compare) {
    while (number >= *compare) {
        *compare *= base;
        *digits = *digits + 1;
    }
}

void print_number(long number, int base) {
    if (number == 0) {
        putchar('0');
        return;
    }
    int digits_count=1;
    long compare = base;
    number_of_digits(number, base, &digits_count, &compare);
    char *digits = (char *) malloc(sizeof(int) * digits_count);
    int index = 0;
    while (number > 0) {
        long digit = number % base;
        digits[index++] = (char) (digit + (digit > 9 ? 'a' - 10 : '0'));
        number /= base;
    }
    for (int g = index-1; g >= 0; g--) {
        putchar(digits[g]);
    }
    free(digits);
}

int main(void) {
    long position;
    int base;
    printf("Pozice a soustava:\n");
    int err;
    while ((err = scanf("%ld %d", &position, &base)) == 2 && position >= 0 && base >= 2 && base <= 36) {
        long number = 0;
        int digits=1, current_position = 0;
        long compare=base;
        while (1) {
            number_of_digits(number, base,&digits,&compare);
            current_position += digits;
            if (current_position > position) {
                break;
            }
            ++number;
        }
        print_number(number, base);
        putchar('\n');
        for (int i = 0; i < position - (current_position - digits); ++i) {
            putchar(' ');
        }
        puts("^");
    }
    if (err != EOF) {
        printf("Nespravny vstup.\n");
        return 1;
    }
    return 0;
}